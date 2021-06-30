#include "pch.h"
#include "OpenGL_Window.hpp"
#include "Engine/FileUtils/OSPaths.hpp"
#include "stb_image.h"

#define SPH_PROPAGATE(e) \
	EventFunc callback = *(EventFunc*)glfwGetWindowUserPointer(win); \
	callback(e);


namespace Sharpheus {

	static void GLFWErrorCallback(int error, const char* description)
	{
		SPH_ERROR("GLFW Error[{0}]: {1}", error, description);
	}


	OpenGL_Window::OpenGL_Window(const Window::Props& props) : Window(props)
	{
		int success = glfwInit();
		SPH_ASSERT(success == GLFW_TRUE, "Error during GLFW initialization!");

		glfwSetErrorCallback(GLFWErrorCallback);
		win = glfwCreateWindow(props.width, props.height, props.title.c_str(), props.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
		SPH_INFO("Window created: {0}x{1}({2})", props.width, props.height, props.fullscreen ? "Fullscreen" : "Windowed");
		glfwMakeContextCurrent(win);

		glfwSetWindowUserPointer(win, &callback);
		SetCallbacks();
		SetVsync(props.vsync);

		startTime = glfwGetTime();
		lastSecond = startTime;

		Renderer::Init();
		Renderer::SetBackgroundColor(props.background);
		Camera::SetStaticRect(props.width, props.height);
		Camera::SetOGHeight(props.height);

		EventHandler::Subscribe<KeyPressedEvent>(0, SPH_BIND(OpenGL_Window::HandleInnerKeyPress));
		EventHandler::Subscribe<KeyReleasedEvent>(0, SPH_BIND(OpenGL_Window::HandleInnerKeyRelease));

		GLFWimage icons[1];
		int channels;
		icons[0].pixels = stbi_load((OSPaths::Get(OSPaths::Folder::EXEC_FOLDER) + "\\Assets\\Icon\\pngicon.png").c_str(), &icons[0].width, &icons[0].height, &channels, 0);
		glfwSetWindowIcon(win, 1, icons);
		stbi_image_free(icons[0].pixels);
	}


	OpenGL_Window::~OpenGL_Window()
	{
		EventHandler::UnSubscribeAll(0);
		glfwDestroyWindow(win);
	}


	void OpenGL_Window::PollEvents()
	{
		NotifyKeyHold();
		glfwPollEvents();
	}


	void OpenGL_Window::StartRender()
	{
		HandleFPS();
		Renderer::StartFrame();
	}


	void OpenGL_Window::EndRender()
	{
		Renderer::EndFrame();
		glfwSwapBuffers(win);
	}


	float OpenGL_Window::GetTime()
	{
		return glfwGetTime() - startTime;
	}


	uint32_t OpenGL_Window::GetFPS()
	{
		return lastFps;
	}


	void OpenGL_Window::SetWidth(uint32_t width)
	{
		props.width = width;
		glfwSetWindowSize(win, props.width, props.height);
	}


	void OpenGL_Window::SetHeight(uint32_t height)
	{
		props.height = height;
		glfwSetWindowSize(win, props.width, props.height);
	}


	void OpenGL_Window::SetTitle(const std::string& title)
	{
		props.title = title;
		glfwSetWindowTitle(win, props.title.c_str());
	}


	void OpenGL_Window::SetVsync(bool vsync)
	{
		props.vsync = vsync;
		glfwSwapInterval(vsync ? 1 : 0);
	}


	void OpenGL_Window::SetFullscreen(bool fullscreen)
	{
		if (props.fullscreen == fullscreen) {
			return;
		}

		props.fullscreen = fullscreen;
		if (fullscreen) {
			glfwGetWindowPos(win, &lastPos[0], &lastPos[1]);
			glfwGetWindowSize(win, &lastSize[0], &lastSize[1]);

			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwSetWindowMonitor(win, monitor, 0, 0, mode->width, mode->height, 0);
		} else {
			glfwSetWindowMonitor(win, nullptr, lastPos[0], lastPos[1], lastSize[0], lastSize[1], 0);
		}
	}

	void OpenGL_Window::SetBackground(const Color& background)
	{
		props.background = background;
		Renderer::SetBackgroundColor(background);
	}


	void OpenGL_Window::HandleInnerKeyPress(const KeyPressedEvent& e)
	{
		pressedKeys.insert(e.code);
	}


	void OpenGL_Window::HandleInnerKeyRelease(const KeyReleasedEvent& e)
	{
		pressedKeys.erase(e.code);
	}


	void OpenGL_Window::HandleFPS()
	{
		float currentTime = glfwGetTime();
		if (lastSecond + 1 < currentTime) {
#ifndef SPH_FINAL
			glfwSetWindowTitle(win, (props.title + " [" + std::to_string(fps) + " FPS]").c_str());
#endif
			lastSecond = currentTime;
			lastFps = fps;
			fps = 0;
		}
		else {
			++fps;
		}
	}


	void OpenGL_Window::NotifyKeyHold()
	{
		for (auto it = pressedKeys.begin(); it != pressedKeys.end(); ++it) {
			EventHandler::Handle(KeyHoldEvent(*it));
		}
	}


	void OpenGL_Window::SetCallbacks()
	{
		glfwSetWindowCloseCallback(win, [](GLFWwindow* win) {
			WindowClosedEvent e;
			SPH_PROPAGATE(e);
		});

		glfwSetWindowSizeCallback(win, [](GLFWwindow* win, int newWidth, int newHeight) {
			WindowResizedEvent e(newWidth, newHeight);
			SPH_PROPAGATE(e);
		});

		glfwSetKeyCallback(win, [](GLFWwindow* win, int keyCode, int scancode, int action, int mods) {
			switch (action) {
				case GLFW_PRESS:
					{
						KeyPressedEvent e((KeyCode)keyCode);
						SPH_PROPAGATE(e);
					}
					break;
				case GLFW_RELEASE:
					{
						KeyReleasedEvent e((KeyCode)keyCode);
						SPH_PROPAGATE(e);
					}
					break;
			}
		});

		glfwSetCursorPosCallback(win, [](GLFWwindow* win, double x, double y) {
			Camera* camera = Renderer::GetCamera();
			Point screenPos(x, y);
			Point gamePos = (camera != nullptr) ? camera->ScreenPosToGamePos(screenPos) : Point();
			MouseMovedEvent e(screenPos, gamePos);
			SPH_PROPAGATE(e);
		});

		glfwSetMouseButtonCallback(win, [](GLFWwindow* win, int button, int action, int mods) {
			double x, y;
			glfwGetCursorPos(win, &x, &y);
			Camera* camera = Renderer::GetCamera();
			Point screenPos(x, y);
			Point gamePos = (camera != nullptr) ? camera->ScreenPosToGamePos(screenPos) : Point();
			switch (action) {
				case GLFW_PRESS:
					{
						MousePressedEvent e(screenPos, gamePos, (MouseButton)button);
						SPH_PROPAGATE(e);
					}
					break;
				case GLFW_RELEASE:
					{
						MouseReleasedEvent e(screenPos, gamePos, (MouseButton)button);
						SPH_PROPAGATE(e);
					}
					break;
			}
		});

		glfwSetScrollCallback(win, [](GLFWwindow* win, double xoffset, double yoffset) {
			double x, y;
			glfwGetCursorPos(win, &x, &y);
			Camera* camera = Renderer::GetCamera();
			Point screenPos(x, y);
			Point gamePos = (camera != nullptr) ? camera->ScreenPosToGamePos(screenPos) : Point();
			MouseScrolledEvent e(screenPos, gamePos, yoffset > 0.f);
			SPH_PROPAGATE(e);
		});
	}

}
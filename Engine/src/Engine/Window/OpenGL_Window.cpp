#include "pch.h"
#include "OpenGL_Window.hpp"

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
		SetVsync(false);

		lastTime = glfwGetTime();
		lastSecond = lastTime;
	}


	OpenGL_Window::~OpenGL_Window()
	{
		glfwDestroyWindow(win);
	}


	void OpenGL_Window::PollEvents()
	{
		glfwPollEvents();
	}


	void OpenGL_Window::StartRender()
	{
		Renderer::StartFrame();
	}


	void OpenGL_Window::EndRender()
	{
		Renderer::EndFrame();
		glfwSwapBuffers(win);
	}


	float OpenGL_Window::GetDeltaTime()
	{
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		if (lastSecond + 1 < currentTime) {
			glfwSetWindowTitle(win, (props.title + " [" + std::to_string(fps) + " FPS]").c_str());
			lastSecond = currentTime;
			fps = 0;
		} else {
			++fps;
		}
		return deltaTime;
	}


	uint32_t OpenGL_Window::GetWidth()
	{
		return props.width;
	}


	uint32_t OpenGL_Window::GetHeight()
	{
		return props.height;
	}


	std::string OpenGL_Window::GetTitle()
	{
		return props.title;
	}


	bool OpenGL_Window::IsVsync()
	{
		return isVsync;
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
		glfwSwapInterval(vsync ? 1 : 0);
		isVsync = vsync;
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
				case GLFW_REPEAT:
					{
						KeyRepeatEvent e((KeyCode)keyCode);
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
	}
}
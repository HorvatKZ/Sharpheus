#include "pch.h"
#include "OpenGL_Window.hpp"

#define SPH_PROPAGATE() \
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

		success = glewInit();
		SPH_ASSERT(success == 0, "Error during GLEW initialization!");
		Renderer::Init();

		glfwSetWindowUserPointer(win, &callback);
		SetCallbacks();
		SetVsync(true);
	}


	OpenGL_Window::~OpenGL_Window()
	{
		glfwDestroyWindow(win);
		Renderer::Clear();
	}


	void OpenGL_Window::PollEvents()
	{
		glfwPollEvents();
	}


	void OpenGL_Window::StartRender()
	{
		glClearColor(0, 0, 0.2, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_TEXTURE_2D);   // textures
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glViewport(0, 0, props.width, props.height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		Renderer::StartFrame();
	}


	void OpenGL_Window::EndRender()
	{
		Renderer::EndFrame();
		glfwSwapBuffers(win);
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
			SPH_LOG("Window closed");
			WindowClosedEvent e;
			SPH_PROPAGATE();
		});

		glfwSetWindowSizeCallback(win, [](GLFWwindow* win, int newWidth, int newHeight) {
			WindowResizedEvent e(newWidth, newHeight);
			SPH_PROPAGATE();
		});
	}
}
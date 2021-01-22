#include "pch.h"
#include "OpenGL_Window.hpp"


namespace Sharpheus {

	OpenGL_Window::OpenGL_Window(const Window::Props& props) : Window(props)
	{
		glfwInit();
		win = glfwCreateWindow(props.width, props.height, props.title.c_str(), props.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
		glfwMakeContextCurrent(win);
		glfwSetWindowUserPointer(win, &callback);
		SetVsync(true);

		glfwSetWindowCloseCallback(win, [](GLFWwindow* win) {
			std::function<void()> closeCallback = *(std::function<void()>*)glfwGetWindowUserPointer(win);
			closeCallback();
		});
	}


	OpenGL_Window::~OpenGL_Window()
	{
		glfwDestroyWindow(win);
	}


	void OpenGL_Window::Tick()
	{
		glfwPollEvents();
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

	void OpenGL_Window::SetCloseCallback(std::function<void()> callback)
	{
		this->callback = callback;
	}
}
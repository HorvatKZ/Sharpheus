#pragma once

#include "Window.hpp"

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GLFW/glfw3.h>


namespace Sharpheus {

	class OpenGL_Window : public Window
	{
	public:
		OpenGL_Window(const Window::Props& props = Props());
		~OpenGL_Window();

		void PollEvents() override;
		void StartRender() override;
		void EndRender() override;

		float GetDeltaTime() override;
		uint32_t GetWidth() override;
		uint32_t GetHeight() override;
		std::string GetTitle() override;
		bool IsVsync() override;

		void SetWidth(uint32_t width) override;
		void SetHeight(uint32_t height) override;
		void SetTitle(const std::string& title) override;
		void SetVsync(bool vsync) override;

	private:
		GLFWwindow* win;
		float lastTime, lastSecond;
		uint32_t fps = 0;

		void SetCallbacks();
	};

}
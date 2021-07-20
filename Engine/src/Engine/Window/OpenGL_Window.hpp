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

		float GetTime() override;
		uint32 GetFPS() override;

		void SetWidth(uint32 width) override;
		void SetHeight(uint32 height) override;
		void SetTitle(const std::string& title) override;
		void SetVsync(bool vsync) override;
		void SetFullscreen(bool fullscreen) override;
		void SetBackground(const Color& backgroun) override;

	private:
		GLFWwindow* win;
		float startTime, lastSecond;
		uint32 fps = 0, lastFps = 0;
		int lastPos[2], lastSize[2];
		std::unordered_set<KeyCode> pressedKeys;

		void HandleInnerKeyPress(const KeyPressedEvent& e);
		void HandleInnerKeyRelease(const KeyReleasedEvent& e);
		void HandleFPS();
		void NotifyKeyHold();

		void SetCallbacks();
	};

}
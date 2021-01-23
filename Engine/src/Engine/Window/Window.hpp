#pragma once

#include "pch.h"
#include "Engine/Renderer/Renderer.hpp"


namespace Sharpheus {

	class Window
	{
	public:
		struct Props {
			std::string title = "Sharpheus Test";
			uint32_t width = 1280;
			uint32_t height = 720;
			bool fullscreen = false;
		};

		Window(const Props& props = Props()) : props(props) {}
		virtual ~Window() = default;

		virtual void PollEvents() = 0;
		virtual void StartRender() = 0;
		virtual void EndRender() = 0;

		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetHeight() = 0;
		virtual std::string GetTitle() = 0;
		virtual bool IsVsync() = 0;

		virtual void SetWidth(uint32_t width) = 0;
		virtual void SetHeight(uint32_t height) = 0;
		virtual void SetTitle(const std::string& title) = 0;
		virtual void SetVsync(bool vsync) = 0;
		virtual void SetCloseCallback(std::function<void()> callback) = 0;

	protected:
		bool isVsync = true;
		Props props;
		std::function<void()> callback;
	};

}
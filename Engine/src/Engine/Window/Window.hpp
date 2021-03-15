#pragma once

#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Events/EventHandler.hpp"


namespace Sharpheus {

	class SPH_EXPORT Window
	{
	public:
		struct Props {
			Props(const std::string& title = "Sharpheus Test", uint32_t width = 1280, uint32_t height = 720, bool fullscreen = false,
				bool vsync = false, Color background = Color(32, 64, 128))
				: title(title), width(width), height(height), fullscreen(fullscreen), vsync(vsync), background(background) {}

			std::string title;
			uint32_t width;
			uint32_t height;
			bool fullscreen;
			bool vsync;
			Color background;
		};


		Window(const Props& props = Props()) : props(props), callback(SPH_BIND_STATIC(EventHandler::Handle)) {}
		virtual ~Window() = default;

		virtual void PollEvents() = 0;
		virtual void StartRender() = 0;
		virtual void EndRender() = 0;

		virtual float GetDeltaTime() = 0;

		inline const Props&			GetProps() { return props; }
		inline uint32_t				GetWidth() { return props.width; }
		inline uint32_t				GetHeight() { return props.height; }
		inline const std::string&	GetTitle() { return props.title; }
		inline bool					IsVsync() { return props.vsync; }
		inline bool					IsFullscreen() { return props.fullscreen; }
		inline const Color&			GetBackground() { return props.background; }

		inline void SetProps(const Props& props) {
			SetWidth(props.width);
			SetHeight(props.height);
			SetTitle(props.title);
			SetVsync(props.vsync);
			SetFullscreen(props.fullscreen);
			SetBackground(props.background);
		}

		virtual void SetWidth(uint32_t width) = 0;
		virtual void SetHeight(uint32_t height) = 0;
		virtual void SetTitle(const std::string& title) = 0;
		virtual void SetVsync(bool vsync) = 0;
		virtual void SetFullscreen(bool fullscreen) = 0;
		virtual void SetBackground(const Color& background) = 0;

	protected:
		Props props;
		EventFunc callback;
	};

	typedef Window::Props WinProps;
}
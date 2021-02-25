#pragma once

#include "Engine/Window/Window.hpp"


namespace Sharpheus {

	class ProjectData
	{
	public:
		static void Init();
		static void Clear();

		static inline const Window::Props& GetWinProps() { return winProps; }
		static inline void SetWinProps(const Window::Props& props) { winProps = props; Camera::SetStaticRect(props.width, props.height); }

	private:
		static Window::Props winProps;
	};

}
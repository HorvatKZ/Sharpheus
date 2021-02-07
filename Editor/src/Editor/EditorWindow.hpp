#pragma once

#include "editor_pch.h"


namespace Sharpheus {

	class EditorWindow : public wxFrame
	{
	public:
		EditorWindow(const std::string& title, uint32_t width, uint32_t height);
		virtual ~EditorWindow();

	private:
	};

}
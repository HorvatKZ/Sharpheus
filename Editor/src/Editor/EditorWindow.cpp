#include "editor_pch.h"
#include "EditorWindow.hpp"


namespace Sharpheus {

	EditorWindow::EditorWindow(const std::string& title, uint32_t width, uint32_t height)
		: wxFrame(nullptr, wxID_ANY, title, wxPoint(30, 30), wxSize(width, height))
	{

	}

	EditorWindow::~EditorWindow()
	{

	}

}
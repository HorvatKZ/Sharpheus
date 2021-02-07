#include "editor_pch.h"
#include "Editor.hpp"


wxIMPLEMENT_APP(Sharpheus::Editor);

namespace Sharpheus {

	Editor::Editor()
	{
	}

	Editor::~Editor()
	{
	}


	bool Editor::OnInit()
	{
		win = new EditorWindow("Sharpheus Editor", 1280, 720);
		win->Show();

		return true;
	}

}
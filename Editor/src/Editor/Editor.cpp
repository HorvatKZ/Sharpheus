#include "editor_pch.h"
#include "Editor.hpp"
#include "EditorData.hpp"
#include "Engine/Renderer/Renderer.hpp"


wxIMPLEMENT_APP(Sharpheus::Editor);

namespace Sharpheus {

	Editor::Editor()
	{
		Logger::Init();
		EditorData::Init();
		SPHE_INFO("Elcome to Sherpheus Editor");
	}

	Editor::~Editor()
	{
		EditorData::Clear();
		SPHE_INFO("Exiting editor. Thanks for using Sharpheus");
	}


	bool Editor::OnInit()
	{
		win = new EditorWindow("Sharpheus Editor", 1800, 1000);
		Renderer::Init();
		EditorData::SetLevel(new Level("Level"));
		win->Show();

		return true;
	}

}
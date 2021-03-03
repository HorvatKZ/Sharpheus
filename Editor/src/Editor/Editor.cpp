#include "editor_pch.h"
#include "Editor.hpp"
#include "Registry/EditorData.hpp"
#include "Registry/ProjectData.hpp"
#include "Registry/ClassRegistry.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "ResourceManagement/ImageManager.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"


wxIMPLEMENT_APP(Sharpheus::Editor);

namespace Sharpheus {

	Editor::Editor()
	{
		Logger::Init();
		EditorData::Init();
		ClassRegistry::Init();
		SPHE_INFO("Welcome to Sharpheus Editor");
	}

	Editor::~Editor()
	{
		Logger::Clear();
		EditorData::Clear();
		ProjectData::Clear();
		ImageManager::Clear();
		ResourceManager::Clear();
		SPHE_INFO("Exiting editor. Thanks for using Sharpheus");
	}


	bool Editor::OnInit()
	{
		ImageManager::Init();
		win = new EditorWindow("", 1280, 720);
		if (argc == 2) {
			ProjectData::Init(wxStr2StdStr(argv[1]));
		}
		else {
			ProjectData::InitNew("Test Project", "D:\\Programming\\Sharpheus\\TestProject\\TestProject.proj.sharpheus",
				"Level", "Level.lvl.sharpheus");
		}
		win->SetTitle("Sharpheus Editor - " + ProjectData::GetProj()->GetName());

		win->InitContent();
		win->Show();

		return true;
	}

}
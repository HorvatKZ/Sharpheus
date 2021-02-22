#include "editor_pch.h"
#include "Editor.hpp"
#include "Registry/EditorData.hpp"
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
		SPHE_INFO("Welcome to Sherpheus Editor");
	}

	Editor::~Editor()
	{
		Logger::Clear();
		EditorData::Clear();
		ImageManager::Clear();
		ResourceManager::Clear();
		SPHE_INFO("Exiting editor. Thanks for using Sharpheus");
	}


	bool Editor::OnInit()
	{
		ImageManager::Init();

		win = new EditorWindow("Sharpheus Editor", 1800, 1000);
		ResourceManager::Init();
		EditorData::SetLevel(new Level("Level"));
		win->InitContent();
		win->Show();

		return true;
	}

}
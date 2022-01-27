#include "editor_pch.h"
#include "Editor.hpp"
#include "Registry/EditorData.hpp"
#include "Registry/ProjectData.hpp"
#include "Registry/ClassRegistry.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "ResourceManagement/ImageManager.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"
#include "Engine/ResourceManager/AudioPlayer.hpp"
#include "EditorCommands.hpp"
#include <wx/stdpaths.h>
#include <wx/dir.h>


wxIMPLEMENT_APP(Sharpheus::Editor);

namespace Sharpheus {

	Editor::Editor()
	{
		Logger::Init();
		EditorData::Init();
		ClassRegistry::Init();
		AudioPlayer::Init();
		SPHE_INFO("Welcome to Sharpheus Editor");
	}

	Editor::~Editor()
	{
		Logger::Clear();
		EditorData::Clear();
		ProjectData::Clear();
		ImageManager::Clear();
		AudioPlayer::Clear();
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
#ifdef SPH_DEV
			ProjectData::InitNew("Test Project", "D:\\Programming\\Sharpheus\\TestProject\\TestProject.proj.sharpheus",
				"Level", "Level.lvl.sharpheus");
#else
			wxFileName binFolder(wxStandardPaths::Get().GetDataDir());
			binFolder.SetName("");
			wxDir dir(binFolder.GetFullPath());
			bool found;
			wxString name;
			found = dir.GetFirst(&name);
			while (found && name.Right(15) != ".proj.sharpheus") {
				found = dir.GetNext(&name);
			}

			if (name.Right(15) != ".proj.sharpheus") {
				SPHE_ERROR("Could not locate the relevant *.proj.sharpheus file");
				wxExit();
			}

			ProjectData::Init(wxStr2StdStr(binFolder.GetFullPath() + name));
#endif
		}
		win->SetTitle("Sharpheus Editor - " + ProjectData::GetProj()->GetName());
		EditorCommands::Init(SPH_BIND_WITH_0(EditorWindow::LevelChanged, win), SPH_BIND_WITH_0(EditorWindow::CurrentChanged, win));

		win->InitContent();
		win->Show();

		return true;
	}

}
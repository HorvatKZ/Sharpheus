#include "editor_pch.h"
#include "GamePreview.hpp"


namespace Sharpheus {

	GamePreview::GamePreview(wxWindow* parent, const std::string& levelFilePath, wxGLContext* glContext, const Window::Props& winProps)
		: wxFrame(parent, wxID_ANY, winProps.title, wxPoint(0, 0), wxSize(winProps.width, winProps.height))
	{
		EventHandler::Init(SPH_BIND(GamePreview::WindowClosed));

		level = new Level();
		bool success = level->Load(levelFilePath);
		SPHE_ASSERT(success, "Cannot start GamePreview, because the .lvl file is damaged");

		canvas = new PreviewCanvas(this, glContext, level);
		lastTick = wxGetLocalTimeMillis();

		if (winProps.fullscreen) {
			ShowFullScreen(true);
		}

		Bind(wxEVT_IDLE, &GamePreview::OnIdle, this);
		Bind(wxEVT_SIZE, &GamePreview::OnResize, this);
	}


	GamePreview::~GamePreview()
	{
		EventHandler::Clear();
	}


	void GamePreview::OnIdle(wxIdleEvent& e)
	{
		wxLongLong currTick = wxGetLocalTimeMillis();
		level->Tick((currTick - lastTick).ToDouble() / 1000);
		canvas->Refresh();
		lastTick = currTick;
	}


	void GamePreview::WindowClosed(const WindowClosedEvent& e)
	{
		//Destroy();
	}
	
	
	void GamePreview::OnResize(wxSizeEvent& e)
	{
		wxSize size = GetSize();
		Camera::SetStaticRect(size.x, size.y);
		e.Skip();
	}

}
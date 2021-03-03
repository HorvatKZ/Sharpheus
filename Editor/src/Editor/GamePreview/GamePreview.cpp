#include "editor_pch.h"
#include "GamePreview.hpp"


namespace Sharpheus {

	GamePreview::GamePreview(wxWindow* parent, const std::string& projectPath, wxGLContext* glContext, const Window::Props& winProps)
		: wxFrame(parent, wxID_ANY, winProps.title, wxPoint(0, 0), wxSize(winProps.width, winProps.height))
	{
		EventHandler::Init(SPH_BIND(GamePreview::WindowClosed));

		bool success = proj.Load(projectPath);
		SPHE_ASSERT(success, "Cannot start GamePreview, because the .lvl file is damaged");

		canvas = new PreviewCanvas(this, glContext, &proj);
		lastTick = wxGetLocalTimeMillis();

		if (winProps.fullscreen) {
			ShowFullScreen(true);
		}

		Bind(wxEVT_IDLE, &GamePreview::OnIdle, this);
		Bind(wxEVT_SIZE, &GamePreview::OnResize, this);
		Bind(wxEVT_CLOSE_WINDOW, &GamePreview::OnClose, this);
		canvas->Bind(wxEVT_KEY_DOWN, &GamePreview::OnKeyDown, this);
		canvas->Bind(wxEVT_KEY_UP, &GamePreview::OnKeyUp, this);
	}


	GamePreview::~GamePreview()
	{
		EventHandler::Clear();
	}


	void GamePreview::OnIdle(wxIdleEvent& e)
	{
		for (auto it = pressedKeys.begin(); it != pressedKeys.end(); ++it) {
			EventHandler::Handle(KeyRepeatEvent(*it));
		}

		wxLongLong currTick = wxGetLocalTimeMillis();
		proj.Tick((currTick - lastTick).ToDouble() / 1000);
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
		EventHandler::Handle(WindowResizedEvent(size.x, size.y));
		e.Skip();
	}


	void GamePreview::OnClose(wxCloseEvent& e)
	{
		EventHandler::Handle(WindowClosedEvent());
		e.Skip();
	}


	void GamePreview::OnKeyDown(wxKeyEvent& e)
	{
		int keyCode = e.GetKeyCode();
		pressedKeys.insert(keyCode);
		EventHandler::Handle(KeyPressedEvent(keyCode));
		e.Skip();
	}


	void GamePreview::OnKeyUp(wxKeyEvent& e)
	{
		int keyCode = e.GetKeyCode();
		pressedKeys.erase(keyCode);
		EventHandler::Handle(KeyReleasedEvent(keyCode));
		e.Skip();
	}

}
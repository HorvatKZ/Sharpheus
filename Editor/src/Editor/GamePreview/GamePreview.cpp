#include "editor_pch.h"
#include "GamePreview.hpp"


namespace Sharpheus {

	GamePreview::GamePreview(wxWindow* parent, const std::string& projectPath, wxGLContext* glContext, const Window::Props& winProps)
		: wxFrame(parent, wxID_ANY, winProps.title, wxPoint(0, 0), wxSize(winProps.width, winProps.height))
	{
		EventHandler::Init(SPH_BIND(GamePreview::WindowClosed));

		proj = new Project();
		bool success = proj->Load(projectPath);
		SPHE_ASSERT(success, "Cannot start GamePreview, because the .lvl file is damaged");

		canvas = new PreviewCanvas(this, glContext, proj);
		lastTick = wxGetLocalTimeMillis();

		if (winProps.fullscreen) {
			ShowFullScreen(true);
		}

		Bind(wxEVT_IDLE, &GamePreview::OnIdle, this);
		Bind(wxEVT_SIZE, &GamePreview::OnResize, this);
		Bind(wxEVT_CLOSE_WINDOW, &GamePreview::OnClose, this);
		canvas->Bind(wxEVT_KEY_DOWN, &GamePreview::OnKeyDown, this);
		canvas->Bind(wxEVT_KEY_UP, &GamePreview::OnKeyUp, this);
		canvas->Bind(wxEVT_LEFT_DOWN, &GamePreview::OnLeftDown, this);
		canvas->Bind(wxEVT_LEFT_UP, &GamePreview::OnLeftUp, this);
		canvas->Bind(wxEVT_RIGHT_DOWN, &GamePreview::OnRightDown, this);
		canvas->Bind(wxEVT_RIGHT_UP, &GamePreview::OnRightUp, this);
		canvas->Bind(wxEVT_MIDDLE_DOWN, &GamePreview::OnMiddleDown, this);
		canvas->Bind(wxEVT_MIDDLE_UP, &GamePreview::OnMiddleUp, this);
		canvas->Bind(wxEVT_AUX1_DOWN, &GamePreview::On4Down, this);
		canvas->Bind(wxEVT_AUX1_UP, &GamePreview::On4Up, this);
		canvas->Bind(wxEVT_AUX2_DOWN, &GamePreview::On5Down, this);
		canvas->Bind(wxEVT_AUX2_UP, &GamePreview::On5Up, this);
		canvas->Bind(wxEVT_MOTION, &GamePreview::OnMove, this);
		canvas->Bind(wxEVT_MOUSEWHEEL, &GamePreview::OnScroll, this);
	}


	GamePreview::~GamePreview()
	{
		delete proj;
		EventHandler::Clear();
	}


	void GamePreview::OnIdle(wxIdleEvent& e)
	{
		for (auto it = pressedKeys.begin(); it != pressedKeys.end(); ++it) {
			EventHandler::Handle(KeyRepeatEvent(*it));
		}

		wxLongLong currTick = wxGetLocalTimeMillis();
		proj->Tick((currTick - lastTick).ToDouble() / 1000);
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


	void GamePreview::OnLeftDown(wxMouseEvent& e)
	{
		wxPoint mousePos = wxGetMousePosition() - this->GetScreenPosition();
		Camera* camera = Renderer::GetCamera();
		Point screenPos(mousePos.x, mousePos.y);
		Point gamePos = (camera != nullptr) ? camera->ScreenPosToGamePos(screenPos) : Point();
		EventHandler::Handle(MousePressedEvent(screenPos, gamePos, MouseButton::LEFT));
		e.Skip();
	}


	void GamePreview::OnLeftUp(wxMouseEvent& e)
	{
		wxPoint mousePos = wxGetMousePosition() - this->GetScreenPosition();
		Camera* camera = Renderer::GetCamera();
		Point screenPos(mousePos.x, mousePos.y);
		Point gamePos = (camera != nullptr) ? camera->ScreenPosToGamePos(screenPos) : Point();
		EventHandler::Handle(MouseReleasedEvent(screenPos, gamePos, MouseButton::LEFT));
		e.Skip();
	}


	void GamePreview::OnRightDown(wxMouseEvent& e)
	{
		wxPoint mousePos = wxGetMousePosition() - this->GetScreenPosition();
		Camera* camera = Renderer::GetCamera();
		Point screenPos(mousePos.x, mousePos.y);
		Point gamePos = (camera != nullptr) ? camera->ScreenPosToGamePos(screenPos) : Point();
		EventHandler::Handle(MousePressedEvent(screenPos, gamePos, MouseButton::RIGHT));
		e.Skip();
	}


	void GamePreview::OnRightUp(wxMouseEvent& e)
	{
		wxPoint mousePos = wxGetMousePosition() - this->GetScreenPosition();
		Camera* camera = Renderer::GetCamera();
		Point screenPos(mousePos.x, mousePos.y);
		Point gamePos = (camera != nullptr) ? camera->ScreenPosToGamePos(screenPos) : Point();
		EventHandler::Handle(MouseReleasedEvent(screenPos, gamePos, MouseButton::RIGHT));
		e.Skip();
	}


	void GamePreview::OnMiddleDown(wxMouseEvent& e)
	{
		wxPoint mousePos = wxGetMousePosition() - this->GetScreenPosition();
		Camera* camera = Renderer::GetCamera();
		Point screenPos(mousePos.x, mousePos.y);
		Point gamePos = (camera != nullptr) ? camera->ScreenPosToGamePos(screenPos) : Point();
		EventHandler::Handle(MousePressedEvent(screenPos, gamePos, MouseButton::MIDDLE));
		e.Skip();
	}


	void GamePreview::OnMiddleUp(wxMouseEvent& e)
	{
		wxPoint mousePos = wxGetMousePosition() - this->GetScreenPosition();
		Camera* camera = Renderer::GetCamera();
		Point screenPos(mousePos.x, mousePos.y);
		Point gamePos = (camera != nullptr) ? camera->ScreenPosToGamePos(screenPos) : Point();
		EventHandler::Handle(MouseReleasedEvent(screenPos, gamePos, MouseButton::MIDDLE));
		e.Skip();
	}


	void GamePreview::On4Down(wxMouseEvent& e)
	{
		wxPoint mousePos = wxGetMousePosition() - this->GetScreenPosition();
		Camera* camera = Renderer::GetCamera();
		Point screenPos(mousePos.x, mousePos.y);
		Point gamePos = (camera != nullptr) ? camera->ScreenPosToGamePos(screenPos) : Point();
		EventHandler::Handle(MousePressedEvent(screenPos, gamePos, (MouseButton)4));
		e.Skip();
	}


	void GamePreview::On4Up(wxMouseEvent& e)
	{
		wxPoint mousePos = wxGetMousePosition() - this->GetScreenPosition();
		Camera* camera = Renderer::GetCamera();
		Point screenPos(mousePos.x, mousePos.y);
		Point gamePos = (camera != nullptr) ? camera->ScreenPosToGamePos(screenPos) : Point();
		EventHandler::Handle(MouseReleasedEvent(screenPos, gamePos, (MouseButton)4));
		e.Skip();
	}


	void GamePreview::On5Down(wxMouseEvent& e)
	{
		wxPoint mousePos = wxGetMousePosition() - this->GetScreenPosition();
		Camera* camera = Renderer::GetCamera();
		Point screenPos(mousePos.x, mousePos.y);
		Point gamePos = (camera != nullptr) ? camera->ScreenPosToGamePos(screenPos) : Point();
		EventHandler::Handle(MousePressedEvent(screenPos, gamePos, (MouseButton)5));
		e.Skip();
	}


	void GamePreview::On5Up(wxMouseEvent& e)
	{
		wxPoint mousePos = wxGetMousePosition() - this->GetScreenPosition();
		Camera* camera = Renderer::GetCamera();
		Point screenPos(mousePos.x, mousePos.y);
		Point gamePos = (camera != nullptr) ? camera->ScreenPosToGamePos(screenPos) : Point();
		EventHandler::Handle(MouseReleasedEvent(screenPos, gamePos, (MouseButton)5));
		e.Skip();
	}


	void GamePreview::OnMove(wxMouseEvent& e)
	{
		wxPoint mousePos = wxGetMousePosition() - this->GetScreenPosition();
		Camera* camera = Renderer::GetCamera();
		Point screenPos(mousePos.x, mousePos.y);
		Point gamePos = (camera != nullptr) ? camera->ScreenPosToGamePos(screenPos) : Point();
		EventHandler::Handle(MouseMovedEvent(screenPos, gamePos));
		e.Skip();
	}


	void GamePreview::OnScroll(wxMouseEvent& e)
	{
		wxPoint mousePos = wxGetMousePosition() - this->GetScreenPosition();
		Camera* camera = Renderer::GetCamera();
		Point screenPos(mousePos.x, mousePos.y);
		Point gamePos = (camera != nullptr) ? camera->ScreenPosToGamePos(screenPos) : Point();
		EventHandler::Handle(MouseScrolledEvent(screenPos, gamePos, Point(0, e.GetWheelRotation())));
		e.Skip();
	}

}
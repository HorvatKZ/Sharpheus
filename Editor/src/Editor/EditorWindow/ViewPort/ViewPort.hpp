#pragma once

#include "editor_pch.h"
#include <GL/glew.h>
#include <GL/wglew.h>
#include <wx/glcanvas.h>
#include "EditingArrow.hpp"


namespace Sharpheus {

	class ViewPort : public wxGLCanvas
	{
	public:
		ViewPort(wxFrame* parent, const wxPoint& pos, const wxSize& size);
		virtual ~ViewPort();

		void BindCallbacks(std::function<void()>&& currChangedCallback, std::function<void()>&& currDataChangedCallback);

		void OnPaintEvent(wxPaintEvent& evt);
		void PaintNow();

		inline void SetPlaying(bool value) { isPlaying = value; }
		inline void SetGrid(bool value) { isGridOn = value; }
		inline bool IsGridOn() { return isGridOn; }
		inline void SetSnapToGrid(bool value) { isSnapToGrid = value; }
		inline bool IsSnapToGrid() { return isSnapToGrid; }

		inline wxGLContext* GetContext() { return glContext; }

	private:
		Camera* camera;
		EditingArrow* editArrow;
		wxGLContext* glContext = nullptr;

		wxPoint prevMousePos;
		EditingArrow::EditType editType = EditingArrow::EditType::NONE;
		std::function<void()> currChangedCallback;
		std::function<void()> currDataChangedCallback;
		float gridSize = 64, gridY = 64, gridThickness = 2;
		Color gridColor = Color(240, 240, 240, 200);
		bool isGridOn = true;
		bool isSnapToGrid = false;
		bool isGridInForeground = false;
		bool isPlaying = false;

		void OnResize(wxSizeEvent& e);
		void OnScroll(wxMouseEvent& e);
		void OnMouseMove(wxMouseEvent& e);
		void OnMouseDown(wxMouseEvent& e);
		void OnMouseUp(wxMouseEvent& e);
		void Render(wxDC& dc);
		void RenderGrid();
	};

}
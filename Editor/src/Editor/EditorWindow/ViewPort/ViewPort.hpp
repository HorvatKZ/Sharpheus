#pragma once

#include "editor_pch.h"
#include "ViewPortBase.hpp"
#include "EditingArrow.hpp"


namespace Sharpheus {

	class ViewPort : public ViewPortBase
	{
	public:
		ViewPort(wxFrame* parent, const wxPoint& pos, const wxSize& size);
		virtual ~ViewPort();

		void BindCallbacks(std::function<void()>&& currChangedCallback, std::function<void()>&& currDataChangedCallback);
		void InitEditingArrow();

		inline void SetGrid(bool value) { isGridOn = value; }
		inline bool IsGridOn() { return isGridOn; }
		inline void SetSnapToGrid(bool value) { isSnapToGrid = value; }
		inline bool IsSnapToGrid() { return isSnapToGrid; }

	private:
		EditingArrow* editArrow;

		EditingArrow::EditType editType = EditingArrow::EditType::NONE;
		std::function<void()> currChangedCallback;
		bool isGridOn = true;
		bool isSnapToGrid = false;

		void OnMouseMove(wxMouseEvent& e) override;
		void Render(wxDC& dc) override;

		void OnMouseDown(wxMouseEvent& e);
		void OnMouseUp(wxMouseEvent& e);

		void RenderGrid();
		void RenderCameraOutline(Camera* cam);
	};

}
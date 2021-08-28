#pragma once

#include "editor_pch.h"
#include "Editor/EditorWindow/ViewPort/ViewPortBase.hpp"
#include "TileSelecor.hpp"


namespace Sharpheus::TileMapEditor {

	class ViewPort : public ViewPortBase
	{
	public:
		ViewPort(wxFrame* parent, wxGLContext* other, const wxPoint& pos, const wxSize& size, TileSelector* selector, TileMap* tileMap);
		virtual ~ViewPort();

		inline bool IsGridOn() { return isGridOn; }
		inline bool IsForegroundOn() { return isForegroundOn; }
		inline bool IsHighlightOn() { return isHighlightOn; }
		inline void SetGrid(bool value) { isGridOn = value; }
		inline void SetHighlight(bool value) { isHighlightOn = value; }
		inline void SetForeground(bool value) { isForegroundOn = value; }

	private:
		bool isGridOn = true;
		bool isForegroundOn = true;
		bool isHighlightOn = false;
		TileSelector* selector;
		TileMap* tileMap;

		void OnMouseMove(wxMouseEvent& e) override;
		void Render(wxDC& dc) override;

		void OnLeftDown(wxMouseEvent& e);
		void OnRightDown(wxMouseEvent& e);

		void RenderGrid();
	};

}
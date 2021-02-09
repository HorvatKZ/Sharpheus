#pragma once

#include "editor_pch.h"


namespace Sharpheus {

	class ToolBar : public wxPanel
	{
	public:
		ToolBar(wxFrame* parent, const wxPoint& pos, const wxSize& size);
		virtual ~ToolBar();
	};

}
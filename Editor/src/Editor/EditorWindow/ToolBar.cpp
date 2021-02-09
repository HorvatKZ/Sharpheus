#include "editor_pch.h"
#include "ToolBar.hpp"


namespace Sharpheus {

	ToolBar::ToolBar(wxFrame* parent, const wxPoint& pos, const wxSize& size)
		: wxPanel(parent, wxID_ANY, pos, size)
	{
	}

	ToolBar::~ToolBar()
	{
	}

}
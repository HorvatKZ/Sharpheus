#include "editor_pch.h"
#include "LevelHierarchy.hpp"


namespace Sharpheus {

	LevelHierarchy::LevelHierarchy(wxFrame* parent, const wxPoint& pos, const wxSize& size)
		: wxTreeCtrl(parent, wxID_ANY, pos, size, wxTR_TWIST_BUTTONS | wxTR_NO_LINES | wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT)
	{
	}

	LevelHierarchy::~LevelHierarchy()
	{
	}

}
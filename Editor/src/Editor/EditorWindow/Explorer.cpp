#include "editor_pch.h"
#include "Explorer.hpp"


namespace Sharpheus {

	Explorer::Explorer(wxFrame* parent, const wxPoint& pos, const wxSize& size)
		: wxListCtrl(parent, wxID_ANY, pos, size, wxLC_ICON)
	{
	}

	Explorer::~Explorer()
	{
	}

}
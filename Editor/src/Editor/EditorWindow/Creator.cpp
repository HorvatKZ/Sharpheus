#include "editor_pch.h"
#include "Creator.hpp"


namespace Sharpheus {

	Creator::Creator(wxFrame* parent, const wxPoint& pos, const wxSize& size)
		: wxPanel(parent, wxID_ANY, pos, size)
	{
	}

	Creator::~Creator()
	{
	}

}
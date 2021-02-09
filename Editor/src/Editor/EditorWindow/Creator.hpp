#pragma once

#include "editor_pch.h"


namespace Sharpheus {

	class Creator : public wxPanel
	{
	public:
		Creator(wxFrame* parent, const wxPoint& pos, const wxSize& size);
		virtual ~Creator();
	};

}
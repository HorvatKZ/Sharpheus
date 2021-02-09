#pragma once

#include "editor_pch.h"


namespace Sharpheus {

	class DetailsPanel : public wxPanel
	{
	public:
		DetailsPanel(wxFrame* parent, const wxPoint& pos, const wxSize& size);
		virtual ~DetailsPanel();
	};

}
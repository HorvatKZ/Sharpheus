#pragma once

#include <wx/listctrl.h>


namespace Sharpheus {

	class Explorer : public wxListCtrl
	{
	public:
		Explorer(wxFrame* parent, const wxPoint& pos, const wxSize& size);
		virtual ~Explorer();
	};

}
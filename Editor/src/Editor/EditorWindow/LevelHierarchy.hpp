#pragma once

#include <wx/treectrl.h>


namespace Sharpheus {

	class LevelHierarchy : public wxTreeCtrl
	{
	public:
		LevelHierarchy(wxFrame* parent, const wxPoint& pos, const wxSize& size);
		virtual ~LevelHierarchy();
	};

}
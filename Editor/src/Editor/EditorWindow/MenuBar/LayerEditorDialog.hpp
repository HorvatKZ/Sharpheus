#pragma once

#include "editor_pch.h"
#include <wx/listctrl.h>


namespace Sharpheus {

	class LayerEditorDialog : public wxDialog
	{
	public:
		LayerEditorDialog(wxWindow* parent);
		virtual ~LayerEditorDialog();

	private:
		enum class Operation {
			NONE,
			REMOVE,
			UP,
			DOWN,
			ADD,
			SETVIS
		};

		wxListView* list;
		wxButton* okButton;

		Operation lastOp = Operation::NONE;
		uint32 firstColWidth;
		bool needToRebuildList = false;

		static const uint32 imgColWidth;

		void OnListClicked(wxMouseEvent& e);
		void OnListDoubleClicked(wxMouseEvent& e);
		void OnListSelection(wxListEvent& e);
		void OnListIdle(wxIdleEvent& e);

		void Remove(uint32 ind);
		void Up(uint32 ind);
		void Down(uint32 ind);
		void Add(uint32 ind);
		void SetVis(uint32 ind);
		void Rename(uint32 ind);

		void FillList();
	};

}
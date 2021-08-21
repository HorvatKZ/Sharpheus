#include "editor_pch.h"
#include "LayerEditorDialog.hpp"
#include "Editor/Registry/ProjectData.hpp"
#include "Editor/ResourceManagement/ImageManager.hpp"
#include <wx/choicdlg.h>
#include <wx/textdlg.h>


namespace Sharpheus {

	const uint32 LayerEditorDialog::imgColWidth = 22;


	LayerEditorDialog::LayerEditorDialog(wxWindow* parent)
		: wxDialog(parent, wxID_ANY, "Layer Editor", wxPoint(), wxSize(400, 290))
	{
		wxSize parentSize = parent->GetSize();
		wxSize clientSize = GetClientSize();
		SetPosition(wxPoint((parentSize.x - clientSize.x) / 2, (parentSize.y - clientSize.y) / 2));

		list = new wxListView(this, wxID_ANY, wxPoint(UI::border, UI::border), wxSize(clientSize.x - 2 * UI::border, 200), wxLC_REPORT | wxLC_HRULES | wxLC_SINGLE_SEL);
		firstColWidth = clientSize.x - 2 * UI::border - 4 * imgColWidth;
		list->AppendColumn("Name", wxLIST_FORMAT_LEFT, firstColWidth);
		list->AppendColumn("", wxLIST_FORMAT_CENTRE, imgColWidth);
		list->AppendColumn("", wxLIST_FORMAT_CENTRE, imgColWidth);
		list->AppendColumn("", wxLIST_FORMAT_CENTRE, imgColWidth);
		list->AppendColumn("", wxLIST_FORMAT_CENTRE, imgColWidth);

		wxImageList* imgs = new wxImageList(20, 20);
		imgs->Add(ImageManager::GetImage("visible.png", ImageManager::PathType::MENUBAR));
		imgs->Add(ImageManager::GetImage("invisible.png", ImageManager::PathType::MENUBAR));
		imgs->Add(ImageManager::GetImage("remove.png", ImageManager::PathType::MENUBAR));
		imgs->Add(ImageManager::GetImage("up.png", ImageManager::PathType::MENUBAR));
		imgs->Add(ImageManager::GetImage("down.png", ImageManager::PathType::MENUBAR));
		imgs->Add(ImageManager::GetImage("add.png", ImageManager::PathType::MENUBAR));
		list->AssignImageList(imgs, wxIMAGE_LIST_SMALL);

		FillList();

		list->Bind(wxEVT_LEFT_DOWN, &LayerEditorDialog::OnListClicked, this);
		list->Bind(wxEVT_LEFT_DCLICK, &LayerEditorDialog::OnListDoubleClicked, this);
		list->Bind(wxEVT_LIST_ITEM_SELECTED, &LayerEditorDialog::OnListSelection, this);
		list->Bind(wxEVT_IDLE, &LayerEditorDialog::OnListIdle, this);

		okButton = new wxButton(this, wxID_OK, "OK", wxPoint(clientSize.x - UI::border - UI::buttonSize.x, clientSize.y - UI::border - UI::buttonSize.y), UI::buttonSize);
	}


	LayerEditorDialog::~LayerEditorDialog()
	{
	}


	void LayerEditorDialog::OnListClicked(wxMouseEvent& e)
	{
		uint32 x = wxGetMousePosition().x - list->GetScreenPosition().x;
		if (x > firstColWidth) {
			uint32 colInd = (x - firstColWidth) / imgColWidth;
			switch (colInd) {
				case 0:
					lastOp = Operation::REMOVE;
					break;
				case 1:
					lastOp = Operation::UP;
					break;
				case 2:
					lastOp = Operation::DOWN;
					break;
				case 3:
					lastOp = Operation::ADD;
					break;
			}
		} else if (x < imgColWidth) {
			lastOp = Operation::SETVIS;
		}
		e.Skip();
	}


	void LayerEditorDialog::OnListDoubleClicked(wxMouseEvent& e)
	{
		uint32 x = wxGetMousePosition().x - list->GetScreenPosition().x;
		if (x > imgColWidth && x < firstColWidth) {
			long selected = list->GetFirstSelected();
			uint32 layersNum = ProjectData::GetLevel()->GetLayerNum();
			if (selected >= 0 && (uint32)selected < layersNum) {
				Rename(layersNum - selected - 1);
			}
		}
		e.Skip();
	}


	void LayerEditorDialog::OnListSelection(wxListEvent& e)
	{
		long selected = list->GetFirstSelected();
		uint32 layersNum = ProjectData::GetLevel()->GetLayerNum();
		if (selected >= 0 && (uint32)selected < layersNum) {
			switch (lastOp) {
				case Operation::REMOVE:
					Remove(layersNum - selected - 1);
					break;
				case Operation::UP:
					Up(layersNum - selected - 1);
					break;
				case Operation::DOWN:
					Down(layersNum - selected - 1);
					break;
				case Operation::ADD:
					Add(layersNum - selected - 1);
					break;
				case Operation::SETVIS:
					SetVis(layersNum - selected - 1);
					break;
			}
			lastOp = Operation::NONE;
		}
	}


	void LayerEditorDialog::OnListIdle(wxIdleEvent& e)
	{
		if (needToRebuildList) {
			list->DeleteAllItems();
			FillList();
			needToRebuildList = false;
		}
	}


	void LayerEditorDialog::Remove(uint32 ind)
	{
		auto layers = ProjectData::GetLevel()->GetLayerNames();
		std::string layerName = layers->at(ind);
		if (Level::IsDeletableLayer(layerName)) {
			wxArrayString arr;
			for (uint32 i = 0; i < layers->size(); ++i) {
				if (i != ind) {
					arr.Add(layers->at(i));
				}
			}

			wxSingleChoiceDialog dial(this, "Move GameObjects from the deleted layer to:", "Remove Layer", arr);
			if (dial.ShowModal() == wxID_CANCEL) {
				return;
			}

			if (wxMessageBox("Are you sure you want to delete layer: \"" + layerName + "\"? This operation cannot be undone!", "Remove Layer", wxICON_WARNING | wxYES | wxNO | wxCENTRE) == wxNO) {
				return;
			}

			bool success = ProjectData::GetLevel()->RemoveLayer(layerName, wxStr2StdStr(dial.GetStringSelection()));
			SPHE_ASSERT(success, "Could not remove the selected layer");
			needToRebuildList = true;
		}
	}


	void LayerEditorDialog::Up(uint32 ind)
	{
		if (ind != ProjectData::GetLevel()->GetLayerNum() - 1) {
			ProjectData::GetLevel()->SwapLayers(ind, ind + 1);
			needToRebuildList = true;
		}
	}


	void LayerEditorDialog::Down(uint32 ind)
	{
		if (ind != 0) {
			ProjectData::GetLevel()->SwapLayers(ind, ind - 1);
			needToRebuildList = true;
		}
	}


	void LayerEditorDialog::Add(uint32 ind)
	{
		wxTextEntryDialog dial(this, "Name of the new layer", "Add Layer");
		dial.SetTextValidator(wxFILTER_EMPTY);
		if (dial.ShowModal() == wxID_CANCEL) {
			return;
		}

		bool success = ProjectData::GetLevel()->CreateLayer(wxStr2StdStr(dial.GetValue()), ind + 1);
		SPHE_ASSERT(success, "Could not create the new layer");
		needToRebuildList = true;
	}


	void LayerEditorDialog::SetVis(uint32 ind)
	{
		Level* level = ProjectData::GetLevel();
		level->SetLayerVisible(ind, !(level->IsLayerVisible(ind)));
		needToRebuildList = true;
	}


	void LayerEditorDialog::Rename(uint32 ind)
	{
		wxTextEntryDialog dial(this, "New name of the layer", "Rename Layer", ProjectData::GetLevel()->GetLayerNames()->at(ind));
		dial.SetTextValidator(wxFILTER_EMPTY);
		if (dial.ShowModal() == wxID_CANCEL) {
			return;
		}

		bool success = ProjectData::GetLevel()->RenameLayer(ind, wxStr2StdStr(dial.GetValue()));
		SPHE_ASSERT(success, "Could not rename the selected layer");
		needToRebuildList = true;
	}


	void LayerEditorDialog::FillList()
	{
		auto layers = ProjectData::GetLevel()->GetLayerNames();
		uint32 layersNum = layers->size();
		for (int32 i = layersNum - 1; i >= 0; --i) {
			long ind = list->InsertItem(layersNum - i - 1, layers->at(i), ProjectData::GetLevel()->IsLayerVisible(i) ? 0 : 1);
			if (Level::IsDeletableLayer(layers->at(i))) {
				list->SetItem(ind, 1, "", 2);
			}
			if (i != layersNum - 1) {
				list->SetItem(ind, 2, "", 3);
			}
			if (i != 0) {
				list->SetItem(ind, 3, "", 4);
			}
			list->SetItem(ind, 4, "", 5);
		}
	}

}
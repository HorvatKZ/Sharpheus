#include "editor_pch.h"
#include "TileMapEditor.hpp"
#include "Editor/Registry/EditorData.hpp"


namespace Sharpheus::TileMapEditor {

	Editor::Editor(wxWindow* parent, wxGLContext* context, TileMap* tileMap)
		: wxFrame(parent, wxID_ANY, "TileMap Editor", parent->GetPosition(), parent->GetSize()), tileMap(tileMap)
	{
		wxSize size = GetClientSize();
		selector = new TileSelector(this, wxPoint(0, size.y - 300), wxSize(size.x, 300));
		viewPort = new ViewPort(this, context, wxPoint(0, 0), wxSize(size.x - 40, size.y - 300), selector, tileMap);
		toolBar = new ToolBar(this, wxPoint(size.x - 40, 0), wxSize(40, size.y - 300), viewPort);

		selector->SetTileSet(tileMap->GetTileSet());

		sizer = new wxGridBagSizer(2, 2);
		sizer->Add(viewPort, wxGBPosition(0, 0), wxGBSpan(1, 1), wxEXPAND);
		sizer->Add(toolBar, wxGBPosition(0, 1), wxGBSpan(1, 1), wxEXPAND);
		sizer->Add(selector, wxGBPosition(1, 0), wxGBSpan(1, 2), wxEXPAND);

		sizer->AddGrowableCol(0);
		sizer->AddGrowableRow(0);

		SetSizerAndFit(sizer);

		wxIcon icon;
		icon.CopyFromBitmap(wxBitmap(EditorData::GetPath() + "Assets\\Editor\\Icons\\sharpheus_icon.png", wxBITMAP_TYPE_PNG));
		SetIcon(icon);

		tileMap->SubscribeForDestruction(0, [&](const GameObjectDestroyedEvent& e) {
			SPHE_WARN("The TileMap Editor will be closed, because the actual TileMap was deleted!");
			this->tileMap = nullptr;
			this->Destroy();
			});
	}

	Editor::~Editor()
	{
		if (tileMap != nullptr) {
			tileMap->UnSubscribeForDestruction(0);
		}
	}


	void Editor::ChangeTileSet(GameObject* obj)
	{
		if (obj->Is(GameObject::Type::TileMap) && (TileMap*)obj == tileMap) {
			TileSet* tileSet = tileMap->GetTileSet();
			if (tileSet == nullptr && !tileSet->IsValid()) {
				SPHE_ERROR("TileMap Editor: The new TileSet is not valid");
				Destroy();
			} else {
				selector->SetTileSet(tileSet);
				viewPort->Refresh();
			}
		}
	}

}
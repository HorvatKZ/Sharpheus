#pragma once
#include "editor_pch.h"
#include "TileMapViewPort.hpp"
#include "TileSelecor.hpp"
#include "TileMapToolBar.hpp"
#include <wx/gbsizer.h>


namespace Sharpheus::TileMapEditor {

    class Editor : public wxFrame
    {
    public:
        Editor(wxWindow* parent, wxGLContext* context, TileMap* tileMap);
        ~Editor();

        void ChangeTileSet(GameObject* obj);

    private:
        wxGridBagSizer* sizer;
        ViewPort* viewPort;
        TileSelector* selector;
        ToolBar* toolBar;

        TileMap* tileMap;
    };

}
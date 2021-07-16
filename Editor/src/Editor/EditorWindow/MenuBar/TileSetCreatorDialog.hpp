#pragma once

#include "AtlasDialog.hpp"

namespace Sharpheus {

	class TileSetCreatorDialog : public AtlasDialog
	{
	public:
		TileSetCreatorDialog(wxWindow* parent, const wxString& defaultTileSetPath = "");
		virtual ~TileSetCreatorDialog();

	private:
		wxStaticText* previewLabel;
		wxStaticText* currTileLabel;
		wxTextCtrl* currTileInput;
		wxButton* decButton, * incButton;
		wxStaticBitmap* tilePreview;
		wxStaticText* manyTileWarning;

		uint32_t currTileInd = 0;

		void BuildTileSetDialog();

		void OnBrowse(wxCommandEvent& e);
		void OnBrowseImg(wxCommandEvent& e) override;
		void OnSlicingWHChanged(wxCommandEvent& e) override;
		void OnSlicingCRChanged(wxCommandEvent& e) override;
		void OnCurrTileChanged(wxCommandEvent& e);
		void OnCurrTileDec(wxCommandEvent& e);
		void OnCurrTileInc(wxCommandEvent& e);

		void FillWithTileSet(const wxString& tileSetPath);
		void FillPreview();
		void SetDefaults() override;
	};

}
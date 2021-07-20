#include "editor_pch.h"
#include "TileSetCreatorDialog.hpp"
#include "Editor/FileUtils/RelativeFileDialog.hpp"
#include "Editor/Registry/ProjectData.hpp"
#include "Editor/ResourceManagement/ImageManager.hpp"
#include <wx/valnum.h>


namespace Sharpheus {

	TileSetCreatorDialog::TileSetCreatorDialog(wxWindow* parent, const wxString& defaultTileSetPath)
		: AtlasDialog(parent, "TileSet Creator")
	{
		BuildTileSetDialog();

		if (!defaultTileSetPath.empty() && wxFileExists(ProjectData::GetPath() + "Assets\\" + defaultTileSetPath)) {
			if (defaultTileSetPath.Length() > 15 && defaultTileSetPath.Right(15) == ".tile.sharpheus") {
				FillWithTileSet(defaultTileSetPath);
			}
		}
	}


	TileSetCreatorDialog::~TileSetCreatorDialog()
	{
	}


	void TileSetCreatorDialog::BuildTileSetDialog()
	{
		browse->Bind(wxEVT_BUTTON, &TileSetCreatorDialog::OnBrowse, this);
		browseImg->Bind(wxEVT_BUTTON, &TileSetCreatorDialog::OnBrowseImg, this);
		widthInput->Bind(wxEVT_TEXT_ENTER, &TileSetCreatorDialog::OnSlicingWHChanged, this);
		heightInput->Bind(wxEVT_TEXT_ENTER, &TileSetCreatorDialog::OnSlicingWHChanged, this);
		colInput->Bind(wxEVT_TEXT_ENTER, &TileSetCreatorDialog::OnSlicingCRChanged, this);
		rowInput->Bind(wxEVT_TEXT_ENTER, &TileSetCreatorDialog::OnSlicingCRChanged, this);

		uint32 firstColWidth = 2 * UI::border + nameLabelWidth + nameInputWidth + UI::buttonSize.x;
		uint32 secondColStartX = 4 * UI::border + firstColWidth;
		uint32 y = 7 * UI::border + 5 * UI::unitHeight + UI::heightPadding;

		previewLabel = new wxStaticText(this, wxID_ANY, "Preview", wxPoint(secondColStartX, y));
		previewLabel->SetFont(UI::titleFont);

		y += UI::border + UI::unitHeight - UI::shift;
		currTileLabel = new wxStaticText(this, wxID_ANY, "Current tile", wxPoint(secondColStartX, y + UI::shift), wxSize(labelWidth, UI::unitHeight));
		currTileInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(secondColStartX + UI::border + labelWidth, y), wxSize(inputWidth, UI::unitHeight), wxTE_PROCESS_ENTER);
		currTileInput->SetValidator(wxIntegerValidator<uint32>());
		currTileInput->Bind(wxEVT_TEXT_ENTER, &TileSetCreatorDialog::OnCurrTileChanged, this);

		y += UI::border + UI::unitHeight;
		uint32 incDecWidth = (labelWidth + inputWidth) / 2;
		decButton = new wxButton(this, wxID_ANY, "-", wxPoint(secondColStartX, y), wxSize(incDecWidth, UI::unitHeight));
		decButton->Bind(wxEVT_BUTTON, &TileSetCreatorDialog::OnCurrTileDec, this);
		incButton = new wxButton(this, wxID_ANY, "+", wxPoint(incDecWidth + UI::border + secondColStartX, y), wxSize(incDecWidth, UI::unitHeight));
		incButton->Bind(wxEVT_BUTTON, &TileSetCreatorDialog::OnCurrTileInc, this);

		y += UI::border + UI::unitHeight;
		tilePreview = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap, wxPoint(secondColStartX, y), wxSize(labelWidth + inputWidth + UI::border, labelWidth + inputWidth + UI::border));

		y += 2 * UI::border + labelWidth + inputWidth;
		manyTileWarning = new wxStaticText(this, wxID_ANY, "Frame number exceeds 255!\nOnly 0-254 frames can be\nused in the tileset", wxPoint(secondColStartX, y));
		manyTileWarning->Hide();
	}


	void TileSetCreatorDialog::OnBrowse(wxCommandEvent& e)
	{
		RelativeSaveDialog browseDialog(this, "Save TileSet", ProjectData::GetPath() + "Assets\\",
			"Sharpheus tileset files(*.tile.sharpheus) | *.tile.sharpheus");

		if (!browseDialog.Show())
			return;

		path = browseDialog.GetPath();
		pathText->SetLabel(path);
		if (wxFileExists(ProjectData::GetPath() + "Assets\\" + path)) {
			FillWithTileSet(path);
		}
		else {
			if (!imgPath.empty()) {
				createButton->Enable(true);
				createButton->SetLabel("Create");
			}
		}
	}


	void TileSetCreatorDialog::OnBrowseImg(wxCommandEvent& e)
	{
		AtlasDialog::OnBrowseImg(e);

		if (img.IsOk()) {
			currTileInd = 0;
			FillPreview();

			if (!path.empty()) {
				createButton->Enable(true);
			}
		}
	}


	void TileSetCreatorDialog::OnSlicingWHChanged(wxCommandEvent& e)
	{
		AtlasDialog::OnSlicingWHChanged(e);

		if (currTileInd >= numOfFrames) {
			currTileInd = numOfFrames - 1;
		}

		FillPreview();
	}


	void TileSetCreatorDialog::OnSlicingCRChanged(wxCommandEvent& e)
	{
		AtlasDialog::OnSlicingCRChanged(e);

		if (currTileInd >= numOfFrames) {
			currTileInd = numOfFrames - 1;
		}

		FillPreview();
	}


	void TileSetCreatorDialog::OnCurrTileChanged(wxCommandEvent& e)
	{
		unsigned long newInd = 0;
		currTileInput->GetValue().ToULong(&newInd);
		currTileInd = newInd;

		FillPreview();
	}


	void TileSetCreatorDialog::OnCurrTileDec(wxCommandEvent& e)
	{
		uint32 limit = (numOfFrames > 255) ? 255 : numOfFrames;
		if (currTileInd > 0) {
			--currTileInd;
		} else {
			currTileInd = limit - 1;
		}

		FillPreview();
	}


	void TileSetCreatorDialog::OnCurrTileInc(wxCommandEvent& e)
	{
		uint32 limit = (numOfFrames > 255) ? 255 : numOfFrames;
		currTileInd = (currTileInd + 1) % limit;
		FillPreview();
	}


	void TileSetCreatorDialog::FillWithTileSet(const wxString& tileSetPath)
	{
		TileSet* tileSet = new TileSet(wxStr2StdStr(tileSetPath));

		if (tileSet->GetAtlas() == nullptr || !SetAtlas(tileSet->GetAtlas()->GetPath())) {
			SetDefaults();
			return;
		}

		path = tileSetPath;
		pathText->SetLabel(path);
		nameInput->SetValue(tileSet->GetName());
		filterCheckBox->SetValue(tileSet->GetAtlas()->IsFiltered());
		frameWidth = tileSet->GetFrameWidth();
		frameHeight = tileSet->GetFrameHeight();
		frameCols = tileSet->GetFrameCols();
		frameRows = tileSet->GetFrameRows();
		numOfFrames = frameCols * frameRows;
		currTileInd = 0;

		FillSlicing();
		FillPreview();
		createButton->Enable(true);
		createButton->SetLabel("Save");
		delete tileSet;
	}


	void TileSetCreatorDialog::FillPreview()
	{
		currTileInput->SetValue(wxString::Format("%d", currTileInd));
		uint32 limit = (numOfFrames > 255) ? 255 : numOfFrames;
		((wxIntegerValidator<uint32>*)currTileInput->GetValidator())->SetRange(0, limit - 1);

		if (numOfFrames > 255) {
			manyTileWarning->Show();
		} else {
			manyTileWarning->Hide();
		}

		wxImage bitmap = img;
		uint32 currCol = currTileInd % frameCols, currRow = currTileInd / frameCols;
		bitmap.Resize(bitmap.GetSize(), wxPoint(-1 * frameWidth * currCol, -1 * frameHeight * currRow));
		bitmap.Resize(wxSize(frameWidth, frameHeight), wxPoint());
		wxSize tilePrevSize = tilePreview->GetSize();
		wxSize newBitmapSize;
		if (frameWidth > frameHeight) {
			newBitmapSize = wxSize(tilePrevSize.x, frameHeight * tilePrevSize.y / frameWidth);
		}
		else {
			newBitmapSize = wxSize(frameWidth * tilePrevSize.x / frameHeight, tilePrevSize.y);
		}
		bitmap.Rescale(newBitmapSize.x, newBitmapSize.y);
		tilePreview->SetBitmap(bitmap);
	}


	void TileSetCreatorDialog::SetDefaults()
	{
		AtlasDialog::SetDefaults();
		
		currTileInd = 0;
		currTileInput->SetValue("0");
		tilePreview->SetBitmap(wxNullBitmap);
	}

}
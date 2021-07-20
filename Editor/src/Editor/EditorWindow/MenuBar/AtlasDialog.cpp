#include "editor_pch.h"
#include "AtlasDialog.hpp"
#include "Editor/FileUtils/RelativeFileDialog.hpp"
#include "Editor/Registry/ProjectData.hpp"
#include "Editor/ResourceManagement/ImageManager.hpp"
#include <wx/valnum.h>



namespace Sharpheus {

	const uint32 AtlasDialog::labelWidth = 90;
	const uint32 AtlasDialog::inputWidth = 60;
	const uint32 AtlasDialog::nameLabelWidth = 60;
	const uint32 AtlasDialog::nameInputWidth = 200;

	AtlasDialog::AtlasDialog(wxWindow* parent, const wxString& title)
		: wxDialog(parent, wxID_ANY, title, wxPoint(0, 0), wxSize(560, 550))
	{
		wxSize parentSize = parent->GetSize();
		wxSize size = GetSize();
		SetPosition(wxPoint((parentSize.x - size.x) / 2, (parentSize.y - size.y) / 2));

		BuildDialog();
	}


	AtlasDialog::~AtlasDialog()
	{
	}


	void AtlasDialog::EndModal(int retCode)
	{
		name = nameInput->GetValue();
		useFilter = filterCheckBox->GetValue();

		wxDialog::EndModal(retCode);
	}


	void AtlasDialog::BuildDialog()
	{
		uint32 y = UI::border;
		nameLabel = new wxStaticText(this, wxID_ANY, "Name", wxPoint(UI::border, y + UI::shift), wxSize(nameLabelWidth, UI::unitHeight));
		nameLabel->SetFont(UI::titleFont);
		nameInput = new wxTextCtrl(this, wxID_ANY, "Unnamed", wxPoint(2 * UI::border + 60, y), wxSize(nameInputWidth, UI::unitHeight));

		y += UI::unitHeight + UI::shift;
		pathLabel = new wxStaticText(this, wxID_ANY, "Path", wxPoint(UI::border, y + UI::shift), wxSize(nameLabelWidth, UI::unitHeight));
		pathLabel->SetFont(UI::titleFont);
		pathText = new wxStaticText(this, wxID_ANY, "", wxPoint(2 * UI::border + 60, y + UI::shift), wxSize(nameInputWidth, UI::unitHeight), wxST_ELLIPSIZE_START);
		pathText->SetMaxSize(wxSize(nameInputWidth, UI::unitHeight));
		browse = new wxButton(this, wxID_ANY, "Browse...", wxPoint(3 * UI::border + nameLabelWidth + nameInputWidth, y), UI::buttonSize);

		y += UI::unitHeight + UI::shift;
		imgLabel = new wxStaticText(this, wxID_ANY, "Atlas", wxPoint(UI::border, y + UI::shift), wxSize(nameLabelWidth, UI::unitHeight));
		imgLabel->SetFont(UI::titleFont);
		imgPathText = new wxStaticText(this, wxID_ANY, "None", wxPoint(2 * UI::border + nameLabelWidth, y + UI::shift), wxSize(nameInputWidth, UI::unitHeight), wxST_ELLIPSIZE_START);
		imgPathText->SetMaxSize(wxSize(nameInputWidth, UI::unitHeight));
		browseImg = new wxButton(this, wxID_ANY, "Browse...", wxPoint(3 * UI::border + nameLabelWidth + nameInputWidth, y), UI::buttonSize);

		y += UI::unitHeight + UI::shift;
		uint32 firstColWidth = 2 * UI::border + nameLabelWidth + nameInputWidth + UI::buttonSize.x;
		imgPreview = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap, wxPoint(UI::border, 2 * UI::border + y), wxSize(firstColWidth, firstColWidth));

		y += 3 * UI::border + firstColWidth;
		filterLabel = new wxStaticText(this, wxID_ANY, "Use filtering", wxPoint(UI::border, y + UI::shift));
		uint32 filterLabelWidth = filterLabel->GetTextExtent("Use filtering").x;
		filterCheckBox = new wxCheckBox(this, wxID_ANY, "", wxPoint(2 * UI::border + filterLabelWidth, y), wxSize(UI::unitHeight, UI::unitHeight));

		uint32 secondColStartX = 4 * UI::border + firstColWidth;
		y = UI::border;
		slicingLabel = new wxStaticText(this, wxID_ANY, "Slicing", wxPoint(secondColStartX, y + UI::shift));
		slicingLabel->SetFont(UI::titleFont);

		y += UI::border + UI::unitHeight;
		widthLabel = new wxStaticText(this, wxID_ANY, "Frame width", wxPoint(secondColStartX, y + UI::shift), wxSize(labelWidth, UI::unitHeight));
		widthInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(secondColStartX + UI::border + labelWidth, y), wxSize(inputWidth, UI::unitHeight), wxTE_PROCESS_ENTER);
		widthInput->SetValidator(wxIntegerValidator<uint32>());

		y += UI::border + UI::unitHeight;
		heightLabel = new wxStaticText(this, wxID_ANY, "Frame height", wxPoint(secondColStartX, y + UI::shift), wxSize(labelWidth, UI::unitHeight));
		heightInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(secondColStartX + UI::border + labelWidth, y), wxSize(inputWidth, UI::unitHeight), wxTE_PROCESS_ENTER);
		heightInput->SetValidator(wxIntegerValidator<uint32>());

		y += UI::border + UI::unitHeight;
		colLabel = new wxStaticText(this, wxID_ANY, "Columns", wxPoint(secondColStartX, y + UI::shift), wxSize(labelWidth, UI::unitHeight));
		colInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(secondColStartX + UI::border + labelWidth, y), wxSize(inputWidth, UI::unitHeight), wxTE_PROCESS_ENTER);
		colInput->SetValidator(wxIntegerValidator<uint32>());

		y += UI::border + UI::unitHeight;
		rowLabel = new wxStaticText(this, wxID_ANY, "Rows", wxPoint(secondColStartX, y + UI::shift), wxSize(labelWidth, UI::unitHeight));
		rowInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(secondColStartX + UI::border + labelWidth, y), wxSize(inputWidth, UI::unitHeight), wxTE_PROCESS_ENTER);
		rowInput->SetValidator(wxIntegerValidator<uint32>());

		y += UI::border + UI::unitHeight;
		numOfFramesLabel = new wxStaticText(this, wxID_ANY, "Frames:", wxPoint(secondColStartX, y + UI::shift), wxSize(labelWidth, UI::unitHeight));
		numOfFramesText = new wxStaticText(this, wxID_ANY, "0", wxPoint(secondColStartX + UI::border + labelWidth, y + UI::shift), wxSize(inputWidth, UI::unitHeight));

		wxSize clientSize = GetClientSize();
		info = new wxStaticText(this, wxID_ANY, "Please browse for an image", wxPoint(UI::border, clientSize.y - UI::border - 20), wxSize(200, UI::unitHeight));
		createButton = new wxButton(this, wxID_OK, "Create", wxPoint(clientSize.x - 2 * UI::border - 2 * UI::buttonSize.x, clientSize.y - UI::border - UI::buttonSize.y), UI::buttonSize);
		cancelButton = new wxButton(this, wxID_CANCEL, "Cancel", wxPoint(clientSize.x - UI::border - UI::buttonSize.x, clientSize.y - UI::border - UI::buttonSize.y), UI::buttonSize);
		createButton->Enable(false);
	}



	void AtlasDialog::OnBrowseImg(wxCommandEvent& e)
	{
		RelativeOpenDialog browseDialog(this, "Browse for image", ProjectData::GetPath() + "Assets\\",
			"Image files(*.png, *.jpg, *.jpeg, *.bmp; *.gif) | *.png; *.jpg; *.jpeg; *.bmp; *.gif");

		if (!browseDialog.Show())
			return;

		wxString newImagePath = browseDialog.GetPath();

		if (newImagePath.empty() || !SetAtlas(newImagePath)) {
			SetDefaults();
			return;
		}

		wxSize imgSize = img.GetSize();
		frameWidth = imgSize.x;
		frameHeight = imgSize.y;
		frameCols = 1;
		frameRows = 1;
		numOfFrames = 1;

		FillSlicing();
	}


	void AtlasDialog::OnSlicingWHChanged(wxCommandEvent& e)
	{
		unsigned long w = 0, h = 0;
		widthInput->GetValue().ToULong(&w);
		heightInput->GetValue().ToULong(&h);
		frameWidth = w;
		frameHeight = h;

		wxSize imgSize = img.GetSize();
		frameCols = imgSize.x / frameWidth;
		frameRows = imgSize.y / frameHeight;
		numOfFrames = frameCols * frameRows;
		colInput->SetValue(wxString::Format("%d", frameCols));
		rowInput->SetValue(wxString::Format("%d", frameRows));
		numOfFramesText->SetLabel(wxString::Format("%d", numOfFrames));
	}


	void AtlasDialog::OnSlicingCRChanged(wxCommandEvent& e)
	{
		unsigned long col = 0, row = 0;
		colInput->GetValue().ToULong(&col);
		rowInput->GetValue().ToULong(&row);
		frameCols = col;
		frameRows = row;

		wxSize imgSize = img.GetSize();
		frameWidth = imgSize.x / frameCols;
		frameHeight = imgSize.y / frameRows;
		numOfFrames = frameCols * frameRows;
		widthInput->SetValue(wxString::Format("%d", frameWidth));
		heightInput->SetValue(wxString::Format("%d", frameHeight));
		numOfFramesText->SetLabel(wxString::Format("%d", numOfFrames));
	}


	bool AtlasDialog::SetAtlas(const wxString& path)
	{
		img = ImageManager::GetImage(ProjectData::GetPath() + "Assets\\" + path);
		if (!img.IsOk()) {
			SPHE_ERROR("The selected image is corrupted!")
			return false;
		}

		imgPath = path;
		wxImage bitmap = img;
		wxSize bitmapSize = bitmap.GetSize();
		wxSize newBitmapSize;
		wxSize imgPrevSize = imgPreview->GetSize();
		if (bitmapSize.x > bitmapSize.y) {
			newBitmapSize = wxSize(imgPrevSize.x, bitmapSize.y * imgPrevSize.y / bitmapSize.x);
		}
		else {
			newBitmapSize = wxSize(bitmapSize.x * imgPrevSize.x / bitmapSize.y, imgPrevSize.y);
		}
		bitmap.Rescale(newBitmapSize.x, newBitmapSize.y);
		imgPathText->SetLabel(imgPath);
		imgPreview->SetBitmap(bitmap);
		info->SetLabel(wxString::Format("Width: %d px, Height: %d px", bitmapSize.x, bitmapSize.y));
		return true;
	}


	void AtlasDialog::SetDefaults()
	{
		createButton->Enable(false);
		imgPath = "";
		imgPathText->SetLabel("None");
		imgPreview->SetBitmap(wxNullBitmap);
		filterCheckBox->SetValue(false);
		frameWidth = 0;
		frameHeight = 0;
		frameCols = 0;
		frameRows = 0;
		numOfFrames = 0;

		FillSlicing();
	}


	void AtlasDialog::FillSlicing()
	{
		widthInput->SetValue(wxString::Format("%d", frameWidth));
		heightInput->SetValue(wxString::Format("%d", frameHeight));
		colInput->SetValue(wxString::Format("%d", frameCols));
		rowInput->SetValue(wxString::Format("%d", frameRows));
		numOfFramesText->SetLabel(wxString::Format("%d", numOfFrames));

		if (img.IsOk()) {
			wxSize imgSize = img.GetSize();
			((wxIntegerValidator<uint32>*)widthInput->GetValidator())->SetRange(1, imgSize.x);
			((wxIntegerValidator<uint32>*)heightInput->GetValidator())->SetRange(1, imgSize.y);
			((wxIntegerValidator<uint32>*)colInput->GetValidator())->SetRange(1, imgSize.x);
			((wxIntegerValidator<uint32>*)rowInput->GetValidator())->SetRange(1, imgSize.y);
		}
	}

}
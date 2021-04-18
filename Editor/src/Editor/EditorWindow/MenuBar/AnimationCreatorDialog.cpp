#include "editor_pch.h"
#include "AnimationCreatorDialog.hpp"
#include "Editor/FileUtils/RelativeFileDialog.hpp"
#include "Editor/Registry/ProjectData.hpp"
#include "Editor/ResourceManagement/ImageManager.hpp"
#include <wx/valnum.h>

#define TIMER_ID 12345

// Event table is used, because wxTimer does not support modern binding
wxBEGIN_EVENT_TABLE(Sharpheus::AnimationCreatorDialog, wxDialog)
EVT_TIMER(TIMER_ID, OnTimerTick)
wxEND_EVENT_TABLE()


namespace Sharpheus {

	uint32_t AnimationCreatorDialog::labelWidth = 90;
	uint32_t AnimationCreatorDialog::inputWidth = 60;
	uint32_t AnimationCreatorDialog::nameLabelWidth = 60;
	uint32_t AnimationCreatorDialog::nameInputWidth = 200;

	AnimationCreatorDialog::AnimationCreatorDialog(wxWindow* parent, const wxString& defaultAnimPath)
		: wxDialog(parent, wxID_ANY, "Animation Creator", wxPoint(0, 0), wxSize(560, 520)), timer(this, TIMER_ID)
	{
		wxSize parentSize = parent->GetSize();
		wxSize size = GetSize();
		SetPosition(wxPoint((parentSize.x - size.x) / 2, (parentSize.y - size.y) / 2));

		BuildDialog();

		if (!defaultAnimPath.empty() && wxFileExists(ProjectData::GetPath() + "Assets\\" + defaultAnimPath)) {
			if (defaultAnimPath.Length() > 15 && defaultAnimPath.Right(15) == ".anim.sharpheus") {
				FillWithAnim(defaultAnimPath);
			}
		}
	}


	AnimationCreatorDialog::~AnimationCreatorDialog()
	{
	}


	void AnimationCreatorDialog::EndModal(int retCode)
	{
		name = nameInput->GetValue();

		wxDialog::EndModal(retCode);
	}


	void AnimationCreatorDialog::BuildDialog()
	{
		uint32_t y = UI::border;
		nameLabel = new wxStaticText(this, wxID_ANY, "Name", wxPoint(UI::border, y + UI::shift), wxSize(nameLabelWidth, UI::unitHeight));
		nameLabel->SetFont(UI::titleFont);
		nameInput = new wxTextCtrl(this, wxID_ANY, "Unnamed", wxPoint(2 * UI::border + 60, y), wxSize(nameInputWidth, UI::unitHeight));

		y += UI::unitHeight + UI::shift;
		pathLabel = new wxStaticText(this, wxID_ANY, "Path", wxPoint(UI::border, y + UI::shift), wxSize(nameLabelWidth, UI::unitHeight));
		pathLabel->SetFont(UI::titleFont);
		pathText = new wxStaticText(this, wxID_ANY, "", wxPoint(2 * UI::border + 60, y + UI::shift), wxSize(nameInputWidth, UI::unitHeight), wxST_ELLIPSIZE_START);
		pathText->SetMaxSize(wxSize(nameInputWidth, UI::unitHeight));
		browse = new wxButton(this, wxID_ANY, "Browse...", wxPoint(3 * UI::border + nameLabelWidth + nameInputWidth, y), UI::buttonSize);
		browse->Bind(wxEVT_BUTTON, &AnimationCreatorDialog::OnBrowse, this);

		y += UI::unitHeight + UI::shift;
		imgLabel = new wxStaticText(this, wxID_ANY, "Atlas", wxPoint(UI::border, y + UI::shift), wxSize(nameLabelWidth, UI::unitHeight));
		imgLabel->SetFont(UI::titleFont);
		imgPathText = new wxStaticText(this, wxID_ANY, "None", wxPoint(2 * UI::border + nameLabelWidth, y + UI::shift), wxSize(nameInputWidth, UI::unitHeight), wxST_ELLIPSIZE_START);
		imgPathText->SetMaxSize(wxSize(nameInputWidth, UI::unitHeight));
		browseImg = new wxButton(this, wxID_ANY, "Browse...", wxPoint(3 * UI::border + nameLabelWidth + nameInputWidth, y), UI::buttonSize);
		browseImg->Bind(wxEVT_BUTTON, &AnimationCreatorDialog::OnBrowseImg, this);

		y += UI::unitHeight + UI::shift;
		uint32_t firstColWidth = 2 * UI::border + nameLabelWidth + nameInputWidth + UI::buttonSize.x;
		imgPreview = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap, wxPoint(UI::border, 2 * UI::border + y), wxSize(firstColWidth, firstColWidth));

		uint32_t secondColStartX = 4 * UI::border + firstColWidth;
		y = UI::border;
		slicingLabel = new wxStaticText(this, wxID_ANY, "Slicing", wxPoint(secondColStartX, y + UI::shift));
		slicingLabel->SetFont(UI::titleFont);

		y += UI::border + UI::unitHeight;
		widthLabel = new wxStaticText(this, wxID_ANY, "Frame width", wxPoint(secondColStartX, y + UI::shift), wxSize(labelWidth, UI::unitHeight));
		widthInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(secondColStartX + UI::border + labelWidth, y), wxSize(inputWidth, UI::unitHeight), wxTE_PROCESS_ENTER);
		widthInput->SetValidator(wxIntegerValidator<uint32_t>());
		widthInput->Bind(wxEVT_TEXT_ENTER, &AnimationCreatorDialog::OnSlicingWHChanged, this);

		y += UI::border + UI::unitHeight;
		heightLabel = new wxStaticText(this, wxID_ANY, "Frame height", wxPoint(secondColStartX, y + UI::shift), wxSize(labelWidth, UI::unitHeight));
		heightInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(secondColStartX + UI::border + labelWidth, y), wxSize(inputWidth, UI::unitHeight), wxTE_PROCESS_ENTER);
		heightInput->SetValidator(wxIntegerValidator<uint32_t>());
		heightInput->Bind(wxEVT_TEXT_ENTER, &AnimationCreatorDialog::OnSlicingWHChanged, this);

		y += UI::border + UI::unitHeight;
		colLabel = new wxStaticText(this, wxID_ANY, "Columns", wxPoint(secondColStartX, y + UI::shift), wxSize(labelWidth, UI::unitHeight));
		colInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(secondColStartX + UI::border + labelWidth, y), wxSize(inputWidth, UI::unitHeight), wxTE_PROCESS_ENTER);
		colInput->SetValidator(wxIntegerValidator<uint32_t>());
		colInput->Bind(wxEVT_TEXT_ENTER, &AnimationCreatorDialog::OnSlicingCRChanged, this);

		y += UI::border + UI::unitHeight;
		rowLabel = new wxStaticText(this, wxID_ANY, "Rows", wxPoint(secondColStartX, y + UI::shift), wxSize(labelWidth, UI::unitHeight));
		rowInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(secondColStartX + UI::border + labelWidth, y), wxSize(inputWidth, UI::unitHeight), wxTE_PROCESS_ENTER);
		rowInput->SetValidator(wxIntegerValidator<uint32_t>());
		rowInput->Bind(wxEVT_TEXT_ENTER, &AnimationCreatorDialog::OnSlicingCRChanged, this);

		y += UI::border + UI::unitHeight;
		numOfFramesLabel = new wxStaticText(this, wxID_ANY, "Frames:", wxPoint(secondColStartX, y + UI::shift), wxSize(labelWidth, UI::unitHeight));
		numOfFramesText = new wxStaticText(this, wxID_ANY, "0", wxPoint(secondColStartX + UI::border + labelWidth, y + UI::shift), wxSize(inputWidth, UI::unitHeight));

		y += UI::border + UI::heightPadding;
		timingLabel = new wxStaticText(this, wxID_ANY, "Timing", wxPoint(secondColStartX, y));
		timingLabel->SetFont(UI::titleFont);

		y += UI::border + UI::unitHeight - UI::shift;
		firstFrameLabel = new wxStaticText(this, wxID_ANY, "First frame", wxPoint(secondColStartX, y + UI::shift), wxSize(labelWidth, UI::unitHeight));
		firstFrameInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(secondColStartX + UI::border + labelWidth, y), wxSize(inputWidth, UI::unitHeight), wxTE_PROCESS_ENTER);
		firstFrameInput->SetValidator(wxIntegerValidator<uint32_t>());
		firstFrameInput->Bind(wxEVT_TEXT_ENTER, &AnimationCreatorDialog::OnTimingFramesChanged, this);

		y += UI::border + UI::unitHeight;
		lastFrameLabel = new wxStaticText(this, wxID_ANY, "Last frame", wxPoint(secondColStartX, y + UI::shift), wxSize(labelWidth, UI::unitHeight));
		lastFrameInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(secondColStartX + UI::border + labelWidth, y), wxSize(inputWidth, UI::unitHeight), wxTE_PROCESS_ENTER);
		lastFrameInput->SetValidator(wxIntegerValidator<uint32_t>());
		lastFrameInput->Bind(wxEVT_TEXT_ENTER, &AnimationCreatorDialog::OnTimingFramesChanged, this);

		y += UI::border + UI::unitHeight;
		animPreview = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap, wxPoint(secondColStartX, y), wxSize(labelWidth + inputWidth + UI::border, labelWidth + inputWidth + UI::border));

		y += 2 * UI::border + labelWidth + inputWidth;
		frameTimeLabel = new wxStaticText(this, wxID_ANY, "Frame time", wxPoint(secondColStartX, y + UI::shift), wxSize(labelWidth, UI::unitHeight));
		frameTimeInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(secondColStartX + UI::border + labelWidth, y), wxSize(inputWidth, UI::unitHeight), wxTE_PROCESS_ENTER);
		wxFloatingPointValidator<float> validator(3);
		validator.SetMin(0.f);
		frameTimeInput->SetValidator(validator);
		frameTimeInput->Bind(wxEVT_TEXT_ENTER, &AnimationCreatorDialog::OnTimingChanged, this);

		wxSize clientSize = GetClientSize();
		info = new wxStaticText(this, wxID_ANY, "Please browse for an image", wxPoint(UI::border, clientSize.y - UI::border - 20), wxSize(200, UI::unitHeight));
		createButton = new wxButton(this, wxID_OK, "Create", wxPoint(clientSize.x - 2 * UI::border - 2 * UI::buttonSize.x, clientSize.y - UI::border - UI::buttonSize.y), UI::buttonSize);
		cancelButton = new wxButton(this, wxID_CANCEL, "Cancel", wxPoint(clientSize.x - UI::border - UI::buttonSize.x, clientSize.y - UI::border - UI::buttonSize.y), UI::buttonSize);
		createButton->Enable(false);
	}


	void AnimationCreatorDialog::OnBrowse(wxCommandEvent& e)
	{
		RelativeSaveDialog browseDialog(this, "Save Animation", ProjectData::GetPath() + "Assets\\",
			"Sharpheus animation files(*.anim.sharpheus) | *.anim.sharpheus");

		if (!browseDialog.Show())
			return;

		path = browseDialog.GetPath();
		pathText->SetLabel(path);
		if (wxFileExists(ProjectData::GetPath() + "Assets\\" + path)) {
			FillWithAnim(path);
		} else {
			if (!imgPath.empty()) {
				createButton->Enable(true);
				createButton->SetLabel("Create");
			}
		}
	}


	void AnimationCreatorDialog::OnBrowseImg(wxCommandEvent& e)
	{
		RelativeOpenDialog browseDialog(this, "Browse for image", ProjectData::GetPath() + "Assets\\",
			"Image files(*.png, *.jpg, *.jpeg, *.bmp; *.gif) | *.png; *.jpg; *.jpeg; *.bmp; *.gif");

		if (!browseDialog.Show())
			return;

		imgPath = browseDialog.GetPath();
		imgPathText->SetLabel(imgPath);
		
		if (!path.empty()) {
			createButton->Enable(true);
		}
		SetAtlas(imgPath);
		wxSize imgSize = img.GetSize();
		frameWidth = imgSize.x;
		frameHeight = imgSize.y;
		frameCols = 1;
		frameRows = 1;
		numOfFrames = 1;
		startFrame = 0;
		endFrame = 0;
		frameTime = 1.f;
		
		FillSecondColumn();
		StartAnimPreview();
	}


	void AnimationCreatorDialog::OnSlicingWHChanged(wxCommandEvent& e)
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
		startFrame = 0;
		endFrame = numOfFrames - 1;
		colInput->SetValue(wxString::Format("%d", frameCols));
		rowInput->SetValue(wxString::Format("%d", frameRows));
		numOfFramesText->SetLabel(wxString::Format("%d", numOfFrames));
		firstFrameInput->SetValue("0");
		lastFrameInput->SetValue(wxString::Format("%d", numOfFrames - 1));
		((wxIntegerValidator<uint32_t>*)firstFrameInput->GetValidator())->SetRange(0, numOfFrames - 1);
		((wxIntegerValidator<uint32_t>*)lastFrameInput->GetValidator())->SetRange(0, numOfFrames - 1);

		StartAnimPreview();
	}


	void AnimationCreatorDialog::OnSlicingCRChanged(wxCommandEvent& e)
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
		startFrame = 0;
		endFrame = numOfFrames - 1;
		widthInput->SetValue(wxString::Format("%d", frameWidth));
		heightInput->SetValue(wxString::Format("%d", frameHeight));
		numOfFramesText->SetLabel(wxString::Format("%d", numOfFrames));
		firstFrameInput->SetValue("0");
		lastFrameInput->SetValue(wxString::Format("%d", numOfFrames - 1));
		((wxIntegerValidator<uint32_t>*)firstFrameInput->GetValidator())->SetRange(0, numOfFrames - 1);
		((wxIntegerValidator<uint32_t>*)lastFrameInput->GetValidator())->SetRange(0, numOfFrames - 1);

		StartAnimPreview();
	}


	void AnimationCreatorDialog::OnTimingFramesChanged(wxCommandEvent& e)
	{
		unsigned long start = 0, end = 0;
		firstFrameInput->GetValue().ToULong(&start);
		lastFrameInput->GetValue().ToULong(&end);
		startFrame = start;
		endFrame = end;
		if (start > end) {
			endFrame = startFrame;
			lastFrameInput->SetValue(wxString::Format("%d", start));
		}
		StartAnimPreview();
	}


	void AnimationCreatorDialog::OnTimingChanged(wxCommandEvent& e)
	{
		double value;
		frameTimeInput->GetValue().ToDouble(&value);
		frameTime = value;
		StartAnimPreview();
	}


	void AnimationCreatorDialog::OnTimerTick(wxTimerEvent& e)
	{
		++currFrameInd;
		if (currFrameInd > endFrame) {
			currFrameInd = startFrame;
		}
		RefreshAnimPreview();
	}


	void AnimationCreatorDialog::FillWithAnim(const wxString& animPath)
	{
		path = animPath;

		Animation* anim = new Animation(wxStr2StdStr(path));
		pathText->SetLabel(path);
		nameInput->SetValue(anim->GetName());
		SetAtlas(anim->GetAtlas()->GetPath());
		frameWidth = anim->GetFrameWidth();
		frameHeight = anim->GetFrameHeight();
		frameCols = anim->GetFrameCols();
		frameRows = anim->GetFrameRows();
		numOfFrames = anim->GetNumOfAllFrames();
		startFrame = anim->GetStartFrame();
		endFrame = anim->GetEndFrame();
		frameTime = anim->GetFrameTime();

		FillSecondColumn();
		StartAnimPreview();
		createButton->Enable(true);
		createButton->SetLabel("Save");
		delete anim;
	}


	void AnimationCreatorDialog::StartAnimPreview()
	{
		if (timer.IsRunning()) {
			timer.Stop();
		}

		currFrameInd = startFrame;
		RefreshAnimPreview();

		timer.Start(frameTime * 1000);
	}


	void AnimationCreatorDialog::RefreshAnimPreview()
	{
		wxImage bitmap = img;
		uint32_t currCol = currFrameInd % frameCols, currRow = currFrameInd / frameCols;
		bitmap.Resize(bitmap.GetSize(), wxPoint(-1 * frameWidth * currCol, -1 * frameHeight * currRow));
		bitmap.Resize(wxSize(frameWidth, frameHeight), wxPoint());
		wxSize animPrevSize = animPreview->GetSize();
		wxSize newBitmapSize;
		if (frameWidth > frameHeight) {
			newBitmapSize = wxSize(animPrevSize.x, frameHeight * animPrevSize.y / frameWidth);
		}
		else {
			newBitmapSize = wxSize(frameWidth * animPrevSize.x / frameHeight, animPrevSize.y);
		}
		bitmap.Rescale(newBitmapSize.x, newBitmapSize.y);
		animPreview->SetBitmap(bitmap);
	}


	void AnimationCreatorDialog::SetAtlas(const wxString& path)
	{
		imgPath = path;
		img = ImageManager::GetImage(ProjectData::GetPath() + "Assets\\" + imgPath);
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
		imgPreview->SetBitmap(bitmap);
		info->SetLabel(wxString::Format("Width: %d px, Height: %d px", bitmapSize.x, bitmapSize.y));
	}


	void AnimationCreatorDialog::FillSecondColumn()
	{
		widthInput->SetValue(wxString::Format("%d", frameWidth));
		heightInput->SetValue(wxString::Format("%d", frameHeight));
		colInput->SetValue(wxString::Format("%d", frameCols));
		rowInput->SetValue(wxString::Format("%d", frameRows));
		numOfFramesText->SetLabel(wxString::Format("%d", numOfFrames));
		firstFrameInput->SetValue(wxString::Format("%d", startFrame));
		lastFrameInput->SetValue(wxString::Format("%d", endFrame));
		frameTimeInput->SetValue(wxString::Format("%.3f", frameTime));

		wxSize imgSize = img.GetSize();
		((wxIntegerValidator<uint32_t>*)widthInput->GetValidator())->SetRange(1, imgSize.x);
		((wxIntegerValidator<uint32_t>*)heightInput->GetValidator())->SetRange(1, imgSize.y);
		((wxIntegerValidator<uint32_t>*)colInput->GetValidator())->SetRange(1, imgSize.x);
		((wxIntegerValidator<uint32_t>*)rowInput->GetValidator())->SetRange(1, imgSize.y);
		((wxIntegerValidator<uint32_t>*)firstFrameInput->GetValidator())->SetRange(0, numOfFrames - 1);
		((wxIntegerValidator<uint32_t>*)lastFrameInput->GetValidator())->SetRange(0, numOfFrames - 1);
	}

}
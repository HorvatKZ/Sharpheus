#include "editor_pch.h"
#include "AnimationCreatorDialog.hpp"
#include "Editor/FileUtils/RelativeFileDialog.hpp"
#include "Editor/Registry/ProjectData.hpp"
#include "Editor/ResourceManagement/ImageManager.hpp"
#include <wx/valnum.h>

#define TIMER_ID 12345

// Event table is used, because wxTimer does not support modern Bind-ing
wxBEGIN_EVENT_TABLE(Sharpheus::AnimationCreatorDialog, wxDialog)
EVT_TIMER(TIMER_ID, OnTimerTick)
wxEND_EVENT_TABLE()


namespace Sharpheus {

	AnimationCreatorDialog::AnimationCreatorDialog(wxWindow* parent)
		: wxDialog(parent, wxID_ANY, "Animation Creator", wxPoint(0, 0), wxSize(560, 520)), timer(this, TIMER_ID)
	{
		wxSize parentSize = parent->GetSize();
		wxSize size = GetSize();
		SetPosition(wxPoint((parentSize.x - size.x) / 2, (parentSize.y - size.y) / 2));

		nameLabel = new wxStaticText(this, wxID_ANY, "Name", wxPoint(UI::border, UI::border + 3), wxSize(60, 22));
		nameLabel->SetFont(UI::titleFont);
		nameInput = new wxTextCtrl(this, wxID_ANY, "Unnamed", wxPoint(2 * UI::border + 60, UI::border), wxSize(200, 22));

		pathLabel = new wxStaticText(this, wxID_ANY, "Path", wxPoint(UI::border, 2 * UI::border + 28), wxSize(60, 22));
		pathLabel->SetFont(UI::titleFont);
		pathText = new wxStaticText(this, wxID_ANY, "", wxPoint(2 * UI::border + 60, 2 * UI::border + 28), wxSize(200, 22), wxST_ELLIPSIZE_START);
		pathText->SetMaxSize(wxSize(200, 22));
		browse = new wxButton(this, wxID_ANY, "Browse...", wxPoint(3 * UI::border + 260, 2 * UI::border + 25), UI::buttonSize);
		browse->Bind(wxEVT_BUTTON, &AnimationCreatorDialog::OnBrowse, this);

		imgLabel = new wxStaticText(this, wxID_ANY, "Atlas", wxPoint(UI::border, 3 * UI::border + 53), wxSize(60, 22));
		imgLabel->SetFont(UI::titleFont);
		imgPathText = new wxStaticText(this, wxID_ANY, "None", wxPoint(2 * UI::border + 60, 3 * UI::border + 53), wxSize(200, 22), wxST_ELLIPSIZE_START);
		imgPathText->SetMaxSize(wxSize(200, 22));
		browseImg = new wxButton(this, wxID_ANY, "Browse...", wxPoint(3 * UI::border + 260, 3 * UI::border + 50), UI::buttonSize);
		browseImg->Bind(wxEVT_BUTTON, &AnimationCreatorDialog::OnBrowseImg, this);

		uint32_t firstColWidth = 2 * UI::border + 260 + UI::buttonSize.x;
		imgPreview = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap, wxPoint(UI::border, 3 * UI::border + 75), wxSize(firstColWidth, firstColWidth));

		uint32_t secondColStartX = 4 * UI::border + firstColWidth;
		slicingLabel = new wxStaticText(this, wxID_ANY, "Slicing", wxPoint(secondColStartX, UI::border + 3));
		slicingLabel->SetFont(UI::titleFont);

		widthLabel = new wxStaticText(this, wxID_ANY, "Frame width", wxPoint(secondColStartX, 2 * UI::border + 25), wxSize(90, 22));
		widthInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(secondColStartX + UI::border + 90, 2 * UI::border + 22), wxSize(60, 22), wxTE_PROCESS_ENTER);
		widthInput->SetValidator(wxIntegerValidator<uint32_t>());
		widthInput->Bind(wxEVT_TEXT_ENTER, &AnimationCreatorDialog::OnSlicingWHChanged, this);

		heightLabel = new wxStaticText(this, wxID_ANY, "Frame height", wxPoint(secondColStartX, 3 * UI::border + 47), wxSize(90, 22));
		heightInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(secondColStartX + UI::border + 90, 3 * UI::border + 44), wxSize(60, 22), wxTE_PROCESS_ENTER);
		heightInput->SetValidator(wxIntegerValidator<uint32_t>());
		heightInput->Bind(wxEVT_TEXT_ENTER, &AnimationCreatorDialog::OnSlicingWHChanged, this);

		colLabel = new wxStaticText(this, wxID_ANY, "Columns", wxPoint(secondColStartX, 4 * UI::border + 69), wxSize(90, 22));
		colInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(secondColStartX + UI::border + 90, 4 * UI::border + 66), wxSize(60, 22), wxTE_PROCESS_ENTER);
		colInput->SetValidator(wxIntegerValidator<uint32_t>());
		colInput->Bind(wxEVT_TEXT_ENTER, &AnimationCreatorDialog::OnSlicingCRChanged, this);

		rowLabel = new wxStaticText(this, wxID_ANY, "Rows", wxPoint(secondColStartX, 5 * UI::border + 91), wxSize(90, 22));
		rowInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(secondColStartX + UI::border + 90, 5 * UI::border + 88), wxSize(60, 22), wxTE_PROCESS_ENTER);
		rowInput->SetValidator(wxIntegerValidator<uint32_t>());
		rowInput->Bind(wxEVT_TEXT_ENTER, &AnimationCreatorDialog::OnSlicingCRChanged, this);

		numOfFramesLabel = new wxStaticText(this, wxID_ANY, "Frames:", wxPoint(secondColStartX, 6 * UI::border + 113), wxSize(90, 22));
		numOfFramesText = new wxStaticText(this, wxID_ANY, "0", wxPoint(secondColStartX + UI::border + 90, 6 * UI::border + 113), wxSize(60, 22));

		timingLabel = new wxStaticText(this, wxID_ANY, "Timing", wxPoint(secondColStartX, 7 * UI::border + 143));
		timingLabel->SetFont(UI::titleFont);

		firstFrameLabel = new wxStaticText(this, wxID_ANY, "First frame", wxPoint(secondColStartX, 8 * UI::border + 165), wxSize(90, 22));
		firstFrameInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(secondColStartX + UI::border + 90, 8 * UI::border + 162), wxSize(60, 22), wxTE_PROCESS_ENTER);
		firstFrameInput->SetValidator(wxIntegerValidator<uint32_t>());
		firstFrameInput->Bind(wxEVT_TEXT_ENTER, &AnimationCreatorDialog::OnTimingFramesChanged, this);

		lastFrameLabel = new wxStaticText(this, wxID_ANY, "Last frame", wxPoint(secondColStartX, 9 * UI::border + 187), wxSize(90, 22));
		lastFrameInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(secondColStartX + UI::border + 90, 9 * UI::border + 184), wxSize(60, 22), wxTE_PROCESS_ENTER);
		lastFrameInput->SetValidator(wxIntegerValidator<uint32_t>());
		lastFrameInput->Bind(wxEVT_TEXT_ENTER, &AnimationCreatorDialog::OnTimingFramesChanged, this);

		animPreview = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap, wxPoint(secondColStartX, 10 * UI::border + 206), wxSize(150 + UI::border, 150 + UI::border));
		
		frameTimeLabel = new wxStaticText(this, wxID_ANY, "Frame time", wxPoint(secondColStartX, 12 * UI::border + 359), wxSize(90, 22));
		frameTimeInput = new wxTextCtrl(this, wxID_ANY, "0", wxPoint(secondColStartX + UI::border + 90, 12 * UI::border + 356), wxSize(60, 22), wxTE_PROCESS_ENTER);
		wxFloatingPointValidator<float> validator(3);
		validator.SetMin(0.f);
		frameTimeInput->SetValidator(validator);
		frameTimeInput->Bind(wxEVT_TEXT_ENTER, &AnimationCreatorDialog::OnTimingChanged, this);

		wxSize clientSize = GetClientSize();
		info = new wxStaticText(this, wxID_ANY, "Please browse for an image", wxPoint(UI::border, clientSize.y - UI::border - 20), wxSize(200, 22));
		createButton = new wxButton(this, wxID_OK, "Create", wxPoint(clientSize.x - 2 * UI::border - 2 * UI::buttonSize.x, clientSize.y - UI::border - UI::buttonSize.y), UI::buttonSize);
		cancelButton = new wxButton(this, wxID_CANCEL, "Cancel", wxPoint(clientSize.x - UI::border - UI::buttonSize.x, clientSize.y - UI::border - UI::buttonSize.y), UI::buttonSize);
		createButton->Enable(false);
	}


	AnimationCreatorDialog::~AnimationCreatorDialog()
	{
	}


	void AnimationCreatorDialog::EndModal(int retCode)
	{
		name = nameInput->GetValue();

		wxDialog::EndModal(retCode);
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
			Animation* anim = new Animation(wxStr2StdStr(path));
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
			delete anim;
		}
		else {
			if (!imgPath.empty()) {
				createButton->Enable(true);
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
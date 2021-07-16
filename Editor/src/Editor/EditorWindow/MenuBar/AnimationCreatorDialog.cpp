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

	AnimationCreatorDialog::AnimationCreatorDialog(wxWindow* parent, const wxString& defaultAnimPath)
		: AtlasDialog(parent, "Animation Creator"), timer(this, TIMER_ID)
	{
		BuildAnimDialog();

		if (!defaultAnimPath.empty() && wxFileExists(ProjectData::GetPath() + "Assets\\" + defaultAnimPath)) {
			if (defaultAnimPath.Length() > 15 && defaultAnimPath.Right(15) == ".anim.sharpheus") {
				FillWithAnim(defaultAnimPath);
			}
		}
	}


	AnimationCreatorDialog::~AnimationCreatorDialog()
	{
	}


	void AnimationCreatorDialog::BuildAnimDialog()
	{
		browse->Bind(wxEVT_BUTTON, &AnimationCreatorDialog::OnBrowse, this);
		browseImg->Bind(wxEVT_BUTTON, &AnimationCreatorDialog::OnBrowseImg, this);
		widthInput->Bind(wxEVT_TEXT_ENTER, &AnimationCreatorDialog::OnSlicingWHChanged, this);
		heightInput->Bind(wxEVT_TEXT_ENTER, &AnimationCreatorDialog::OnSlicingWHChanged, this);
		colInput->Bind(wxEVT_TEXT_ENTER, &AnimationCreatorDialog::OnSlicingCRChanged, this);
		rowInput->Bind(wxEVT_TEXT_ENTER, &AnimationCreatorDialog::OnSlicingCRChanged, this);

		uint32_t firstColWidth = 2 * UI::border + nameLabelWidth + nameInputWidth + UI::buttonSize.x;
		uint32_t secondColStartX = 4 * UI::border + firstColWidth;
		uint32_t y = 7 * UI::border + 5 * UI::unitHeight + UI::heightPadding;

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
		if (timer.IsRunning()) {
			timer.Stop();
		}
		AtlasDialog::OnBrowseImg(e);

		if (img.IsOk()) {
			startFrame = 0;
			endFrame = 0;
			frameTime = 1.f;

			FillTiming();
			StartAnimPreview();

			if (!path.empty()) {
				createButton->Enable(true);
			}
		}
	}


	void AnimationCreatorDialog::OnSlicingWHChanged(wxCommandEvent& e)
	{
		AtlasDialog::OnSlicingWHChanged(e);

		startFrame = 0;
		endFrame = numOfFrames - 1;
		firstFrameInput->SetValue("0");
		lastFrameInput->SetValue(wxString::Format("%d", numOfFrames - 1));
		((wxIntegerValidator<uint32_t>*)firstFrameInput->GetValidator())->SetRange(0, numOfFrames - 1);
		((wxIntegerValidator<uint32_t>*)lastFrameInput->GetValidator())->SetRange(0, numOfFrames - 1);

		StartAnimPreview();
	}


	void AnimationCreatorDialog::OnSlicingCRChanged(wxCommandEvent& e)
	{
		AtlasDialog::OnSlicingCRChanged(e);

		startFrame = 0;
		endFrame = numOfFrames - 1;
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
		Animation* anim = new Animation(wxStr2StdStr(animPath));

		if (anim->GetAtlas() == nullptr || !SetAtlas(anim->GetAtlas()->GetPath())) {
			SetDefaults();
			return;
		}

		path = animPath;
		pathText->SetLabel(path);
		nameInput->SetValue(anim->GetName());
		filterCheckBox->SetValue(anim->GetAtlas()->IsFiltered());
		frameWidth = anim->GetFrameWidth();
		frameHeight = anim->GetFrameHeight();
		frameCols = anim->GetFrameCols();
		frameRows = anim->GetFrameRows();
		numOfFrames = anim->GetNumOfAllFrames();
		startFrame = anim->GetStartFrame();
		endFrame = anim->GetEndFrame();
		frameTime = anim->GetFrameTime();

		FillSlicing();
		FillTiming();
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


	void AnimationCreatorDialog::FillTiming()
	{
		firstFrameInput->SetValue(wxString::Format("%d", startFrame));
		lastFrameInput->SetValue(wxString::Format("%d", endFrame));
		frameTimeInput->SetValue(wxString::Format("%.3f", frameTime));

		if (img.IsOk()) {
			((wxIntegerValidator<uint32_t>*)firstFrameInput->GetValidator())->SetRange(0, numOfFrames - 1);
			((wxIntegerValidator<uint32_t>*)lastFrameInput->GetValidator())->SetRange(0, numOfFrames - 1);
		}
	}


	void AnimationCreatorDialog::SetDefaults()
	{
		AtlasDialog::SetDefaults();
		if (timer.IsRunning()) {
			timer.Stop();
		}

		startFrame = 0;
		endFrame = 0;
		frameTime = 1.f;

		animPreview->SetBitmap(wxNullBitmap);
		FillTiming();
	}

}
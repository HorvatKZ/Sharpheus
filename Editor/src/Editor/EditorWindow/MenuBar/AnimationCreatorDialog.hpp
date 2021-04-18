#pragma once

#include "editor_pch.h"

namespace Sharpheus {

	class AnimationCreatorDialog : public wxDialog
	{
	public:
		AnimationCreatorDialog(wxWindow* parent, const wxString& defaultAnimPath = "");
		virtual ~AnimationCreatorDialog();

		inline const wxString& GetName() { return name; }
		inline const wxString& GetPath() { return path; }
		inline const wxString& GetAtlas() { return imgPath; }
		inline uint32_t GetFrameWidth() { return frameWidth; }
		inline uint32_t GetFrameHeight() { return frameHeight; }
		inline uint32_t GetStartFrame() { return startFrame; }
		inline uint32_t GetEndFrame() { return endFrame; }
		inline float GetFrameTime() { return frameTime; }

		virtual void EndModal(int retCode) override;

	private:
		wxStaticText* nameLabel;
		wxTextCtrl* nameInput;

		wxStaticText* pathLabel;
		wxStaticText* pathText;
		wxButton* browse;

		wxStaticText* imgLabel;
		wxStaticText* imgPathText;
		wxButton* browseImg;
		wxStaticBitmap* imgPreview;
		wxStaticText* info;

		wxStaticText* slicingLabel;
		wxStaticText* widthLabel, * heightLabel, * colLabel, * rowLabel, * numOfFramesLabel, * numOfFramesText;
		wxTextCtrl* widthInput, * heightInput, * colInput, * rowInput;

		wxStaticText* timingLabel;
		wxStaticText* firstFrameLabel, * lastFrameLabel, * frameTimeLabel;
		wxTextCtrl* firstFrameInput, * lastFrameInput, * frameTimeInput;
		wxStaticBitmap* animPreview;

		wxButton* createButton;
		wxButton* cancelButton;

		wxImage img;
		wxString name, path, imgPath;
		uint32_t frameWidth = 1, frameHeight = 1, frameCols = 1, frameRows = 1, numOfFrames = 1, startFrame = 0, endFrame = 0;
		float frameTime = 1.f;
		wxTimer timer;
		uint32_t currFrameInd = 0;

		static uint32_t labelWidth;
		static uint32_t inputWidth;
		static uint32_t nameLabelWidth;
		static uint32_t nameInputWidth;

		void BuildDialog();

		void OnBrowse(wxCommandEvent& e);
		void OnBrowseImg(wxCommandEvent& e);
		void OnSlicingWHChanged(wxCommandEvent& e);
		void OnSlicingCRChanged(wxCommandEvent& e);
		void OnTimingFramesChanged(wxCommandEvent& e);
		void OnTimingChanged(wxCommandEvent& e);
		void OnTimerTick(wxTimerEvent& e);

		void FillWithAnim(const wxString& animPath);
		void StartAnimPreview();
		void RefreshAnimPreview();
		void SetAtlas(const wxString& path);
		void FillSecondColumn();

		wxDECLARE_EVENT_TABLE();
	};

}
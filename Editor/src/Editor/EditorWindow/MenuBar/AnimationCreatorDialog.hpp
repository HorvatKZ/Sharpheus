#pragma once

#include "AtlasDialog.hpp"

namespace Sharpheus {

	class AnimationCreatorDialog : public AtlasDialog
	{
	public:
		AnimationCreatorDialog(wxWindow* parent, const wxString& defaultAnimPath = "");
		virtual ~AnimationCreatorDialog();

		inline uint32 GetStartFrame() { return startFrame; }
		inline uint32 GetEndFrame() { return endFrame; }
		inline float GetFrameTime() { return frameTime; }

	private:
		wxStaticText* timingLabel;
		wxStaticText* firstFrameLabel, * lastFrameLabel, * frameTimeLabel;
		wxTextCtrl* firstFrameInput, * lastFrameInput, * frameTimeInput;
		wxStaticBitmap* animPreview;

		uint32 startFrame = 0, endFrame = 0;
		float frameTime = 1.f;
		wxTimer timer;
		uint32 currFrameInd = 0;

		void BuildAnimDialog();

		void OnBrowse(wxCommandEvent& e);
		void OnBrowseImg(wxCommandEvent& e) override;
		void OnSlicingWHChanged(wxCommandEvent& e) override;
		void OnSlicingCRChanged(wxCommandEvent& e) override;
		void OnTimingFramesChanged(wxCommandEvent& e);
		void OnTimingChanged(wxCommandEvent& e);
		void OnTimerTick(wxTimerEvent& e);

		void FillWithAnim(const wxString& animPath);
		void StartAnimPreview();
		void RefreshAnimPreview();
		void FillTiming();
		void SetDefaults() override;

		wxDECLARE_EVENT_TABLE();
	};

}
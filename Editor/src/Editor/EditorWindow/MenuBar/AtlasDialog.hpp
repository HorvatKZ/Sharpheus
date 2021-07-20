#pragma once

#include "editor_pch.h"

namespace Sharpheus {

	class AtlasDialog : public wxDialog
	{
	public:
		AtlasDialog(wxWindow* parent, const wxString& title);
		virtual ~AtlasDialog();

		inline const wxString& GetName() { return name; }
		inline const wxString& GetPath() { return path; }
		inline const wxString& GetAtlas() { return imgPath; }
		inline uint32 GetFrameWidth() { return frameWidth; }
		inline uint32 GetFrameHeight() { return frameHeight; }
		inline bool UseFilter() { return useFilter; }

		virtual void EndModal(int retCode) override;

	protected:
		wxStaticText* nameLabel;
		wxTextCtrl* nameInput;

		wxStaticText* pathLabel;
		wxStaticText* pathText;
		wxButton* browse;

		wxStaticText* imgLabel;
		wxStaticText* imgPathText;
		wxButton* browseImg;
		wxStaticBitmap* imgPreview;
		wxStaticText* filterLabel;
		wxCheckBox* filterCheckBox;
		wxStaticText* info;

		wxStaticText* slicingLabel;
		wxStaticText* widthLabel, * heightLabel, * colLabel, * rowLabel, * numOfFramesLabel, * numOfFramesText;
		wxTextCtrl* widthInput, * heightInput, * colInput, * rowInput;

		wxButton* createButton;
		wxButton* cancelButton;

		wxImage img;
		wxString name, path, imgPath;
		uint32 frameWidth = 0, frameHeight = 0, frameCols = 0, frameRows = 0, numOfFrames = 0;
		bool useFilter;

		static const uint32 labelWidth;
		static const uint32 inputWidth;
		static const uint32 nameLabelWidth;
		static const uint32 nameInputWidth;

		void BuildDialog();
		void FillSlicing();

		virtual void OnBrowseImg(wxCommandEvent& e);
		virtual void OnSlicingWHChanged(wxCommandEvent& e);
		virtual void OnSlicingCRChanged(wxCommandEvent& e);

		virtual bool SetAtlas(const wxString& path);
		virtual void SetDefaults();
	};

}
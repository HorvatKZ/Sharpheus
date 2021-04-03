#include "editor_pch.h"
#include "AnimationCreatorDialog.hpp"
#include "Editor/FileUtils/RelativeFileDialog.hpp"
#include "Editor/Registry/ProjectData.hpp"
#include "Editor/ResourceManagement/ImageManager.hpp"


namespace Sharpheus {

	AnimationCreatorDialog::AnimationCreatorDialog(wxWindow* parent)
		: wxDialog(parent, wxID_ANY, "Animation Creator", wxPoint(0, 0), wxSize(800, 600))
	{
		wxSize parentSize = parent->GetSize();
		wxSize size = GetSize();
		SetPosition(wxPoint((parentSize.x - size.x) / 2, (parentSize.y - size.y) / 2));

		nameLabel = new wxStaticText(this, wxID_ANY, "Name", wxPoint(UI::border, UI::border + 3), wxSize(60, 22));
		nameLabel->SetFont(UI::titleFont);
		nameInput = new wxTextCtrl(this, wxID_ANY, "Unnamed", wxPoint(2 * UI::border + 60, UI::border), wxSize(200, 22));

		imgLabel = new wxStaticText(this, wxID_ANY, "Image", wxPoint(UI::border, 2 * UI::border + 28), wxSize(60, 22));
		imgLabel->SetFont(UI::titleFont);
		imgPath = new wxStaticText(this, wxID_ANY, "None", wxPoint(2 * UI::border + 60, 2 * UI::border + 28), wxSize(200, 22), wxST_ELLIPSIZE_START);
		imgPath->SetMaxSize(wxSize(200, 22));
		browse = new wxButton(this, wxID_ANY, "Browse...", wxPoint(3 * UI::border + 260, 2 * UI::border + 25), UI::buttonSize);
		browse->Bind(wxEVT_BUTTON, &AnimationCreatorDialog::OnBrowse, this);

		img = new wxStaticBitmap(this, wxID_ANY, wxNullBitmap, wxPoint(UI::border, 2 * UI::border + 50), wxSize(300, 300));

		wxSize clientSize = GetClientSize();
		createButton = new wxButton(this, wxID_OK, "Create", wxPoint(clientSize.x - 2 * UI::border - 2 * UI::buttonSize.x, clientSize.y - UI::border - UI::buttonSize.y), UI::buttonSize);
		cancelButton = new wxButton(this, wxID_CANCEL, "Cancel", wxPoint(clientSize.x - UI::border - UI::buttonSize.x, clientSize.y - UI::border - UI::buttonSize.y), UI::buttonSize);
	}


	AnimationCreatorDialog::~AnimationCreatorDialog()
	{
	}


	void AnimationCreatorDialog::EndModal(int retCode)
	{
		wxDialog::EndModal(retCode);
	}


	void AnimationCreatorDialog::OnBrowse(wxCommandEvent& e)
	{
		RelativeOpenDialog browseDialog(this, "Browse for image", ProjectData::GetPath() + "Assets\\",
			"Image files(*.png, *.jpg, *.jpeg, *.bmp; *.gif) | *.png; *.jpg; *.jpeg; *.bmp; *.gif");

		if (!browseDialog.Show())
			return;

		wxString path = browseDialog.GetPath();
		imgPath->SetLabel(path);
		wxImage bitmap = ImageManager::GetImage(ProjectData::GetPath() + "Assets\\" + path);
		wxSize imgSize = bitmap.GetSize();
		wxSize newImgSize;
		if (imgSize.x > imgSize.y) {
			newImgSize = wxSize(300, imgSize.y * 300 / imgSize.x);
		}
		else {
			newImgSize = wxSize(imgSize.x * 300 / imgSize.y, 300);
		}
		bitmap.Rescale(newImgSize.x, newImgSize.y);
		img->SetBitmap(bitmap);
		img->SetSize(newImgSize);
	}

}
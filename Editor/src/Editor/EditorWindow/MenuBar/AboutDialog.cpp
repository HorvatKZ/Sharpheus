#include "editor_pch.h"
#include "AboutDialog.hpp"
#include "Editor/Registry/EditorData.hpp"
#include "Editor/ResourceManagement/ImageManager.hpp"


namespace Sharpheus {

	uint32_t AboutDialog::logoHeight = 100;
	
	AboutDialog::AboutDialog(wxWindow* parent)
		: wxDialog(parent, wxID_ANY, "About Sharpheus", wxPoint(), wxSize(500, 150))
	{
		wxSize parentSize = parent->GetSize();
		wxSize clientSize = GetClientSize();
		SetPosition(wxPoint((parentSize.x - clientSize.x) / 2, (parentSize.y - clientSize.y) / 2));

		wxString nameStr = "Sharpheus Game Engine and Editor (" + EditorData::GetVersion() + ")";
		nameLabel = new wxStaticText(this, wxID_ANY, nameStr, wxPoint(2 * UI::border + logoHeight, 2 * UI::border));
		nameLabel->SetFont(UI::titleFont);

		authorLabel = new wxStaticText(this, wxID_ANY, "by HorvatKZ", wxPoint(2 * UI::border + logoHeight, 3 * UI::border + UI::unitHeight));

		wxImage logoImg = ImageManager::GetImage("Icons\\sharpheus_icon.png", ImageManager::PathType::RELATIVE_PATH);
		logoImg.Rescale(logoHeight, logoHeight);
		logo = new wxStaticBitmap(this, wxID_ANY, wxBitmap(logoImg), wxPoint(UI::border, UI::border));

		wxString oglString = "Running OpenGL " + EditorData::GetOGLVersion();
		oglLabel = new wxStaticText(this, wxID_ANY, oglString, wxPoint(2 * UI::border + logoHeight, clientSize.y - UI::border - UI::unitHeight));

		okButton = new wxButton(this, wxID_OK, "OK", wxPoint(clientSize.x - UI::border - UI::buttonSize.x, clientSize.y - UI::border - UI::buttonSize.y), UI::buttonSize);
	}


	AboutDialog::~AboutDialog()
	{
	}

}
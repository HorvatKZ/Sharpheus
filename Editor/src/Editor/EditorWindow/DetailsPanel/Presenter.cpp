#include "editor_pch.h"
#include "Presenter.hpp"
#include "Editor/ResourceManagement/ImageManager.hpp"
#include "Editor/EditorCommands.hpp"


namespace Sharpheus {

	wxColour Presenter::redish(176, 0, 32);
	wxColour Presenter::greenish(3, 153, 18);
	wxColour Presenter::blueish(55, 0, 179);
	wxColour Presenter::greyish(70, 70, 80);

	uint32_t Presenter::previewHeight = 50;

	// Presenter

	Presenter::Presenter(wxWindow* parent, const std::string& title, Signal signal, uint32_t& y) : parent(parent), signal(signal)
	{
		wxSize limit(parent->GetSize().x / 2 - 2 * UI::border, 16);
		this->title = new wxStaticText(parent, wxID_ANY, title, wxPoint(UI::border, y + UI::border), limit, wxST_ELLIPSIZE_END);
		this->title->SetFont(UI::titleFont);
		this->title->SetMaxSize(limit);
	}

	Presenter::~Presenter()
	{
		wxREMOVE(title);
	}


	// FieldPresenter

	FieldPresenter::FieldPresenter(wxWindow* parent, const std::string& title, Signal signal, uint32_t& y)
		: Presenter(parent, title, signal, y)
	{
		wxSize extent = this->title->GetTextExtent(title);
		uint32_t start = parent->GetSize().x / 2;
		uint32_t width = parent->GetSize().x - start - UI::border;
		input = new wxTextCtrl(parent, wxID_ANY, "", wxPoint(start, y), wxSize(width, UI::unitHeight), wxTE_PROCESS_ENTER);
		y += UI::heightPadding;
	}

	FieldPresenter::~FieldPresenter()
	{
		wxREMOVE(input);
	}


	// HeaderPresenter

	HeaderPresenter::HeaderPresenter(wxWindow* parent, const std::string& title, Signal signal, NameSignal nameSignal, uint32_t& y)
		: FieldPresenter(parent, title, signal, y), nameSignal(nameSignal)
	{
		InitBitmaps();
		wxSize titleExtent = this->title->GetTextExtent("Name");
		uint32_t realWidth = parent->GetVirtualSize().x;

		this->title->SetSize(titleExtent);
		input->SetPosition(wxPoint(2 * UI::border + titleExtent.x, y - UI::heightPadding));
		input->SetSize(wxSize(realWidth - 4 * UI::border - titleExtent.x - UI::extUnitHeight, UI::unitHeight));
		input->Bind(wxEVT_TEXT_ENTER, &HeaderPresenter::OnNameChanged, this);

		visibilityButton = new wxBitmapButton(parent, wxID_ANY, wxNullBitmap, wxPoint(realWidth - UI::border - UI::extUnitHeight, y - UI::heightPadding - 1), wxSize(UI::extUnitHeight, UI::extUnitHeight));
		visibilityButton->Bind(wxEVT_BUTTON, &HeaderPresenter::OnVisibilityChanged, this);
	}

	HeaderPresenter::~HeaderPresenter()
	{
		wxREMOVE(visibilityButton);
	}

	void HeaderPresenter::SetCurrent(GameObject* curr)
	{
		Presenter::SetCurrent(curr);

		input->SetLabel(curr->GetName());
		visibilityButton->SetBitmap(curr->IsVisible() ? visibleBitmap : invisibleBitmap);
	}

	void HeaderPresenter::InitBitmaps()
	{
		visibleBitmap = wxBitmap(ImageManager::GetImage("visible.png", ImageManager::PathType::DETAILSPANEL));
		invisibleBitmap = wxBitmap(ImageManager::GetImage("invisible.png", ImageManager::PathType::DETAILSPANEL));
	}

	void HeaderPresenter::OnNameChanged(wxCommandEvent& e)
	{
		if (curr != nullptr) {
			const std::string oldName = curr->GetName();
			curr->SetName(wxStr2StdStr(input->GetValue()));
			const std::string newName = curr->GetName();
			nameSignal(oldName, newName);
			signal();
		}
	}

	void HeaderPresenter::OnVisibilityChanged(wxCommandEvent& e)
	{
		if (curr != nullptr) {
			curr->SwitchVisiblity();
			signal();
		}
	}


	// TrafoPresenterBase

	TrafoPresenterBase::TrafoPresenterBase(wxWindow* parent, const std::string& title, Signal signal, uint32_t& y)
		: Presenter(parent, title, signal, y)
	{
		InitBitmaps();

		uint32_t width = parent->GetSize().x;
		uint32_t floatCompWidth = (width - 4 * UI::border - UI::extUnitHeight) / 2;
		y += UI::unitHeight;
		posImg = new wxStaticBitmap(parent, wxID_ANY, posBitmap, wxPoint(UI::border, y), wxSize(UI::extUnitHeight, UI::extUnitHeight));
		posX = new FloatComponentCtrl(parent, "X", wxPoint(2 * UI::border + UI::extUnitHeight, y), floatCompWidth, redish);
		posY = new FloatComponentCtrl(parent, "Y", wxPoint(width - UI::border - floatCompWidth, y), floatCompWidth, blueish);
		y += UI::heightPadding;
		scaleImg = new wxStaticBitmap(parent, wxID_ANY, scaleBitmap, wxPoint(UI::border, y), wxSize(UI::extUnitHeight, UI::extUnitHeight));
		scaleX = new FloatComponentCtrl(parent, "X", wxPoint(2 * UI::border + UI::extUnitHeight, y), floatCompWidth, redish);
		scaleY = new FloatComponentCtrl(parent, "Y", wxPoint(width - UI::border - floatCompWidth, y), floatCompWidth, blueish);
		y += UI::heightPadding;
		rotImg = new wxStaticBitmap(parent, wxID_ANY, rotBitmap, wxPoint(UI::border, y), wxSize(UI::extUnitHeight, UI::extUnitHeight));
		rot = new AngleComponentCtrl(parent, "Angle", wxPoint(2 * UI::border + UI::extUnitHeight, y), width - UI::extUnitHeight - 3 * UI::border, greenish);
		y += UI::heightPadding + UI::border;
	}

	TrafoPresenterBase::~TrafoPresenterBase()
	{
		wxREMOVE(posImg);
		wxREMOVE(posX);
		wxREMOVE(posY);
		wxREMOVE(scaleImg);
		wxREMOVE(scaleX);
		wxREMOVE(scaleY);
		wxREMOVE(rotImg);
		wxREMOVE(rot);
	}

	void TrafoPresenterBase::InitBitmaps()
	{
		posBitmap = wxBitmap(ImageManager::GetImage("position.png", ImageManager::PathType::DETAILSPANEL));
		scaleBitmap = wxBitmap(ImageManager::GetImage("scale.png", ImageManager::PathType::DETAILSPANEL));
		rotBitmap = wxBitmap(ImageManager::GetImage("rotation.png", ImageManager::PathType::DETAILSPANEL));
	}


	// MainTrafoPresenter

	MainTrafoPresenter::MainTrafoPresenter(wxWindow* parent, const std::string& title, Signal signal, uint32_t& y)
		: TrafoPresenterBase(parent, title, signal, y)
	{
		InitBitmaps();

		uint32_t width = parent->GetSize().x;
		rot->SetWidth(rot->GetWidth() - 24 - UI::border);
		trafoTypeSwitch = new wxBitmapButton(parent, wxID_ANY, localBitmap, wxPoint(width - UI::border - UI::extUnitHeight, y - UI::heightPadding - UI::border), wxSize(UI::extUnitHeight, UI::extUnitHeight));
		trafoTypeSwitch->Bind(wxEVT_BUTTON, &MainTrafoPresenter::TypeSwitchPressed, this);

		posX->Bind(wxEVT_TEXT_ENTER, &MainTrafoPresenter::HandleChange, this);
		posY->Bind(wxEVT_TEXT_ENTER, &MainTrafoPresenter::HandleChange, this);
		scaleX->Bind(wxEVT_TEXT_ENTER, &MainTrafoPresenter::HandleChange, this);
		scaleY->Bind(wxEVT_TEXT_ENTER, &MainTrafoPresenter::HandleChange, this);
		rot->Bind(wxEVT_TEXT_ENTER, &MainTrafoPresenter::HandleChange, this);
	}

	MainTrafoPresenter::~MainTrafoPresenter()
	{
		wxREMOVE(trafoTypeSwitch);
	}

	void MainTrafoPresenter::SetCurrent(GameObject* curr)
	{
		Presenter::SetCurrent(curr);

		Transform trafo = isWorldTrafo ? curr->GetWorldTrafo() : curr->GetTrafo();
		posX->SetValue(trafo.pos.x);
		posY->SetValue(trafo.pos.y);
		scaleX->SetValue(trafo.scale.x);
		scaleY->SetValue(trafo.scale.y);
		rot->SetValue(trafo.rot);
	}

	void MainTrafoPresenter::InitBitmaps()
	{
		TrafoPresenterBase::InitBitmaps();
		worldBitmap = wxBitmap(ImageManager::GetImage("world.png", ImageManager::PathType::DETAILSPANEL));
		localBitmap = wxBitmap(ImageManager::GetImage("local.png", ImageManager::PathType::DETAILSPANEL));
	}

	void MainTrafoPresenter::TypeSwitchPressed(wxCommandEvent& e)
	{
		isWorldTrafo = !isWorldTrafo;
		trafoTypeSwitch->SetBitmap(isWorldTrafo ? worldBitmap : localBitmap);
		if (curr != nullptr) {
			SetCurrent(curr);
		}
	}

	void MainTrafoPresenter::HandleChange(wxCommandEvent& e)
	{
		if (curr != nullptr) {
			Transform trafo(Point(posX->GetValue(), posY->GetValue()), Point(scaleX->GetValue(), scaleY->GetValue()), rot->GetValue());

			if (isWorldTrafo) {
				curr->SetWorldTrafo(trafo);
			}
			else {
				curr->SetTrafo(trafo);
			}
			signal();
		}
	}


	// TileMapPresenter

	TileMapPresenter::TileMapPresenter(wxWindow* parent, const std::string& title, Signal signal, uint32_t& y)
		: Presenter(parent, title, signal, y)
	{
		y += UI::unitHeight;
		uint32_t x = (parent->GetClientSize().x - UI::smallButtonSize.x) / 2;
		openButton = new wxButton(parent, wxID_ANY, "Open", wxPoint(x, y), UI::smallButtonSize);
		openButton->Bind(wxEVT_BUTTON, &TileMapPresenter::HandleChange, this);
		y += UI::heightPadding;
	}

	TileMapPresenter::~TileMapPresenter()
	{
		wxREMOVE(openButton);
	}

	void TileMapPresenter::SetCurrent(GameObject* curr)
	{
		SPHE_ASSERT(curr->Is(GameObject::Type::TileMap), "Non-tilemap type is passed to TileMapPresenter");
		Presenter::SetCurrent(curr);
	}

	void TileMapPresenter::HandleChange(wxCommandEvent& e)
	{
		EditorCommands::OpenTileMapEditor((TileMap*)curr);
	}
}
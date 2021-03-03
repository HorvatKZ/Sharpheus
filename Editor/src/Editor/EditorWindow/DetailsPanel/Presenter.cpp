#include "editor_pch.h"
#include "Presenter.hpp"
#include "Editor/ResourceManagement/ImageManager.hpp"
#include "BehaviorCreator.hpp"


namespace Sharpheus {

	uint32_t Presenter::border = 5;
	wxFont Presenter::titleFont(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	wxColour Presenter::redish(176, 0, 32);
	wxColour Presenter::greenish(3, 153, 18);
	wxColour Presenter::blueish(55, 0, 179);
	wxColour Presenter::greyish(70, 70, 80);


	// Presenter

	Presenter::Presenter(wxWindow* parent, const std::string& title, Signal signal, uint32_t& y) : parent(parent), signal(signal)
	{
		wxSize limit(parent->GetSize().x / 2 - 2 * border, 16);
		this->title = new wxStaticText(parent, wxID_ANY, title, wxPoint(border, y + 5), limit, wxST_ELLIPSIZE_END);
		this->title->SetFont(titleFont);
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
		uint32_t width = parent->GetSize().x - start - border;
		input = new wxTextCtrl(parent, wxID_ANY, "", wxPoint(start, y), wxSize(width, 22), wxTE_PROCESS_ENTER);
		y += 30;
	}

	FieldPresenter::~FieldPresenter()
	{
		wxREMOVE(input);
	}


	// NamaPresenter

	NamePresenter::NamePresenter(wxWindow* parent, const std::string& title, Signal signal, NameSignal nameSignal, uint32_t& y)
		: FieldPresenter(parent, title, signal, y), nameSignal(nameSignal)
	{
		input->Bind(wxEVT_TEXT_ENTER, &NamePresenter::HandleChange, this);
	}

	NamePresenter::~NamePresenter()
	{
	}

	void NamePresenter::SetCurrent(GameObject* curr)
	{
		Presenter::SetCurrent(curr);

		input->SetLabel(curr->GetName());
	}

	void NamePresenter::Refresh()
	{
		SetCurrent(curr);
	}

	void NamePresenter::HandleChange(wxCommandEvent& e)
	{
		if (curr != nullptr) {
			const std::string oldName = curr->GetName();
			curr->SetName(wxStr2StdStr(input->GetValue()));
			const std::string newName = curr->GetName();
			nameSignal(oldName, newName);
			signal();
		}
	}


	// TrafoPresenterBase

	TrafoPresenterBase::TrafoPresenterBase(wxWindow* parent, const std::string& title, Signal signal, uint32_t& y)
		: Presenter(parent, title, signal, y)
	{
		InitBitmaps();

		uint32_t width = parent->GetSize().x;
		uint32_t floatCompWidth = (width - 4 * border - 24) / 2;
		y += 22;
		posImg = new wxStaticBitmap(parent, wxID_ANY, posBitmap, wxPoint(border, y), wxSize(24, 24));
		posX = new FloatComponentCtrl(parent, "X", wxPoint(2 * border + 24, y), floatCompWidth, redish);
		posY = new FloatComponentCtrl(parent, "Y", wxPoint(width - border - floatCompWidth, y), floatCompWidth, blueish);
		y += 30;
		scaleImg = new wxStaticBitmap(parent, wxID_ANY, scaleBitmap, wxPoint(border, y), wxSize(24, 24));
		scaleX = new FloatComponentCtrl(parent, "X", wxPoint(2 * border + 24, y), floatCompWidth, redish);
		scaleY = new FloatComponentCtrl(parent, "Y", wxPoint(width - border - floatCompWidth, y), floatCompWidth, blueish);
		y += 30;
		rotImg = new wxStaticBitmap(parent, wxID_ANY, rotBitmap, wxPoint(border, y), wxSize(24, 24));
		rot = new AngleComponentCtrl(parent, "Angle", wxPoint(2 * border + 24, y), width - 24 - 3 * border, greenish);
		y += 35;
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
		rot->SetWidth(rot->GetWidth() - 24 - border);
		trafoTypeSwitch = new wxBitmapButton(parent, wxID_ANY, localBitmap, wxPoint(width - border - 24, y - 35), wxSize(24, 24));
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

	void MainTrafoPresenter::Refresh()
	{
		SetCurrent(curr);
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


	// BehaviorPresenter

	BehaviorPresenter::BehaviorPresenter(wxWindow* parent, const std::string& title, std::function<void(uint32_t)>& mainSignal, Signal signal, uint32_t& y)
		: Presenter(parent, title, signal, y), mainSignal(mainSignal), y(y)
	{
		y += 22;
		wxSize extent = this->title->GetTextExtent(title);
		uint32_t parentWidth = parent->GetSize().x;
		typeSelector = new wxComboBox(parent, wxID_ANY, "", wxPoint(border, y), wxSize(parentWidth - 3 * border - 22, 22));
		createNewTypeButton = new wxButton(parent, wxID_ANY, "+", wxPoint(parentWidth - 22 - border, y), wxSize(22, 22));
		y += 30;
	}

	BehaviorPresenter::~BehaviorPresenter()
	{
		wxREMOVE(typeSelector);
		wxREMOVE(createNewTypeButton);
	}

	void BehaviorPresenter::SetCurrent(GameObject* curr)
	{
		Presenter::SetCurrent(curr);

		wxREMOVE(typeSelector);

		uint32_t parentWidth = parent->GetSize().x;
		wxArrayString arr;
		for (auto it = BehaviorCreator::behaviorNames.begin(); it != BehaviorCreator::behaviorNames.end(); ++it) {
			if (BehaviorCreator::IsCompatibleWithParent((*it).first, curr->GetParent())) {
				arr.Add(wxString::Format("%d - %s", (*it).first, (*it).second));
			}
		}
		typeSelector = new wxComboBox(parent, wxID_ANY, "", wxPoint(border, y + 22), wxSize(parentWidth - 3 * border - 22, 22), arr);
		typeSelector->Bind(wxEVT_COMBOBOX, &BehaviorPresenter::HandleChange, this);
	}

	void BehaviorPresenter::Refresh()
	{
		SetCurrent(curr);
	}

	void BehaviorPresenter::HandleChange(wxCommandEvent& e)
	{
		wxString value = typeSelector->GetValue();
		unsigned long subType;
		value.Left(value.find(' ')).ToULong(&subType);
		mainSignal(subType);
	}

}
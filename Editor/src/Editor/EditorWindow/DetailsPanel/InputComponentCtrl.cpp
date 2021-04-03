#include "editor_pch.h"
#include "InputComponentCtrl.hpp"
#include <wx/valnum.h>


namespace Sharpheus {

	wxFont InputComponentCtrl::titleFont(15, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);


	// InputComponentCtrl

	InputComponentCtrl::InputComponentCtrl()
	{
		titleText = nullptr;
		input = nullptr;
	}


	InputComponentCtrl::InputComponentCtrl(wxWindow* parent, const wxString& title, const wxPoint& pos, uint32_t width, const wxColour& color)
		: wxPanel(parent, wxID_ANY, pos, wxSize(width, 24))
	{
		SetBackgroundColour(color);

		titleText = new wxStaticText(this, wxID_ANY, title, wxPoint(3, 0));
		titleText->SetFont(titleFont);
		titleText->SetForegroundColour(*wxWHITE);
		titleText->SetBackgroundColour(color);

		wxSize titleExtent = titleText->GetTextExtent(title);
		input = new wxTextCtrl(this, wxID_ANY, "", wxPoint(titleExtent.x + 6, 1), wxSize(width - titleExtent.x - 7, 22), wxTE_PROCESS_ENTER);
	}


	InputComponentCtrl::~InputComponentCtrl()
	{
	}

	void InputComponentCtrl::SetWidth(uint32_t width)
	{
		uint32_t currWidth = GetSize().x;
		input->SetSize(input->GetSize() - wxSize(currWidth - width, 0));
		SetSize(wxSize(width, 24));
	}


	// IntComponentCtrl

	IntComponentCtrl::IntComponentCtrl() : InputComponentCtrl()
	{
	}


	IntComponentCtrl::IntComponentCtrl(wxWindow* parent, const wxString& title, const wxPoint& pos, uint32_t width, const wxColour& color, int32_t min, int32_t max)
		: InputComponentCtrl(parent, title, pos, width, color)
	{
		wxIntegerValidator validator = wxIntegerValidator<int32_t>(&value, wxNUM_VAL_THOUSANDS_SEPARATOR);
		validator.SetRange(min, max);
		input->SetValidator(validator);
		formater = wxT("%d");
	}


	IntComponentCtrl::~IntComponentCtrl()
	{
	}


	// UIntComponentCtrl

	UIntComponentCtrl::UIntComponentCtrl() : InputComponentCtrl()
	{
	}


	UIntComponentCtrl::UIntComponentCtrl(wxWindow* parent, const wxString& title, const wxPoint& pos, uint32_t width, const wxColour& color, uint32_t min, uint32_t max)
		: InputComponentCtrl(parent, title, pos, width, color)
	{
		wxIntegerValidator validator = wxIntegerValidator<uint32_t>(&value, wxNUM_VAL_THOUSANDS_SEPARATOR);
		validator.SetRange(min, max);
		input->SetValidator(validator);
		formater = wxT("%d");
	}


	UIntComponentCtrl::~UIntComponentCtrl()
	{
	}


	// ByteComponentCtrl

	ByteComponentCtrl::ByteComponentCtrl() : InputComponentCtrl()
	{
	}


	ByteComponentCtrl::ByteComponentCtrl(wxWindow* parent, const wxString& title, const wxPoint& pos, uint32_t width, const wxColour& color, bool hex)
		: InputComponentCtrl(parent, title, pos, width, color)
	{
		input->SetValidator(wxIntegerValidator<uint8_t>(&value));
		
		if (hex) {
			formater = wxT("%x");
		} else {
			formater = wxT("%d");
		}
	}


	ByteComponentCtrl::~ByteComponentCtrl()
	{
	}


	// FloatComponentCtrl

	FloatComponentCtrl::FloatComponentCtrl() : InputComponentCtrl()
	{
	}


	FloatComponentCtrl::FloatComponentCtrl(wxWindow* parent, const wxString& title, const wxPoint& pos, uint32_t width, const wxColour& color, uint32_t precision)
		: InputComponentCtrl(parent, title, pos, width, color)
	{
		input->SetValidator(wxFloatingPointValidator<float>(precision, &value));
		formater = wxT("%." + std::to_string(precision) + "f");
	}


	FloatComponentCtrl::~FloatComponentCtrl()
	{
	}


	// AngleComponentCtrl

	AngleComponentCtrl::AngleComponentCtrl() : InputComponentCtrl()
	{
	}

	AngleComponentCtrl::AngleComponentCtrl(wxWindow* parent, const wxString& title, const wxPoint& pos, uint32_t width, const wxColour& color, uint32_t precision)
		: InputComponentCtrl(parent, title, pos, width, color)
	{
		wxFloatingPointValidator validator = wxFloatingPointValidator<float>(precision, &value);
		validator.SetRange(-180.f, 180.f);
		input->SetValidator(validator);
		formater = wxT("%." + std::to_string(precision) + "f");
	}


	AngleComponentCtrl::~AngleComponentCtrl()
	{
	}
}
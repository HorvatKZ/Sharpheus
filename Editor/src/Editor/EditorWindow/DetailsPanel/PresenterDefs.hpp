#pragma once

#include "Presenter.hpp"
#include <wx/valnum.h>


namespace Sharpheus {

	// StringPresenter

	template<class Class>
	inline StringPresenter<Class>::StringPresenter(wxWindow* parent, StringProvider<Class>* provider, Signal signal, uint32_t& y)
		: FieldPresenter(parent, provider->GetName(), signal, y), provider(provider)
	{
		input->Bind(wxEVT_TEXT_ENTER, &StringPresenter<Class>::HandleChange, this);
	}

	template<class Class>
	inline StringPresenter<Class>::~StringPresenter()
	{
	}

	template<class Class>
	inline void StringPresenter<Class>::SetCurrent(GameObject* curr)
	{
		Presenter::SetCurrent(curr);

		input->SetLabel(provider->Get((Class*)curr));
	}

	template<class Class>
	inline void StringPresenter<Class>::Refresh()
	{
		SetCurrent(curr);
	}

	template<class Class>
	inline void StringPresenter<Class>::HandleChange(wxCommandEvent& e)
	{
		if (curr != nullptr) {
			provider->Set((Class*)curr, wxStr2StdStr(input->GetValue()));
			signal();
		}
	}


	// IntPresenter

	template<class Class>
	inline IntPresenter<Class>::IntPresenter(wxWindow* parent, IntProvider<Class>* provider, Signal signal, uint32_t& y)
		: FieldPresenter(parent, provider->GetName(), signal, y), provider(provider)
	{
		wxIntegerValidator<int32_t> validator(&value, wxNUM_VAL_THOUSANDS_SEPARATOR);
		if (provider->HasRange()) {
			validator.SetRange(provider->GetMin(), provider->GetMax());
		}
		input->SetValidator(validator);
		input->Bind(wxEVT_TEXT_ENTER, &IntPresenter<Class>::HandleChange, this);
	}

	template<class Class>
	inline IntPresenter<Class>::~IntPresenter()
	{
	}

	template<class Class>
	inline void IntPresenter<Class>::SetCurrent(GameObject* curr)
	{
		Presenter::SetCurrent(curr);

		input->SetLabel(wxString::Format(wxT("%d"), provider->Get((Class*)curr)));
	}

	template<class Class>
	inline void IntPresenter<Class>::Refresh()
	{
		SetCurrent(curr);
	}

	template<class Class>
	inline void IntPresenter<Class>::HandleChange(wxCommandEvent& e)
	{
		if (curr != nullptr) {
			long realValue;
			input->GetValue().ToLong(&realValue);
			provider->Set((Class*)curr, (int32_t)realValue);
			signal();
		}
	}


	// UIntPresenter

	template<class Class>
	inline UIntPresenter<Class>::UIntPresenter(wxWindow* parent, UIntProvider<Class>* provider, Signal signal, uint32_t& y)
		: FieldPresenter(parent, provider->GetName(), signal, y), provider(provider)
	{
		wxIntegerValidator<uint32_t> validator(&value, wxNUM_VAL_THOUSANDS_SEPARATOR);
		if (provider->HasRange()) {
			validator.SetRange(provider->GetMin(), provider->GetMax());
		}
		input->SetValidator(validator);
		input->Bind(wxEVT_TEXT_ENTER, &UIntPresenter<Class>::HandleChange, this);
	}

	template<class Class>
	inline UIntPresenter<Class>::~UIntPresenter()
	{
	}

	template<class Class>
	inline void UIntPresenter<Class>::SetCurrent(GameObject* curr)
	{
		Presenter::SetCurrent(curr);

		input->SetLabel(wxString::Format(wxT("%d"), provider->Get((Class*)curr)));
	}

	template<class Class>
	inline void UIntPresenter<Class>::Refresh()
	{
		SetCurrent(curr);
	}

	template<class Class>
	inline void UIntPresenter<Class>::HandleChange(wxCommandEvent& e)
	{
		if (curr != nullptr) {
			unsigned long realValue;
			input->GetValue().ToULong(&realValue);
			provider->Set((Class*)curr, (uint32_t)realValue);
			signal();
		}
	}


	// FloatPresenter

	template<class Class>
	inline FloatPresenter<Class>::FloatPresenter(wxWindow* parent, FloatProvider<Class>* provider, Signal signal, uint32_t& y)
		: FieldPresenter(parent, provider->GetName(), signal, y), provider(provider)
	{
		wxFloatingPointValidator<float> validator(3, &value, wxNUM_VAL_THOUSANDS_SEPARATOR);
		if (provider->GetType() == CommonProvider::Type::UFLOAT) {
			if (provider->HasRange()) {
				float min = provider->GetMin() > 0.f ? provider->GetMin() : 0.f;
				validator.SetRange(min, provider->GetMax());
			} else {
				validator.SetMin(0.f);
			}
		} else {
			if (provider->HasRange()) {
				validator.SetRange(provider->GetMin(), provider->GetMax());
			}
		}
		input->SetValidator(validator);
		input->Bind(wxEVT_TEXT_ENTER, &FloatPresenter<Class>::HandleChange, this);
	}

	template<class Class>
	inline FloatPresenter<Class>::~FloatPresenter()
	{
	}

	template<class Class>
	inline void FloatPresenter<Class>::SetCurrent(GameObject* curr)
	{
		Presenter::SetCurrent(curr);

		input->SetLabel(wxString::Format(wxT("%.3f"), provider->Get((Class*)curr)));
	}

	template<class Class>
	inline void FloatPresenter<Class>::Refresh()
	{
		SetCurrent(curr);
	}

	template<class Class>
	inline void FloatPresenter<Class>::HandleChange(wxCommandEvent& e)
	{
		if (curr != nullptr) {
			double realValue;
			input->GetValue().ToDouble(&realValue);
			provider->Set((Class*)curr, (float)realValue);
			signal();
		}
	}


	// BoolPresenter

	template<class Class>
	inline BoolPresenter<Class>::BoolPresenter(wxWindow* parent, BoolProvider<Class>* provider, Signal signal, uint32_t& y)
		: Presenter(parent, provider->GetName(), signal, y), provider(provider)
	{
		wxSize extent = title->GetTextExtent(provider->GetName());
		checkBox = new wxCheckBox(parent, wxID_ANY, "", wxPoint(parent->GetSize().x / 2, y), wxSize(22, 22));
		checkBox->Bind(wxEVT_CHECKBOX, &BoolPresenter<Class>::HandleChange, this);
		y += 30;
	}

	template<class Class>
	inline BoolPresenter<Class>::~BoolPresenter()
	{
		wxREMOVE(checkBox);
	}

	template<class Class>
	inline void BoolPresenter<Class>::SetCurrent(GameObject* curr)
	{
		Presenter::SetCurrent(curr);
		checkBox->SetValue(provider->Get((Class*)curr));
	}

	template<class Class>
	inline void BoolPresenter<Class>::SetDefault()
	{
		Presenter::SetDefault();
		checkBox->SetValue(false);
	}

	template<class Class>
	inline void BoolPresenter<Class>::Refresh()
	{
		SetCurrent(curr);
	}

	template<class Class>
	inline void BoolPresenter<Class>::HandleChange(wxCommandEvent& e)
	{
		if (curr != nullptr) {
			provider->Set((Class*)curr, checkBox->GetValue());
			signal();
		}
	}


	// OneWayBoolPresenter
	template<class Class>
	inline OneWayBoolPresenter<Class>::OneWayBoolPresenter(wxWindow* parent, OneWayBoolProvider<Class>* provider, Signal signal, uint32_t& y)
		: Presenter(parent, provider->GetName(), signal, y), provider(provider)
	{
		wxSize extent = title->GetTextExtent(provider->GetName());
		checkBox = new wxCheckBox(parent, wxID_ANY, "", wxPoint(parent->GetSize().x / 2, y), wxSize(22, 22));
		checkBox->Bind(wxEVT_CHECKBOX, &OneWayBoolPresenter<Class>::HandleChange, this);
		y += 30;
	}

	template<class Class>
	inline OneWayBoolPresenter<Class>::~OneWayBoolPresenter()
	{
		wxREMOVE(checkBox);
	}

	template<class Class>
	inline void OneWayBoolPresenter<Class>::SetCurrent(GameObject* curr)
	{
		Presenter::SetCurrent(curr);
		checkBox->SetValue(provider->Get((Class*)curr));
	}

	template<class Class>
	inline void OneWayBoolPresenter<Class>::SetDefault()
	{
		Presenter::SetDefault();
		checkBox->SetValue(false);
	}

	template<class Class>
	inline void OneWayBoolPresenter<Class>::Refresh()
	{
		SetCurrent(curr);
	}

	template<class Class>
	inline void OneWayBoolPresenter<Class>::HandleChange(wxCommandEvent& e)
	{
		if (curr != nullptr && checkBox->GetValue() == provider->GetWay()) {
			provider->Set((Class*)curr);
			signal();
		} else {
			checkBox->SetValue(provider->GetWay());
		}
	}

	// PointPresenter

	template<class Class>
	inline PointPresenter<Class>::PointPresenter(wxWindow* parent, PointProvider<Class>* provider, Signal signal, uint32_t& y)
		: Presenter(parent, provider->GetName(), signal, y), provider(provider)
	{
		uint32_t width = (parent->GetSize().x - 3 * border) / 2;
		y += 22;
		xField = new FloatComponentCtrl(parent, "X", wxPoint(border, y), width, redish);
		yField = new FloatComponentCtrl(parent, "Y", wxPoint(2 * border + width, y), width, blueish);
		xField->Bind(wxEVT_TEXT_ENTER, &PointPresenter<Class>::HandleChange, this);
		yField->Bind(wxEVT_TEXT_ENTER, &PointPresenter<Class>::HandleChange, this);
		y += 35;
	}

	template<class Class>
	inline PointPresenter<Class>::~PointPresenter()
	{
		wxREMOVE(xField);
		wxREMOVE(yField);
	}

	template<class Class>
	inline void PointPresenter<Class>::SetCurrent(GameObject* curr)
	{
		Presenter::SetCurrent(curr);

		Point p = provider->Get((Class*)curr);
		xField->SetValue(p.x);
		yField->SetValue(p.y);
	}

	template<class Class>
	inline void PointPresenter<Class>::SetDefault()
	{
		Presenter::SetDefault();
		xField->SetValue(0.f);
		yField->SetValue(0.f);
	}

	template<class Class>
	inline void PointPresenter<Class>::Refresh()
	{
		SetCurrent(curr);
	}

	template<class Class>
	inline void PointPresenter<Class>::HandleChange(wxCommandEvent& e)
	{
		if (curr != nullptr) {
			provider->Set((Class*)curr, Point(xField->GetValue(), yField->GetValue()));
			signal();
		}
	}


	// ColorPresenter

	template<class Class>
	inline ColorPresenter<Class>::ColorPresenter(wxWindow* parent, ColorProvider<Class>* provider, Signal signal, uint32_t& y)
		: Presenter(parent, provider->GetName(), signal, y), provider(provider)
	{
		lastColor = *wxBLACK;

		uint32_t width = (parent->GetSize().x - 4 * border) / 3;
		y += 22;
		rField = new ByteComponentCtrl(parent, "R", wxPoint(border, y), width, redish);
		gField = new ByteComponentCtrl(parent, "G", wxPoint(2 * border + width, y), width, greenish);
		bField = new ByteComponentCtrl(parent, "B", wxPoint(3 * border + 2 * width, y), width, blueish);
		rField->Bind(wxEVT_TEXT_ENTER, &ColorPresenter<Class>::HandleInputChange, this);
		gField->Bind(wxEVT_TEXT_ENTER, &ColorPresenter<Class>::HandleInputChange, this);
		bField->Bind(wxEVT_TEXT_ENTER, &ColorPresenter<Class>::HandleInputChange, this);
		y += 30;
		aField = new ByteComponentCtrl(parent, "A", wxPoint(border, y), width, greyish);
		aField->Bind(wxEVT_TEXT_ENTER, &ColorPresenter<Class>::HandleInputChange, this);
		color = new wxColourPickerCtrl(parent, wxID_ANY, *wxBLACK, wxPoint(2 * border + width, y), wxSize(2 * width + border, 24));
		color->Bind(wxEVT_COLOURPICKER_CHANGED, &ColorPresenter<Class>::HandlePickerChange, this);
		y += 35;
	}

	template<class Class>
	inline ColorPresenter<Class>::~ColorPresenter()
	{
		wxREMOVE(rField);
		wxREMOVE(gField);
		wxREMOVE(bField);
		wxREMOVE(aField);
		wxREMOVE(color);
	}

	template<class Class>
	inline void ColorPresenter<Class>::SetCurrent(GameObject* curr)
	{
		Presenter::SetCurrent(curr);

		Color c = provider->Get((Class*)curr);
		wxColour wxC(c.r, c.g, c.b);
		if (wxC != lastColor) {
			Color c = provider->Get((Class*)curr);
			rField->SetValue(c.r);
			gField->SetValue(c.g);
			bField->SetValue(c.b);
			aField->SetValue(c.a);
			color->SetColour(wxC);
			lastColor = wxC;
		}
	}

	template<class Class>
	inline void ColorPresenter<Class>::SetDefault()
	{
		Presenter::SetDefault();
		rField->SetValue(0);
		gField->SetValue(0);
		bField->SetValue(0);
		aField->SetValue(0);
		color->SetColour(*wxBLACK);
		lastColor = *wxBLACK;
	}

	template<class Class>
	inline void ColorPresenter<Class>::Refresh()
	{
		SetCurrent(curr);
	}

	template<class Class>
	inline void ColorPresenter<Class>::HandlePickerChange(wxColourPickerEvent& e)
	{
		if (curr != nullptr) {
			wxColour newWxColor = color->GetColour();
			lastColor = newWxColor;
			Color newColor(newWxColor.Red(), newWxColor.Green(), newWxColor.Blue(), newWxColor.Alpha());
			rField->SetValue(newColor.r);
			gField->SetValue(newColor.g);
			bField->SetValue(newColor.b);
			aField->SetValue(newColor.a);
			provider->Set((Class*)curr, newColor);
			signal();
		}
	}

	template<class Class>
	inline void ColorPresenter<Class>::HandleInputChange(wxCommandEvent& e)
	{
		if (curr != nullptr) {
			Color newColor(rField->GetValue(), gField->GetValue(), bField->GetValue(), aField->GetValue());
			lastColor = wxColour(newColor.r, newColor.g, newColor.b, newColor.a);
			color->SetColour(lastColor);
			provider->Set((Class*)curr, newColor);
			signal();
		}
	}


	// ImagePresenter

	template<class Class>
	inline ImagePresenter<Class>::ImagePresenter(wxWindow* parent, ImageProvider<Class>* provider, Signal signal, uint32_t& y)
		: Presenter(parent, provider->GetName(), signal, y), provider(provider)
	{
		lastPath = "";

		uint32_t width = parent->GetSize().x - 3 * border - 50;
		y += 22;
		path = new wxStaticText(parent, wxID_ANY, "", wxPoint(border, y), wxSize(width, 22), wxST_ELLIPSIZE_START);
		path->SetMaxSize(wxSize(width, 22));
		preview = new wxStaticBitmap(parent, wxID_ANY, wxNullBitmap, wxPoint(2 * border + width, y), wxSize(50, 50));
		y += 22;
		browse = new wxButton(parent, wxID_ANY, "Browse...", wxPoint(border, y), wxSize(75, 25));
		browse->Bind(wxEVT_BUTTON, &ImagePresenter<Class>::HandleChange, this);
		y += 33;
	}

	template<class Class>
	inline ImagePresenter<Class>::~ImagePresenter()
	{
		wxREMOVE(path);
		wxREMOVE(browse);
		wxREMOVE(preview);
	}

	template<class Class>
	inline void ImagePresenter<Class>::SetCurrent(GameObject* curr)
	{
    	Presenter::SetCurrent(curr);
		Image* image = provider->Get((Class*)curr);
		if (image != nullptr) {
			std::string path = image->GetPath();
			if (path != lastPath) {
				this->path->SetLabel(path);
				wxImage bitmap(path, wxBITMAP_TYPE_PNG);
				bitmap.Rescale(50, 50);
				preview->SetBitmap(bitmap);
				lastPath = path;
			}
		} else {
			this->path->SetLabel("None");
			preview->SetBitmap(wxNullBitmap);
			lastPath = "";
		}
	}

	template<class Class>
	inline void ImagePresenter<Class>::SetDefault()
	{
		Presenter::SetDefault();
		this->path->SetLabel("None");
		preview->SetBitmap(wxNullBitmap);
		lastPath = "";
	}

	template<class Class>
	inline void ImagePresenter<Class>::Refresh()
	{
		SetCurrent(curr);
	}

	template<class Class>
	inline void ImagePresenter<Class>::HandleChange(wxCommandEvent& e)
	{
		if (curr != nullptr) {
			wxFileDialog browseDialog(parent, "Browse for image", "../Assets", "",
				"Image files(*.png, *.jpg, *.jpeg, *.bmp; *.gif) | *.png; *.jpg; *.jpeg; *.bmp; *.gif", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
			
			if (browseDialog.ShowModal() == wxID_CANCEL)
				return;

			lastPath = browseDialog.GetPath();
			provider->SetPath((Class*)curr, wxStr2StdStr(lastPath), false);
			signal();
		}
	}


	// TrafoPresenter

	template<class Class>
	inline TrafoPresenter<Class>::TrafoPresenter(wxWindow* parent, TrafoProvider<Class>* provider, Signal signal, uint32_t& y)
		: TrafoPresenterBase(parent, provider->GetName(), signal, y), provider(provider)
	{
		posX->Bind(wxEVT_TEXT_ENTER, &TrafoPresenter<Class>::HandleChange, this);
		posY->Bind(wxEVT_TEXT_ENTER, &TrafoPresenter<Class>::HandleChange, this);
		scaleX->Bind(wxEVT_TEXT_ENTER, &TrafoPresenter<Class>::HandleChange, this);
		scaleY->Bind(wxEVT_TEXT_ENTER, &TrafoPresenter<Class>::HandleChange, this);
		rot->Bind(wxEVT_TEXT_ENTER, &TrafoPresenter<Class>::HandleChange, this);
	}

	template<class Class>
	inline TrafoPresenter<Class>::~TrafoPresenter()
	{
	}

	template<class Class>
	inline void TrafoPresenter<Class>::SetCurrent(GameObject* curr)
	{
		Presenter::SetCurrent(curr);

		Transform trafo = provider->Get((Class*)curr);
		posX->SetValue(trafo.pos.x);
		posY->SetValue(trafo.pos.y);
		scaleX->SetValue(trafo.scale.x);
		scaleY->SetValue(trafo.scale.y);
		rot->SetValue(trafo.rot);
	}

	template<class Class>
	inline void TrafoPresenter<Class>::Refresh()
	{
		SetCurrent(curr);
	}

	template<class Class>
	inline void TrafoPresenter<Class>::HandleChange(wxCommandEvent& e)
	{
		if (curr != nullptr) {
			Transform trafo(Point(posX->GetValue(), posY->GetValue()), Point(scaleX->GetValue(), scaleY->GetValue()), rot->GetValue());
			provider->Set((Class*)curr, trafo);
			signal();
		}
	}

}
#pragma once

#include "Presenter.hpp"
#include "Editor/Registry/ProjectData.hpp"
#include "Editor/FileUtils/RelativeFileDialog.hpp"
#include "Editor/ResourceManagement/ImageManager.hpp"
#include "Engine/ResourceManager/Font.hpp"
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
		checkBox = new wxCheckBox(parent, wxID_ANY, "", wxPoint(parent->GetSize().x / 2, y), wxSize(UI::unitHeight, UI::unitHeight));
		checkBox->Bind(wxEVT_CHECKBOX, &BoolPresenter<Class>::HandleChange, this);
		y += UI::heightPadding;
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
		checkBox = new wxCheckBox(parent, wxID_ANY, "", wxPoint(parent->GetSize().x / 2, y), wxSize(UI::unitHeight, UI::unitHeight));
		checkBox->Bind(wxEVT_CHECKBOX, &OneWayBoolPresenter<Class>::HandleChange, this);
		y += UI::heightPadding;
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
		uint32_t width = (parent->GetSize().x - 3 * UI::border) / 2;
		y += UI::unitHeight;
		xField = new FloatComponentCtrl(parent, "X", wxPoint(UI::border, y), width, redish);
		yField = new FloatComponentCtrl(parent, "Y", wxPoint(2 * UI::border + width, y), width, blueish);
		xField->Bind(wxEVT_TEXT_ENTER, &PointPresenter<Class>::HandleChange, this);
		yField->Bind(wxEVT_TEXT_ENTER, &PointPresenter<Class>::HandleChange, this);
		y += UI::heightPadding + UI::border;
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

		uint32_t width = (parent->GetSize().x - 4 * UI::border) / 3;
		y += UI::unitHeight;
		rField = new ByteComponentCtrl(parent, "R", wxPoint(UI::border, y), width, redish);
		gField = new ByteComponentCtrl(parent, "G", wxPoint(2 * UI::border + width, y), width, greenish);
		bField = new ByteComponentCtrl(parent, "B", wxPoint(3 * UI::border + 2 * width, y), width, blueish);
		rField->Bind(wxEVT_TEXT_ENTER, &ColorPresenter<Class>::HandleInputChange, this);
		gField->Bind(wxEVT_TEXT_ENTER, &ColorPresenter<Class>::HandleInputChange, this);
		bField->Bind(wxEVT_TEXT_ENTER, &ColorPresenter<Class>::HandleInputChange, this);
		y += UI::heightPadding;
		aField = new ByteComponentCtrl(parent, "A", wxPoint(UI::border, y), width, greyish);
		aField->Bind(wxEVT_TEXT_ENTER, &ColorPresenter<Class>::HandleInputChange, this);
		color = new wxColourPickerCtrl(parent, wxID_ANY, *wxBLACK, wxPoint(2 * UI::border + width, y), wxSize(2 * width + UI::border, UI::extUnitHeight));
		color->Bind(wxEVT_COLOURPICKER_CHANGED, &ColorPresenter<Class>::HandlePickerChange, this);
		y += UI::heightPadding + UI::border;
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
		if (wxC != lastColor || wxC == *wxBLACK) {
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

		uint32_t width = parent->GetSize().x - 3 * UI::border - previewHeight;
		y += UI::unitHeight;
		path = new wxStaticText(parent, wxID_ANY, "", wxPoint(UI::border, y), wxSize(width, UI::unitHeight), wxST_ELLIPSIZE_START);
		path->SetMaxSize(wxSize(width, UI::unitHeight));
		preview = new wxStaticBitmap(parent, wxID_ANY, wxNullBitmap, wxPoint(2 * UI::border + width, y), wxSize(previewHeight, previewHeight));
		y += UI::unitHeight;
		browse = new wxButton(parent, wxID_ANY, "Browse...", wxPoint(UI::border, y), UI::smallButtonSize);
		browse->Bind(wxEVT_BUTTON, &ImagePresenter<Class>::HandleChange, this);
		y += UI::heightPadding;
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
				wxImage bitmap = ImageManager::GetImage(image->GetFullPath());
				if (!bitmap.IsOk()) {
					SPHE_ERROR("The image is not found or corrupted");
					provider->Set((Class*)curr, nullptr);
					signal();
					return;
				}
				this->path->SetLabel(path);
				bitmap.Rescale(previewHeight, previewHeight);
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
			RelativeOpenDialog browseDialog(parent, "Browse for image", ProjectData::GetPath() + "Assets\\",
				"Image files(*.png, *.jpg, *.jpeg, *.bmp; *.gif) | *.png; *.jpg; *.jpeg; *.bmp; *.gif");
			
			if (!browseDialog.Show())
				return;

			provider->SetByPath((Class*)curr, wxStr2StdStr(browseDialog.GetPath()), false);
			signal();
		}
	}


	// FontPresenter

	template<class Class>
	inline FontPresenter<Class>::FontPresenter(wxWindow* parent, FontProvider<Class>* provider, Signal signal, uint32_t& y)
		: Presenter(parent, provider->GetName(), signal, y), provider(provider)
	{
		uint32_t width = parent->GetSize().x - 3 * UI::border - UI::unitHeight;
		y += UI::unitHeight;
		fontPicker = new wxComboBox(parent, wxID_ANY, "", wxPoint(UI::border, y), wxSize(width, UI::unitHeight));
		fontPicker->Bind(wxEVT_COMBOBOX, &FontPresenter<Class>::HandleChange, this);
		fontPicker->SetEditable(false);
		addButton = new wxButton(parent, wxID_ANY, "+", wxPoint(2 * UI::border + width, y), wxSize(UI::unitHeight, UI::unitHeight));
		addButton->Bind(wxEVT_BUTTON, &FontPresenter<Class>::OnAdd, this);
		y += UI::heightPadding;
	}

	template<class Class>
	inline FontPresenter<Class>::~FontPresenter()
	{
		wxREMOVE(fontPicker);
		wxREMOVE(addButton);
	}

	template<class Class>
	inline void FontPresenter<Class>::SetCurrent(GameObject* curr)
	{
		Presenter::SetCurrent(curr);

		fontPicker->Clear();
		auto fontTable = provider->GetAllFonts();
		for (auto it = fontTable->begin(); it != fontTable->end(); ++it) {
			fontPicker->Append((*it).first);
		}
		
		Font* font = provider->Get((Class*)curr);
		if (font != nullptr) {
			fontPicker->SetValue(font->GetName());
		}
		else {
			fontPicker->SetValue("None");
		}
	}

	template<class Class>
	inline void FontPresenter<Class>::SetDefault()
	{
		fontPicker->Clear();
		fontPicker->SetValue("");
	}

	template<class Class>
	inline void FontPresenter<Class>::Refresh()
	{
		SetCurrent(curr);
	}

	template<class Class>
	inline void FontPresenter<Class>::HandleChange(wxCommandEvent& e)
	{
		if (curr != nullptr) {
			provider->SetByName((Class*)curr, wxStr2StdStr(fontPicker->GetValue()));
			signal();
		}
	}

	template<class Class>
	inline void FontPresenter<Class>::OnAdd(wxCommandEvent& e)
	{
		if (curr != nullptr) {
			RelativeOpenDialog browseDialog(parent, "Add new Font", ProjectData::GetPath() + "Assets\\Fonts\\",
				"Font file(*.fnt) | *.fnt");

			if (!browseDialog.Show())
				return;

			wxString fontFile = "Fonts\\" + browseDialog.GetPath();
			wxString imgFile = fontFile.Left(fontFile.find_last_of('.')) + ".png";
			provider->SetByPath((Class*)curr, wxStr2StdStr(fontFile), wxStr2StdStr(imgFile));
			signal();
		}
	}


	// FontStylePresenter

	template<class Class>
	inline FontStylePresenter<Class>::FontStylePresenter(wxWindow* parent, FontStyleProvider<Class>* provider, Signal signal, uint32_t& y)
		: Presenter(parent, provider->GetName(), signal, y), provider(provider)
	{
		uint32_t width = parent->GetSize().x - 3 * UI::border - UI::unitHeight;
		y += UI::unitHeight;
		boldLabel = new wxStaticText(parent, wxID_ANY, "Bold", wxPoint(UI::border, y));
		boldCheckBox = new wxCheckBox(parent, wxID_ANY, "", wxPoint(parent->GetSize().x / 2, y - UI::shift), wxSize(UI::unitHeight, UI::unitHeight));
		boldCheckBox->Bind(wxEVT_CHECKBOX, &FontStylePresenter<Class>::HandleChange, this);
		y += UI::unitHeight + UI::shift;
		italicLabel = new wxStaticText(parent, wxID_ANY, "Italic", wxPoint(UI::border, y));
		italicCheckBox = new wxCheckBox(parent, wxID_ANY, "", wxPoint(parent->GetSize().x / 2, y - UI::shift), wxSize(UI::unitHeight, UI::unitHeight));
		italicCheckBox->Bind(wxEVT_CHECKBOX, &FontStylePresenter<Class>::HandleChange, this);
		y += UI::unitHeight + UI::shift;
		underlinedLabel = new wxStaticText(parent, wxID_ANY, "Underlined", wxPoint(UI::border, y));
		underlinedCheckBox = new wxCheckBox(parent, wxID_ANY, "", wxPoint(parent->GetSize().x / 2, y - UI::shift), wxSize(UI::unitHeight, UI::unitHeight));
		underlinedCheckBox->Bind(wxEVT_CHECKBOX, &FontStylePresenter<Class>::HandleChange, this);
		y += UI::heightPadding;
	}

	template<class Class>
	inline FontStylePresenter<Class>::~FontStylePresenter()
	{
		wxREMOVE(boldLabel);
		wxREMOVE(boldCheckBox);
		wxREMOVE(italicLabel);
		wxREMOVE(italicCheckBox);
		wxREMOVE(underlinedLabel);
		wxREMOVE(underlinedCheckBox);
	}

	template<class Class>
	inline void FontStylePresenter<Class>::SetCurrent(GameObject* curr)
	{
		Presenter::SetCurrent(curr);

		uint8_t style = provider->Get((Class*)curr);
		boldCheckBox->SetValue(style & SPH_FONT_BOLD);
		italicCheckBox->SetValue(style & SPH_FONT_ITALIC);
		underlinedCheckBox->SetValue(style & SPH_FONT_UNDERLINED);
	}

	template<class Class>
	inline void FontStylePresenter<Class>::SetDefault()
	{
		boldCheckBox->SetValue(false);
		italicCheckBox->SetValue(false);
		underlinedCheckBox->SetValue(false);
	}

	template<class Class>
	inline void FontStylePresenter<Class>::Refresh()
	{
		SetCurrent(curr);
	}

	template<class Class>
	inline void FontStylePresenter<Class>::HandleChange(wxCommandEvent& e)
	{
		if (curr != nullptr) {
			uint8_t style = 0;
			if (boldCheckBox->GetValue()) {
				style = style | SPH_FONT_BOLD;
			}
			if (italicCheckBox->GetValue()) {
				style = style | SPH_FONT_ITALIC;
			}
			if (underlinedCheckBox->GetValue()) {
				style = style | SPH_FONT_UNDERLINED;
			}

			provider->Set((Class*)curr, style);
			signal();
		}
	}


	// AnimationPresenter

	template<class Class>
	inline AnimationPresenter<Class>::AnimationPresenter(wxWindow* parent, AnimationProvider<Class>* provider, Signal signal, uint32_t& y)
		: Presenter(parent, provider->GetName(), signal, y), provider(provider)
	{
		lastPath = "";

		uint32_t width = parent->GetSize().x - 3 * UI::border - previewHeight;
		y += UI::unitHeight;
		name = new wxStaticText(parent, wxID_ANY, "", wxPoint(UI::border, y), wxSize(width, UI::unitHeight), wxST_ELLIPSIZE_START);
		name->SetMaxSize(wxSize(width, UI::unitHeight));
		preview = new wxStaticBitmap(parent, wxID_ANY, wxNullBitmap, wxPoint(2 * UI::border + width, y), wxSize(previewHeight, previewHeight));
		y += UI::unitHeight;
		browse = new wxButton(parent, wxID_ANY, "Browse...", wxPoint(UI::border, y), UI::smallButtonSize);
		browse->Bind(wxEVT_BUTTON, &AnimationPresenter<Class>::HandleChange, this);
		y += 33;
	}

	template<class Class>
	inline AnimationPresenter<Class>::~AnimationPresenter()
	{
		wxREMOVE(name);
		wxREMOVE(browse);
		wxREMOVE(preview);
	}

	template<class Class>
	inline void AnimationPresenter<Class>::SetCurrent(GameObject* curr)
	{
		Presenter::SetCurrent(curr);
		Animation* anim = provider->Get((Class*)curr);
		if (anim != nullptr) {
			std::string path = anim->GetPath();
			if (path != lastPath) {
				wxImage bitmap = ImageManager::GetImage(anim->GetAtlas()->GetFullPath());
				if (!bitmap.IsOk()) {
					SPHE_ERROR("The image is not found or corrupted");
					provider->Set((Class*)curr, nullptr);
					signal();
					return;
				}
				name->SetLabel(anim->GetName());
				uint32_t startFrame = anim->GetStartFrame(), frameCols = anim->GetFrameCols();
				uint32_t frameWidth = anim->GetFrameWidth(), frameHeight = anim->GetFrameHeight();
				uint32_t currCol = startFrame % frameCols, currRow = startFrame / frameCols;
				bitmap.Resize(bitmap.GetSize(), wxPoint(-1 * frameWidth * currCol, -1 * frameHeight * currRow));
				bitmap.Resize(wxSize(frameWidth, frameHeight), wxPoint());
				bitmap.Rescale(50, 50);
				preview->SetBitmap(bitmap);
				lastPath = path;
			}
		}
		else {
			name->SetLabel("");
			preview->SetBitmap(wxNullBitmap);
			lastPath = "";
		}
	}

	template<class Class>
	inline void AnimationPresenter<Class>::SetDefault()
	{
		Presenter::SetDefault();
		name->SetLabel("");
		preview->SetBitmap(wxNullBitmap);
		lastPath = "";
	}

	template<class Class>
	inline void AnimationPresenter<Class>::Refresh()
	{
		SetCurrent(curr);
	}

	template<class Class>
	inline void AnimationPresenter<Class>::HandleChange(wxCommandEvent& e)
	{
		if (curr != nullptr) {
			RelativeOpenDialog browseDialog(parent, "Browse for animation", ProjectData::GetPath() + "Assets\\",
				"Sharpheus animation files(*.anim.sharpheus) | *.anim.sharpheus");

			if (!browseDialog.Show())
				return;

			provider->SetByPath((Class*)curr, wxStr2StdStr(browseDialog.GetPath()));
			signal();
		}
	}


	// SoundPresenter

	template<class Class>
	inline SoundPresenter<Class>::SoundPresenter(wxWindow* parent, SoundProvider<Class>* provider, Signal signal, uint32_t& y)
		: Presenter(parent, provider->GetName(), signal, y), provider(provider)
	{
		uint32_t width = parent->GetSize().x - 3 * UI::border - UI::smallButtonSize.x;
		y += UI::unitHeight;
		path = new wxStaticText(parent, wxID_ANY, "", wxPoint(UI::border, y), wxSize(width, UI::unitHeight), wxST_ELLIPSIZE_START);
		path->SetMaxSize(wxSize(width, UI::unitHeight));
		browse = new wxButton(parent, wxID_ANY, "Browse...", wxPoint(2 * UI::border + width, y - 3), UI::smallButtonSize);
		browse->Bind(wxEVT_BUTTON, &SoundPresenter<Class>::HandleChange, this);
		y += UI::heightPadding;
	}

	template<class Class>
	inline SoundPresenter<Class>::~SoundPresenter()
	{
		wxREMOVE(path);
		wxREMOVE(browse);
	}

	template<class Class>
	inline void SoundPresenter<Class>::SetCurrent(GameObject* curr)
	{
		Presenter::SetCurrent(curr);
		path->SetLabel(provider->Get((Class*)curr));
	}

	template<class Class>
	inline void SoundPresenter<Class>::SetDefault()
	{
		Presenter::SetDefault();
		this->path->SetLabel("");
	}

	template<class Class>
	inline void SoundPresenter<Class>::Refresh()
	{
		SetCurrent(curr);
	}

	template<class Class>
	inline void SoundPresenter<Class>::HandleChange(wxCommandEvent& e)
	{
		if (curr != nullptr) {
			RelativeOpenDialog browseDialog(parent, "Browse for sound file", ProjectData::GetPath() + "Assets\\",
				"Wave file(*.wav) | *.wav");

			if (!browseDialog.Show())
				return;

			provider->Set((Class*)curr, wxStr2StdStr(browseDialog.GetPath()));
			signal();
		}
	}


	// StringListPresenter

	template<class Class>
	inline StringListPresenter<Class>::StringListPresenter(wxWindow* parent, StringListProvider<Class>* provider, Signal signal, uint32_t& y)
		: Presenter(parent, provider->GetName(), signal, y), provider(provider)
	{
		uint32_t width = parent->GetSize().x - 2 * UI::border;
		y += UI::unitHeight;

		list = new wxListView(parent, wxID_ANY, wxPoint(UI::border, y), wxSize(width, 100), wxLC_REPORT | wxLC_HRULES | wxLC_SINGLE_SEL);
		list->Bind(wxEVT_LEFT_DOWN, &StringListPresenter<Class>::OnListClicked, this);
		list->Bind(wxEVT_LIST_ITEM_SELECTED, &StringListPresenter<Class>::OnSelectionChanged, this);
		secondColStart = width - 50;
		list->AppendColumn("Name", wxLIST_FORMAT_CENTRE, secondColStart);
		list->AppendColumn("", wxLIST_FORMAT_LEFT, width - secondColStart);
		wxImageList* imgs = new wxImageList(20, 20);
		imgs->Add(ImageManager::GetImage("delete.png", ImageManager::PathType::DETAILSPANEL));
		list->AssignImageList(imgs, wxIMAGE_LIST_SMALL);
		y += 100 + UI::border;

		addButton = new wxButton(parent, wxID_ANY, "+ Add new", wxPoint(UI::border, y), UI::smallButtonSize);
		addButton->Bind(wxEVT_BUTTON, &StringListPresenter<Class>::OnAdd, this);
		y += UI::heightPadding;
	}

	template<class Class>
	inline StringListPresenter<Class>::~StringListPresenter()
	{
		wxREMOVE(list);
		wxREMOVE(addButton);
	}

	template<class Class>
	inline void StringListPresenter<Class>::SetCurrent(GameObject* curr)
	{
		Presenter::SetCurrent(curr);

		list->DeleteAllItems();
		uint32_t strCount = provider->GetCount((Class*)curr);
		for (uint32_t i = 0; i < strCount; ++i) {
			long ind = list->InsertItem(i, provider->GetString((Class*)curr, i), -1);
			list->SetItem(ind, 1, "", 0);
		}

		uint32_t selected = provider->GetCurr((Class*)curr);
		if (selected < strCount) {
			list->Select(selected);
		}
	}

	template<class Class>
	inline void StringListPresenter<Class>::SetDefault()
	{
		Presenter::SetDefault();
	}

	template<class Class>
	inline void StringListPresenter<Class>::Refresh()
	{
		SetCurrent(curr);
	}

	template<class Class>
	inline void StringListPresenter<Class>::OnAdd(wxCommandEvent& e)
	{
		if (curr != nullptr) {
			RelativeOpenDialog browseDialog(parent, "Browse for animation", ProjectData::GetPath() + "Assets\\",
				"Sharpheus animation files(*.anim.sharpheus) | *.anim.sharpheus");

			if (!browseDialog.Show())
				return;

			provider->AddString((Class*)curr, wxStr2StdStr(browseDialog.GetPath()));
			signal();
		}
	}

	template<class Class>
	inline void StringListPresenter<Class>::OnSelectionChanged(wxListEvent& e)
	{
		if (curr != nullptr) {
			long selected = list->GetFirstSelected();
			if (selected >= 0 && (uint32_t)selected < provider->GetCount((Class*)curr)) {
				if (deleteSelected) {
					provider->RemoveString((Class*)curr, selected);
					deleteSelected = false;
					signal();
				}
				else if (selected != provider->GetCurr((Class*)curr)) {
					provider->SetCurr((Class*)curr, selected);
				}
			}
		}
	}

	template<class Class>
	inline void StringListPresenter<Class>::OnListClicked(wxMouseEvent& e)
	{
		if (wxGetMousePosition().x - list->GetScreenPosition().x > (int)secondColStart) {
			deleteSelected = true;
		}
		e.Skip();
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
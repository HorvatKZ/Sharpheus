#pragma once

#include "editor_pch.h"
#include "InputComponentCtrl.hpp"
#include <wx/clrpicker.h>
#include <wx/listctrl.h>

#define wxREMOVE(item) if (item != nullptr) item->Destroy();


namespace Sharpheus {

	typedef std::function<void()>& Signal;
	typedef std::function<void(const std::string&, const std::string&)>& NameSignal;

	class Presenter
	{
	public:
		Presenter(wxWindow* parent, const std::string& title, Signal signal, uint32_t& y);
		virtual ~Presenter();

		virtual void SetCurrent(GameObject* curr) { this->curr = curr; }
		virtual void SetDefault() { curr = nullptr; }
		virtual void Refresh() = 0;

	protected:
		wxWindow* parent;
		GameObject* curr = nullptr;
		wxStaticText* title;
		Signal signal;

		static wxColour redish;
		static wxColour greenish;
		static wxColour blueish;
		static wxColour greyish;
	};


	class FieldPresenter : public Presenter
	{
	public:
		FieldPresenter(wxWindow* parent, const std::string& title, Signal signal, uint32_t& y);
		virtual ~FieldPresenter();

		virtual inline void SetDefault() override { Presenter::SetDefault(); input->SetLabel(""); }

	protected:
		wxTextCtrl* input;
	};


	class NamePresenter : public FieldPresenter
	{
	public:
		NamePresenter(wxWindow* parent, const std::string& title, Signal signal, NameSignal nameSignal, uint32_t& y);
		virtual ~NamePresenter();

		virtual void SetCurrent(GameObject* curr) override;
		virtual void Refresh() override;

	protected:
		NameSignal nameSignal;

		virtual void HandleChange(wxCommandEvent& e);
	};


	template <class Class>
	class StringPresenter : public FieldPresenter
	{
	public:
		StringPresenter(wxWindow* parent, StringProvider<Class>* provider, Signal signal, uint32_t& y);
		virtual ~StringPresenter();

		virtual void SetCurrent(GameObject* curr) override;
		virtual void Refresh() override;

	protected:
		StringProvider<Class>* provider;

		virtual void HandleChange(wxCommandEvent& e);
	};


	template <class Class>
	class IntPresenter : public FieldPresenter
	{
	public:
		IntPresenter(wxWindow* parent, IntProvider<Class>* provider, Signal signal, uint32_t& y);
		virtual ~IntPresenter();

		virtual void SetCurrent(GameObject* curr) override;
		virtual void Refresh() override;

	protected:
		IntProvider<Class>* provider;
		int32_t value;

		virtual void HandleChange(wxCommandEvent& e);
	};


	template <class Class>
	class UIntPresenter : public FieldPresenter
	{
	public:
		UIntPresenter(wxWindow* parent, UIntProvider<Class>* provider, Signal signal, uint32_t& y);
		virtual ~UIntPresenter();

		virtual void SetCurrent(GameObject* curr) override;
		virtual void Refresh() override;

	protected:
		UIntProvider<Class>* provider;
		uint32_t value;

		virtual void HandleChange(wxCommandEvent& e);
	};


	template <class Class>
	class FloatPresenter : public FieldPresenter
	{
	public:
		FloatPresenter(wxWindow* parent, FloatProvider<Class>* provider, Signal signal, uint32_t& y);
		virtual ~FloatPresenter();

		virtual void SetCurrent(GameObject* curr) override;
		virtual void Refresh() override;

	protected:
		FloatProvider<Class>* provider;
		float value;

		virtual void HandleChange(wxCommandEvent& e);
	};


	template <class Class>
	class BoolPresenter : public Presenter
	{
	public:
		BoolPresenter(wxWindow* parent, BoolProvider<Class>* provider, Signal signal, uint32_t& y);
		virtual ~BoolPresenter();

		virtual void SetCurrent(GameObject* curr) override;
		virtual inline void SetDefault() override;
		virtual void Refresh() override;

	protected:
		BoolProvider<Class>* provider;
		wxCheckBox* checkBox;

		virtual void HandleChange(wxCommandEvent& e);
	};


	template <class Class>
	class OneWayBoolPresenter : public Presenter
	{
	public:
		OneWayBoolPresenter(wxWindow* parent, OneWayBoolProvider<Class>* provider, Signal signal, uint32_t& y);
		virtual ~OneWayBoolPresenter();

		virtual void SetCurrent(GameObject* curr) override;
		virtual inline void SetDefault() override;
		virtual void Refresh() override;

	protected:
		OneWayBoolProvider<Class>* provider;
		wxCheckBox* checkBox;

		virtual void HandleChange(wxCommandEvent& e);
	};


	template <class Class>
	class PointPresenter : public Presenter
	{
	public:
		PointPresenter(wxWindow* parent, PointProvider<Class>* provider, Signal signal, uint32_t& y);
		virtual ~PointPresenter();

		virtual void SetCurrent(GameObject* curr) override;
		virtual inline void SetDefault() override;
		virtual void Refresh() override;

	protected:
		PointProvider<Class>* provider;
		FloatComponentCtrl* xField;
		FloatComponentCtrl* yField;

		virtual void HandleChange(wxCommandEvent& e);
	};


	template <class Class>
	class ColorPresenter : public Presenter
	{
	public:
		ColorPresenter(wxWindow* parent, ColorProvider<Class>* provider, Signal signal, uint32_t& y);
		virtual ~ColorPresenter();

		virtual void SetCurrent(GameObject* curr) override;
		virtual inline void SetDefault() override;
		virtual void Refresh() override;

	protected:
		ColorProvider<Class>* provider;
		ByteComponentCtrl* rField;
		ByteComponentCtrl* gField;
		ByteComponentCtrl* bField;
		ByteComponentCtrl* aField;
		wxColourPickerCtrl* color;
		wxColour lastColor;

		virtual void HandlePickerChange(wxColourPickerEvent& e);
		virtual void HandleInputChange(wxCommandEvent& e);
	};


	template <class Class>
	class ImagePresenter : public Presenter
	{
	public:
		ImagePresenter(wxWindow* parent, ImageProvider<Class>* provider, Signal signal, uint32_t& y);
		virtual ~ImagePresenter();

		virtual void SetCurrent(GameObject* curr) override;
		virtual inline void SetDefault() override;
		virtual void Refresh() override;

	protected:
		ImageProvider<Class>* provider;
		wxStaticText* path;
		wxStaticBitmap* preview;
		wxButton* browse;
		wxString lastPath;

		virtual void HandleChange(wxCommandEvent& e);
	};

	template <class Class>
	class FontPresenter : public Presenter
	{
	public:
		FontPresenter(wxWindow* parent, FontProvider<Class>* provider, Signal signal, uint32_t& y);
		virtual ~FontPresenter();

		virtual void SetCurrent(GameObject* curr) override;
		virtual inline void SetDefault() override;
		virtual void Refresh() override;

	protected:
		FontProvider<Class>* provider;
		wxComboBox* fontPicker;
		wxButton* addButton;

		virtual void HandleChange(wxCommandEvent& e);
		virtual void OnAdd(wxCommandEvent& e);
	};


	template <class Class>
	class AnimationPresenter : public Presenter
	{
	public:
		AnimationPresenter(wxWindow* parent, AnimationProvider<Class>* provider, Signal signal, uint32_t& y);
		virtual ~AnimationPresenter();

		virtual void SetCurrent(GameObject* curr) override;
		virtual inline void SetDefault() override;
		virtual void Refresh() override;

	protected:
		AnimationProvider<Class>* provider;
		wxStaticText* name;
		wxStaticBitmap* preview;
		wxButton* browse;
		wxString lastPath;

		virtual void HandleChange(wxCommandEvent& e);
	};


	template <class Class>
	class StringListPresenter : public Presenter
	{
	public:
		StringListPresenter(wxWindow* parent, StringListProvider<Class>* provider, Signal signal, uint32_t& y);
		virtual ~StringListPresenter();

		virtual void SetCurrent(GameObject* curr) override;
		virtual inline void SetDefault() override;
		virtual void Refresh() override;

	protected:
		StringListProvider<Class>* provider;
		wxListView* list;
		wxButton* addButton;
		uint32_t secondColStart;
		bool deleteSelected = false;

		virtual void OnAdd(wxCommandEvent& e);
		virtual void OnSelectionChanged(wxListEvent& e);
		virtual void OnListClicked(wxMouseEvent& e);
	};


	class TrafoPresenterBase : public Presenter
	{
	public:
		TrafoPresenterBase(wxWindow* parent, const std::string& title, Signal signal, uint32_t& y);
		virtual ~TrafoPresenterBase();

		virtual inline void SetDefault() override {
			Presenter::SetDefault();
			posX->SetValue(0);
			posY->SetValue(0);
			scaleX->SetValue(0);
			scaleY->SetValue(0);
			rot->SetValue(0);
		}

	protected:
		wxStaticBitmap* posImg;
		wxStaticBitmap* scaleImg;
		wxStaticBitmap* rotImg;

		FloatComponentCtrl* posX;
		FloatComponentCtrl* posY;
		FloatComponentCtrl* scaleX;
		FloatComponentCtrl* scaleY;
		AngleComponentCtrl* rot;

		wxBitmap posBitmap, scaleBitmap, rotBitmap;
		virtual void InitBitmaps();
	};


	class MainTrafoPresenter : public TrafoPresenterBase
	{
	public:
		MainTrafoPresenter(wxWindow* parent, const std::string& title, Signal signal, uint32_t& y);
		virtual ~MainTrafoPresenter();

		void SetCurrent(GameObject* curr) override;
		inline void SetDefault() override {
			TrafoPresenterBase::SetDefault();
			trafoTypeSwitch->SetBitmap(localBitmap);
			isWorldTrafo = false;
		}
		virtual void Refresh() override;

	private:
		wxBitmapButton* trafoTypeSwitch;

		bool isWorldTrafo = false;
		wxBitmap worldBitmap, localBitmap;
		void InitBitmaps() override;

		void TypeSwitchPressed(wxCommandEvent& e);
		virtual void HandleChange(wxCommandEvent& e);
	};


	template <class Class>
	class TrafoPresenter : public TrafoPresenterBase
	{
	public:
		TrafoPresenter(wxWindow* parent, TrafoProvider<Class>* provider, Signal signal, uint32_t& y);
		virtual ~TrafoPresenter();

		void SetCurrent(GameObject* curr) override;
		virtual void Refresh() override;

	private:
		TrafoProvider<Class>* provider;

		virtual void HandleChange(wxCommandEvent& e);
	};

}
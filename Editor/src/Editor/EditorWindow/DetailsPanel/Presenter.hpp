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
		Presenter(wxWindow* parent, const std::string& title, Signal signal, uint32& y);
		virtual ~Presenter();

		virtual void SetCurrent(GameObject* curr) { this->curr = curr; }
		virtual void SetDefault() { curr = nullptr; }
		void Refresh() { SetCurrent(curr); }

	protected:
		wxWindow* parent;
		GameObject* curr = nullptr;
		wxStaticText* title;
		Signal signal;

		static const uint32 previewHeight;
		static const wxColour redish;
		static const wxColour greenish;
		static const wxColour blueish;
		static const wxColour greyish;
	};


	class FieldPresenter : public Presenter
	{
	public:
		FieldPresenter(wxWindow* parent, const std::string& title, Signal signal, uint32& y);
		virtual ~FieldPresenter();

		virtual inline void SetDefault() override { Presenter::SetDefault(); input->SetLabel(""); }

	protected:
		wxTextCtrl* input;
	};


	class HeaderPresenter : public FieldPresenter
	{
	public:
		HeaderPresenter(wxWindow* parent, const std::string& title, Signal signal, NameSignal nameSignal, uint32& y);
		virtual ~HeaderPresenter();

		virtual void SetCurrent(GameObject* curr) override;

		virtual inline void SetDefault() override { FieldPresenter::SetDefault(); visibilityButton->SetBitmap(wxNullBitmap); }

	protected:
		wxBitmapButton* visibilityButton;

		NameSignal nameSignal;
		wxBitmap visibleBitmap, invisibleBitmap;

		void InitBitmaps();

		virtual void OnNameChanged(wxCommandEvent& e);
		virtual void OnVisibilityChanged(wxCommandEvent& e);
	};


	template <class Class>
	class StringPresenter : public FieldPresenter
	{
	public:
		StringPresenter(wxWindow* parent, StringProvider<Class>* provider, Signal signal, uint32& y);
		virtual ~StringPresenter();

		virtual void SetCurrent(GameObject* curr) override;

	protected:
		StringProvider<Class>* provider;

		virtual void HandleChange(wxCommandEvent& e);
	};


	template <class Class>
	class IntPresenter : public FieldPresenter
	{
	public:
		IntPresenter(wxWindow* parent, IntProvider<Class>* provider, Signal signal, uint32& y);
		virtual ~IntPresenter();

		virtual void SetCurrent(GameObject* curr) override;

	protected:
		IntProvider<Class>* provider;
		int32 value;

		virtual void HandleChange(wxCommandEvent& e);
	};


	template <class Class>
	class UIntPresenter : public FieldPresenter
	{
	public:
		UIntPresenter(wxWindow* parent, UIntProvider<Class>* provider, Signal signal, uint32& y);
		virtual ~UIntPresenter();

		virtual void SetCurrent(GameObject* curr) override;

	protected:
		UIntProvider<Class>* provider;
		uint32 value;

		virtual void HandleChange(wxCommandEvent& e);
	};


	template <class Class>
	class FloatPresenter : public FieldPresenter
	{
	public:
		FloatPresenter(wxWindow* parent, FloatProvider<Class>* provider, Signal signal, uint32& y);
		virtual ~FloatPresenter();

		virtual void SetCurrent(GameObject* curr) override;

	protected:
		FloatProvider<Class>* provider;
		float value;

		virtual void HandleChange(wxCommandEvent& e);
	};


	template <class Class>
	class BoolPresenter : public Presenter
	{
	public:
		BoolPresenter(wxWindow* parent, BoolProvider<Class>* provider, Signal signal, uint32& y);
		virtual ~BoolPresenter();

		virtual void SetCurrent(GameObject* curr) override;
		virtual inline void SetDefault() override;

	protected:
		BoolProvider<Class>* provider;
		wxCheckBox* checkBox;

		virtual void HandleChange(wxCommandEvent& e);
	};


	template <class Class>
	class OneWayBoolPresenter : public Presenter
	{
	public:
		OneWayBoolPresenter(wxWindow* parent, OneWayBoolProvider<Class>* provider, Signal signal, uint32& y);
		virtual ~OneWayBoolPresenter();

		virtual void SetCurrent(GameObject* curr) override;
		virtual inline void SetDefault() override;

	protected:
		OneWayBoolProvider<Class>* provider;
		wxCheckBox* checkBox;

		virtual void HandleChange(wxCommandEvent& e);
	};


	template <class Class>
	class PointPresenter : public Presenter
	{
	public:
		PointPresenter(wxWindow* parent, PointProvider<Class>* provider, Signal signal, uint32& y);
		virtual ~PointPresenter();

		virtual void SetCurrent(GameObject* curr) override;
		virtual inline void SetDefault() override;

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
		ColorPresenter(wxWindow* parent, ColorProvider<Class>* provider, Signal signal, uint32& y);
		virtual ~ColorPresenter();

		virtual void SetCurrent(GameObject* curr) override;
		virtual inline void SetDefault() override;

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
		ImagePresenter(wxWindow* parent, ImageProvider<Class>* provider, Signal signal, uint32& y);
		virtual ~ImagePresenter();

		virtual void SetCurrent(GameObject* curr) override;
		virtual inline void SetDefault() override;

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
		FontPresenter(wxWindow* parent, FontProvider<Class>* provider, Signal signal, uint32& y);
		virtual ~FontPresenter();

		virtual void SetCurrent(GameObject* curr) override;
		virtual inline void SetDefault() override;

	protected:
		FontProvider<Class>* provider;
		wxComboBox* fontPicker;
		wxButton* addButton;

		virtual void HandleChange(wxCommandEvent& e);
		virtual void OnAdd(wxCommandEvent& e);
	};


	template <class Class>
	class FontStylePresenter : public Presenter
	{
	public:
		FontStylePresenter(wxWindow* parent, FontStyleProvider<Class>* provider, Signal signal, uint32& y);
		virtual ~FontStylePresenter();

		virtual void SetCurrent(GameObject* curr) override;
		virtual inline void SetDefault() override;

	protected:
		FontStyleProvider<Class>* provider;
		wxStaticText* boldLabel, * italicLabel, * underlinedLabel;
		wxCheckBox* boldCheckBox, * italicCheckBox, * underlinedCheckBox;

		virtual void HandleChange(wxCommandEvent& e);
	};


	template <class Class>
	class AnimationPresenter : public Presenter
	{
	public:
		AnimationPresenter(wxWindow* parent, AnimationProvider<Class>* provider, Signal signal, uint32& y);
		virtual ~AnimationPresenter();

		virtual void SetCurrent(GameObject* curr) override;
		virtual inline void SetDefault() override;

	protected:
		AnimationProvider<Class>* provider;
		wxStaticText* name;
		wxStaticBitmap* preview;
		wxButton* browse;
		wxString lastPath;

		virtual void HandleChange(wxCommandEvent& e);
	};


	template <class Class>
	class TileSetPresenter : public Presenter
	{
	public:
		TileSetPresenter(wxWindow* parent, TileSetProvider<Class>* provider, Signal signal, uint32& y);
		virtual ~TileSetPresenter();

		virtual void SetCurrent(GameObject* curr) override;
		virtual inline void SetDefault() override;

	protected:
		TileSetProvider<Class>* provider;
		wxStaticText* name;
		wxStaticBitmap* preview;
		wxButton* browse;
		wxString lastPath;

		virtual void HandleChange(wxCommandEvent& e);
	};


	template <class Class>
	class AudioPresenter : public Presenter
	{
	public:
		AudioPresenter(wxWindow* parent, AudioProvider<Class>* provider, Signal signal, uint32& y);
		virtual ~AudioPresenter();

		virtual void SetCurrent(GameObject* curr) override;
		virtual inline void SetDefault() override;

	protected:
		AudioProvider<Class>* provider;
		wxStaticText* path;
		wxButton* browse;

		virtual void HandleChange(wxCommandEvent& e);
	};

	template <class Class>
	class ScriptPresenter : public Presenter
	{
	public:
		ScriptPresenter(wxWindow* parent, ScriptProvider<Class>* provider, Signal signal, Signal moduleChangedSignal, uint32& y);
		virtual ~ScriptPresenter();

		virtual void SetCurrent(GameObject* curr) override;
		virtual inline void SetDefault() override;

	protected:
		ScriptProvider<Class>* provider;
		wxStaticText* path;
		wxButton* browse;
		wxButton* add;
		Signal moduleChangedSignal;

		virtual void Browse(wxCommandEvent& e);
		virtual void CreatePyBehavior(wxCommandEvent& e);
	};


	template <class Class>
	class StringListPresenter : public Presenter
	{
	public:
		StringListPresenter(wxWindow* parent, StringListProvider<Class>* provider, Signal signal, uint32& y);
		virtual ~StringListPresenter();

		virtual void SetCurrent(GameObject* curr) override;
		virtual inline void SetDefault() override;

	protected:
		StringListProvider<Class>* provider;
		wxListView* list;
		wxButton* addButton;
		uint32 secondColStart;
		bool deleteSelected = false;

		virtual void OnAdd(wxCommandEvent& e);
		virtual void OnSelectionChanged(wxListEvent& e);
		virtual void OnListClicked(wxMouseEvent& e);
	};


	class TrafoPresenterBase : public Presenter
	{
	public:
		TrafoPresenterBase(wxWindow* parent, const std::string& title, Signal signal, uint32& y);
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
		MainTrafoPresenter(wxWindow* parent, const std::string& title, Signal signal, uint32& y);
		virtual ~MainTrafoPresenter();

		void SetCurrent(GameObject* curr) override;
		inline void SetDefault() override {
			TrafoPresenterBase::SetDefault();
			trafoTypeSwitch->SetBitmap(localBitmap);
			isWorldTrafo = false;
		}

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
		TrafoPresenter(wxWindow* parent, TrafoProvider<Class>* provider, Signal signal, uint32& y);
		virtual ~TrafoPresenter();

		void SetCurrent(GameObject* curr) override;

	private:
		TrafoProvider<Class>* provider;

		virtual void HandleChange(wxCommandEvent& e);
	};


	class TileMapPresenter : public Presenter
	{
	public:
		TileMapPresenter(wxWindow* parent, const std::string& title, Signal signal, uint32& y);
		virtual ~TileMapPresenter();

		void SetCurrent(GameObject* curr) override;

	private:
		wxButton* openButton;

		virtual void HandleChange(wxCommandEvent& e);
	};


	template <class Class>
	class LayerPresenter : public Presenter
	{
	public:
		LayerPresenter(wxWindow* parent, LayerProvider<Class>* provider, Signal signal, uint32& y);
		virtual ~LayerPresenter();

		void SetCurrent(GameObject* curr) override;

	protected:
		wxComboBox* layerSelector = nullptr;
		wxBitmapButton* layerEditorButton;

		LayerProvider<Class>* provider;

		virtual void HandleChange(wxCommandEvent& e);

		void OnLayerEditor(wxCommandEvent& e);
	};
}
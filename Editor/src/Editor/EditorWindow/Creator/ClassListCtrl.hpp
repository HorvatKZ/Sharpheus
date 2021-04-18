#pragma once

#include "editor_pch.h"
#include <wx/treectrl.h>
#include <wx/combo.h>


namespace Sharpheus {

	class ClassListCtrl : public wxTreeCtrl, public wxComboPopup
	{
		struct Info {
			Info (const std::string& name, const std::string& path, GameObject::Type type, uint32_t level)
				: name(name), path(path), type(type), level(level) {}

			std::string name, path;
			GameObject::Type type;
			uint32_t level;
		};

	public:
		ClassListCtrl();
		virtual ~ClassListCtrl();

		virtual void Init() override;
		virtual void InitContent(std::function<void(const wxString&, GameObject::Type)>&& selectCallback);
		virtual bool Create(wxWindow* parent) override;
		virtual wxWindow* GetControl() override;
		virtual wxString GetStringValue() const override;

	private:
		std::unordered_map<wxString, GameObject::Type> nameToType;
		std::function<void(const wxString&, GameObject::Type)> selectCallback;

		static wxColour inativeColour;
		static wxFont inactiveFont;
		static std::vector<Info> infos;

		void OnSelect(wxTreeEvent& e);

		void Fill();
	};

}
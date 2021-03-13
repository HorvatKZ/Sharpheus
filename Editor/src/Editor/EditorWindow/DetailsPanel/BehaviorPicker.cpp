#include "editor_pch.h"
#include "BehaviorPicker.hpp"
#include "BehaviorCreator.hpp"
#include "CreateBehaviorDialog.hpp"
#include "Editor/Registry/ProjectData.hpp"
#include "Editor/FileUtils/ClassWriter.hpp"
#include "Editor/FileUtils/RelativeFileDialog.hpp"
#include <wx/dir.h>


namespace Sharpheus {

	BehaviorPicker::BehaviorPicker(wxWindow* parent, const std::string& title, std::function<void(uint32_t)>& mainSignal, Signal signal, uint32_t& y)
		: Presenter(parent, title, signal, y), mainSignal(mainSignal), y(y)
	{
		y += 22;
		wxSize extent = this->title->GetTextExtent(title);
		uint32_t parentWidth = parent->GetSize().x;
		typeSelector = new wxComboBox(parent, wxID_ANY, "", wxPoint(UI::border, y), wxSize(parentWidth - 3 * UI::border - 22, 22));
		createNewTypeButton = new wxButton(parent, wxID_ANY, "+", wxPoint(parentWidth - 22 - UI::border, y), wxSize(22, 22));
		createNewTypeButton->Bind(wxEVT_BUTTON, &BehaviorPicker::CreateBehavior, this);
		y += 30;
	}

	BehaviorPicker::~BehaviorPicker()
	{
		wxREMOVE(typeSelector);
		wxREMOVE(createNewTypeButton);
	}

	void BehaviorPicker::SetCurrent(GameObject* curr)
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
		typeSelector = new wxComboBox(parent, wxID_ANY, "", wxPoint(UI::border, y + 22), wxSize(parentWidth - 3 * UI::border - 22, 22), arr);
		typeSelector->SetEditable(false);
		typeSelector->Bind(wxEVT_COMBOBOX, &BehaviorPicker::HandleChange, this);
	}

	void BehaviorPicker::Refresh()
	{
		SetCurrent(curr);
	}

	void BehaviorPicker::HandleChange(wxCommandEvent& e)
	{
		wxString value = typeSelector->GetValue();
		unsigned long subType;
		value.Left(value.find(' ')).ToULong(&subType);
		mainSignal(subType);
	}

	void BehaviorPicker::CreateBehavior(wxCommandEvent& e)
	{
		CreateBehaviorDialog dialog(parent->GetParent(), ProjectData::GetPath());

		if (dialog.ShowModal() == wxID_CANCEL) {
			return;
		}

		if (!IsClassNameFree(dialog.GetName())) {
			SPHE_ERROR("The given classname is already occupied. Be aware, this check is not case-sesitive");
			return;
		}

		bool unique = true;
		wxString newClass = dialog.GetName();
		newClass.MakeUpper();
		for (auto it = BehaviorCreator::behaviorNames.begin(); it != BehaviorCreator::behaviorNames.end() && unique; ++it) {
			wxString oldClass = (*it).second;
			oldClass.MakeUpper();
			if (oldClass == newClass) {
				SPHE_ERROR("The given classname is already occupied");
				return;
			}
		}

		CreateFiles(dialog.GetName(), dialog.GetParentName(), dialog.GetFolder(), dialog.GetID());
		RegenerateProject();
		
		int response = wxMessageBox("Do you want to save the current level before exiting?", "Warning", wxICON_WARNING | wxYES | wxNO | wxCENTRE);
		if (response == wxYES) {
			SaveLevel();
		}

		wxExit();
	}


	bool BehaviorPicker::IsClassNameFree(const wxString& className)
	{
		wxString newClass = className;
		newClass.MakeUpper();
		for (auto it = BehaviorCreator::behaviorNames.begin(); it != BehaviorCreator::behaviorNames.end(); ++it) {
			wxString oldClass = (*it).second;
			oldClass.MakeUpper();
			if (oldClass == newClass) {
				return false;
			}
		}

		return true;
	}


	void BehaviorPicker::CreateFiles(const wxString& className, const wxString& parentClassName, const wxString& folder, uint32_t ID)
	{
		wxString hppPath = folder + className + ".hpp";
		wxString cppPath = folder + className + ".cpp";
		hppPath.Replace("\\", "/");
		cppPath.Replace("\\", "/");
		ClassWriter::CreateBehaviorHeader(ProjectData::GetPath() + hppPath, className, parentClassName, ID);
		ClassWriter::CreateBehaviorSource(ProjectData::GetPath() + cppPath, className + ".hpp", className, parentClassName);
		ClassWriter::UpdateCreateBehaviorHeader(ProjectData::GetPath() + "Source\\BehaviorCreator.hpp", hppPath.Mid(7), className, ID, BehaviorCreator::behaviorNames);
		ClassWriter::UpdateCreateBehaviorSource(ProjectData::GetPath() + "Source\\BehaviorCreator.cpp", className, ID, BehaviorCreator::behaviorNames);
	}


	void BehaviorPicker::RegenerateProject()
	{
		bool found;
		wxString name;
		wxDir dir(ProjectData::GetPath() + "Solution");
		found = dir.GetFirst(&name);
		while (found && name.Left(11) != "GenSolution") {
			found = dir.GetNext(&name);
		}

		if (name.Length() < 11 || name.Left(11) != "GenSolution") {
			SPHE_WARN("The new files are created, but the solution generator cannot be located.\nPlease regenerate the solution manually, and then edit the new file.\nYou need to rebuild the solution before using the new Behavior.");
		}
		else {
			wxExecute(ProjectData::GetPath() + "Solution\\" + name, wxEXEC_SYNC);
			wxMessageBox("The new files are created. Now you can edit them.\nYou need to rebuild the solution before using the new Behavior.", "Behavior generation");
		}
	}


	void BehaviorPicker::SaveLevel()
	{
		if (ProjectData::GetLevel()->HasPath()) {
			bool success = ProjectData::GetLevel()->Save();
			SPHE_ASSERT(success, "Cannot save level. Check the log files for more information");
		}
		else {
			RelativeSaveDialog saveDialog(parent, "Save Level", ProjectData::GetPath() + "Levels\\", "Sharpheus level file(*.lvl.sharpheus) | *.lvl.sharpheus");

			if (!saveDialog.Show())
				return;

			bool success = ProjectData::GetProj()->SaveLevel(wxStr2StdStr(saveDialog.GetPath()));
			SPHE_ASSERT(success, "Cannot save level. Check the log files for more information");
		}
	}

}
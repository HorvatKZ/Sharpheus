#pragma once

namespace Sharpheus {

	class Exporter
	{
	public:
		Exporter(bool includeLogging);
		virtual ~Exporter();

		void Export();

	private:
		bool includeLogging;

		bool CopyFolderContent(const wxString& folder);
		bool CopyFolderContent(class wxDir& from, const wxString& toUnder);
		bool CopyItem(const wxString& from, const wxString& to);
		bool PrecompilePyScripts();
		bool PrecompilePyScripts(class wxDir& pyFolder, const wxString& pycFolder);
		bool DeleteItems(const std::vector<wxString> items);
	};

}
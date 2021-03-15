#pragma once

namespace Sharpheus {

	class Exporter
	{
	public:
		Exporter();
		virtual ~Exporter();

	private:
		void Export();

		bool CopyFolderContent(const wxString& folder);
		bool CopyFolderContent(class wxDir& from, const wxString& toUnder);
		bool CopyItem(const wxString& from, const wxString& to);
	};

}
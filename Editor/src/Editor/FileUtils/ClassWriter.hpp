#pragma once

#include <wx/textfile.h>


namespace Sharpheus {

	class ClassWriter
	{
	public:
		ClassWriter(const wxString& filePath, const wxString& className, const wxString& parent, bool header, const wxString& comment = "");
		virtual ~ClassWriter();

		static void CreateBehaviorHeader(const wxString& filePath, const wxString& className, const wxString& parent, uint32 subType);
		static void CreateBehaviorSource(const wxString& filePath, const wxString& headerInclude, const wxString& className, const wxString& parent, uint32 subType);
		static void UpdateCreateBehaviorHeader(const wxString& filePath, const wxString& newClassPath, const wxString& newClassName, uint32 newSubType, const std::unordered_map<uint32, std::string>& oldClasses);
		static void UpdateCreateBehaviorSource(const wxString& filePath, const wxString& newClassName, uint32 newSubType, const std::unordered_map<uint32, std::string>& oldClasses);

		void Include(const wxString& file);
		void IncludeCompilerPath(const wxString& file);
		void WriteLine(const wxString& line);
		void WriteLine(uint32 tabs, const wxString& line);
		void WriteEmptyLine();
		void WriteEmptyLines(uint32 number);

		// Header
		void StartClass(const wxString& inharitanceMode = "public");
		void EndClass();
		void WriteVisibility(const wxString& visibility);
		void WriteConstructorDecl(const wxString& params = "");
		void WriteDestructorDecl();
		void WriteFunctionDecl(const wxString& retType, const wxString& name, const wxString& params = "", const wxString& modifiers = "", bool isOverride = false, bool constCallable = false);

		// Source
		void UsingNamespace(const wxString& nameSpace);
		void StartConstructorDef(const wxString& params = "");
		void StartConstructorDefWithParent(const wxString& params = "", const wxString& parentParams = "");
		void StartDestructorDef();
		void StartFunctionDef(const wxString& retType, const wxString& name, const wxString& params = "", bool constCallable = false);
		void EndFunctionDef();

	private:
		wxTextFile f;
		wxString className, parent;
	};

}
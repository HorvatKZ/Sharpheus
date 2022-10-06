#pragma once

#include <wx/textfile.h>


namespace Sharpheus {

	class ClassWriter
	{
	public:
		ClassWriter(const wxString& filePath, const wxString& className, const wxString& parent);
		virtual ~ClassWriter();

		inline void SetCommentsMode(bool commentMode) { this->commentMode = commentMode; }

		void WriteLine(const wxString& line);
		void WriteLine(uint32 tabs, const wxString& line);
		void WriteEmptyLine();
		void WriteEmptyLines(uint32 number);

	protected:
		wxTextFile f;
		wxString className, parent;
		bool commentMode = false;

		virtual wxString GetCommentChars() = 0;
	};


	class CppClassWriter : public ClassWriter
	{
	public:
		CppClassWriter(const wxString& filePath, const wxString& className, const wxString& parent, bool header, const wxString& comment = "");
		virtual ~CppClassWriter();

		static void CreateBehaviorHeader(const wxString& filePath, const wxString& className, const wxString& parent, uint32 subType);
		static void CreateBehaviorSource(const wxString& filePath, const wxString& headerInclude, const wxString& className, const wxString& parent, uint32 subType);
		static void UpdateCreateBehaviorHeader(const wxString& filePath, const wxString& newClassPath, const wxString& newClassName, uint32 newSubType, const std::unordered_map<uint32, std::string>& oldClasses);
		static void UpdateCreateBehaviorSource(const wxString& filePath, const wxString& newClassName, uint32 newSubType, const std::unordered_map<uint32, std::string>& oldClasses);

		void Include(const wxString& file);
		void IncludeCompilerPath(const wxString& file);

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

	protected:
		wxString GetCommentChars() override { return "//"; }
	};


	class PyClassWriter : public ClassWriter
	{
	public:
		PyClassWriter(const wxString& filePath, const wxString& className, const wxString& parent);
		virtual ~PyClassWriter();

		static void CreatePyBehaviorFile(const wxString& filePath, const wxString& className);

		void Import(const wxString& module, const wxString& from = "", const wxString& alias = "");

		void StartClass();
		void StartFunction(const wxString& name, const wxString& params = "", const wxString& retType = "");
		void StartFreeFunction(const wxString& name, const wxString& params = "", const wxString& retType = "");

	protected:
		wxString GetCommentChars() override { return "#"; }
	};

}
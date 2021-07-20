#include "editor_pch.h"
#include "ClassWriter.hpp"


namespace Sharpheus {

	ClassWriter::ClassWriter(const wxString& filePath, const wxString& className, const wxString& parent, bool header, const wxString& comment)
		: className(className), parent(parent)
	{
		if (!f.Create(filePath)) {
			f.Open(filePath);
			f.Clear();
		}

		if (!comment.empty()) {
			WriteLine("// " + comment);
		}

		if (header) {
			WriteLine("#pragma once");
			WriteEmptyLine();
		}
	}


	ClassWriter::~ClassWriter()
	{
		f.Write();
	}


	void ClassWriter::CreateBehaviorHeader(const wxString& filePath, const wxString& className, const wxString& parent, uint32 subType)
	{
		ClassWriter cw(filePath, className, parent, true);
		if (parent.Length() > 11 && parent.Left(11) == "Sharpheus::") {
			cw.Include(parent.Mid(11) + ".hpp");
		}
		else {
			cw.Include(parent + ".hpp");
		}
		cw.WriteEmptyLines(2);

		cw.StartClass();
		cw.WriteVisibility("public");
		cw.WriteConstructorDecl("Sharpheus::Behavior* other");
		cw.WriteConstructorDecl("Sharpheus::GameObject* parent, const std::string& name");
		cw.WriteDestructorDecl();
		cw.WriteEmptyLine();
		cw.WriteFunctionDecl("bool", "Load", "Sharpheus::FileLoader& fl", "virtual", true);
		cw.WriteEmptyLine();
		cw.WriteLine(1, "SPH_DECL_BEHAVIOR(" + className + ", " + wxString::Format("%d", subType) + ");");
		cw.WriteEmptyLine();

		cw.WriteVisibility("protected");
		cw.WriteFunctionDecl("bool", "Save", "Sharpheus::FileSaver& fs", "virtual", true);
		cw.WriteEmptyLine();
		cw.WriteFunctionDecl("void", "Tick", "float deltaTime", "virtual", true);
		cw.WriteEmptyLine();
		cw.WriteFunctionDecl("void", "CopyContent", "GameObject* other", "virtual", true);
		cw.EndClass();
		cw.WriteEmptyLine();
	}


	void ClassWriter::CreateBehaviorSource(const wxString& filePath, const wxString& headerInclude, const wxString& className, const wxString& parent, uint32 subType)
	{
		ClassWriter cw(filePath, className, parent, false);
		cw.Include("pch.h");
		cw.Include(headerInclude);
		cw.WriteEmptyLines(2);

		cw.UsingNamespace("Sharpheus");
		cw.WriteEmptyLines(2);

		cw.WriteLine("uint32 " + className + "::classVersion = 1;");
		cw.WriteEmptyLine();

		cw.WriteLine("SPH_START_CLASSINFO(" + className + ", \"behavior.png\")");
		cw.WriteLine(1, "// Provide necessary data members here for the Editor to present them");
		cw.WriteLine("SPH_END_CLASSINFO");
		cw.WriteEmptyLines(2);

		cw.StartConstructorDefWithParent("Sharpheus::Behavior* other", "other");
		cw.EndFunctionDef();
		cw.WriteEmptyLines(2);

		cw.StartConstructorDefWithParent("Sharpheus::GameObject* parent, const std::string& name", "parent, name");
		cw.EndFunctionDef();
		cw.WriteEmptyLines(2);

		cw.StartDestructorDef();
		cw.EndFunctionDef();
		cw.WriteEmptyLines(2);

		cw.StartFunctionDef("bool", "Load", "FileLoader& fl");
		cw.WriteLine(1, parent + "::Load(fl);");
		cw.WriteLine(1, "SPH_CHECK_CLASSVERSION(fl, classVersion);");
		cw.WriteLine(1, "// Read data members here");
		cw.WriteLine(1, "return fl.GetStatus();");
		cw.EndFunctionDef();
		cw.WriteEmptyLines(2);

		cw.StartFunctionDef("bool", "Save", "FileSaver& fs");
		cw.WriteLine(1, parent + "::Save(fs);");
		cw.WriteLine(1, "fs.Write(classVersion);");
		cw.WriteLine(1, "// Write data members here");
		cw.WriteLine(1, "return fs.GetStatus();");
		cw.EndFunctionDef();
		cw.WriteEmptyLines(2);

		cw.StartFunctionDef("void", "Tick", "float deltaTime");
		cw.WriteLine(1, "// Called once per tick");
		cw.EndFunctionDef();
		cw.WriteEmptyLines(2);

		cw.StartFunctionDef("void", "CopyContent", "GameObject* other");
		cw.WriteLine(1, parent + "::CopyContent(other);");
		cw.WriteEmptyLine();
		cw.WriteLine(1, className + "* trueOther = (" + className + "*)other;");
		cw.WriteLine(1, "// Copy data members here");
		cw.EndFunctionDef();
		cw.WriteEmptyLines(2);

		cw.StartFunctionDef("void", "Init");
		cw.WriteLine(1, "// Do subscribing here");
		cw.EndFunctionDef();
		cw.WriteEmptyLines(2);

		cw.StartFunctionDef("bool", "IsCompatibleWithParent", "GameObject* parent");
		cw.WriteLine(1, "return parent != nullptr;");
		cw.EndFunctionDef();
		cw.WriteEmptyLine();
	}


	void ClassWriter::UpdateCreateBehaviorHeader(const wxString& filePath, const wxString& newClassPath, const wxString& newClassName, uint32 newSubType, std::unordered_map<uint32, std::string>& oldClasses)
	{
		uint32 ind = 9;
		wxTextFile file;
		file.Open(filePath);
		wxArrayString behaviorIncludes;
		wxString curr = file.GetLine(ind++);
		while (!curr.empty()) {
			behaviorIncludes.Add(curr);
			curr = file.GetLine(ind++);
		}
		file.Close();

		ClassWriter cw(filePath, "BehaviorCreator", "", true, "This file is generated by Sharpheus. Do not edit!");
		cw.Include("pch.h");
		cw.Include("Behavior.hpp");
		cw.Include("PlaceholderBehavior.hpp");
		cw.Include("CustomMacros.h");
		cw.WriteEmptyLine();
		cw.WriteLine("// Custom behaviors");
		for (const wxString& behavInc : behaviorIncludes) {
			cw.WriteLine(behavInc);
		}
		cw.Include(newClassPath);
		cw.WriteEmptyLine();

		cw.WriteLine("#define SPH_PRESENT_BEHAVIOR(obj) \\");
		cw.WriteLine(1, "SPH_START_PRESENTING(obj) \\");
		cw.WriteLine(2, "SPH_PRESENT(PlaceholderBehavior, 0) \\");
		for (auto it = oldClasses.begin(); it != oldClasses.end(); ++it) {
			cw.WriteLine(2, "SPH_PRESENT(" + (*it).second + ", " + wxString::Format("%d", (*it).first) + ") \\");
		}
		cw.WriteLine(2, "SPH_PRESENT(" + newClassName + ", " + wxString::Format("%d", newSubType) + ") \\");
		cw.WriteLine(1, "SPH_END_PRESENTING() \\");
		cw.WriteEmptyLines(2);

		cw.StartClass();
		cw.WriteVisibility("public");
		cw.WriteFunctionDecl("Sharpheus::Behavior*", "Create", "uint32 subType, Sharpheus::PlaceholderBehavior* other", "static");
		cw.WriteFunctionDecl("Sharpheus::Behavior*", "Create", "uint32 subType, Sharpheus::GameObject* parent, const std::string& name", "static");
		cw.WriteEmptyLine();
		cw.WriteFunctionDecl("bool", "IsCompatibleWithParent", "uint32 subType, Sharpheus::GameObject* parent", "static");
		cw.WriteEmptyLine();
		cw.WriteLine(1, "static std::unordered_map<uint32, std::string> behaviorNames;");
		cw.EndClass();
		cw.WriteEmptyLine();
	}


	void ClassWriter::UpdateCreateBehaviorSource(const wxString& filePath, const wxString& newClassName, uint32 newSubType, std::unordered_map<uint32, std::string>& oldClasses)
	{
		ClassWriter cw(filePath, "BehaviorCreator", "", false, "This file is generated by Sharpheus. Do not edit!");
		cw.Include("pch.h");
		cw.Include("BehaviorCreator.hpp");
		cw.WriteEmptyLines(2);

		cw.WriteLine("std::unordered_map<uint32, std::string> BehaviorCreator::behaviorNames({");
		for (auto it = oldClasses.begin(); it != oldClasses.end(); ++it) {
			cw.WriteLine(1, "{" + wxString::Format("%d", (*it).first) + ", \"" + (*it).second + "\"},");
		}
		cw.WriteLine(1, "{" + wxString::Format("%d", newSubType) + ", \"" + newClassName + "\"}");
		cw.WriteLine("});");
		cw.WriteEmptyLines(2);

		cw.StartFunctionDef("Sharpheus::Behavior*", "Create", "uint32 subType, Sharpheus::PlaceholderBehavior* other");
		cw.WriteLine(1, "SPH_START_BEHAVIORS(subType)");
		for (auto it = oldClasses.begin(); it != oldClasses.end(); ++it) {
			cw.WriteLine(2, "SPH_CREATE_BEHAVIOR_1(" + (*it).second + ", " + wxString::Format("%d", (*it).first) + ", other)");
		}
		cw.WriteLine(2, "SPH_CREATE_BEHAVIOR_1(" + newClassName + ", " + wxString::Format("%d", newSubType) + ", other)");
		cw.WriteLine(1, "SPH_END_BEHAVIOR_CREATION(subType)");
		cw.EndFunctionDef();
		cw.WriteEmptyLines(2);

		cw.StartFunctionDef("Sharpheus::Behavior*", "Create", "uint32 subType, Sharpheus::GameObject* parent, const std::string& name");
		cw.WriteLine(1, "SPH_START_BEHAVIORS(subType)");
		cw.WriteLine(2, "SPH_CREATE_BEHAVIOR_2(Sharpheus::PlaceholderBehavior, 0, parent, name)");
		for (auto it = oldClasses.begin(); it != oldClasses.end(); ++it) {
			cw.WriteLine(2, "SPH_CREATE_BEHAVIOR_2(" + (*it).second + ", " + wxString::Format("%d", (*it).first) + ", parent, name)");
		}
		cw.WriteLine(2, "SPH_CREATE_BEHAVIOR_2(" + newClassName + ", " + wxString::Format("%d", newSubType) + ", parent, name)");
		cw.WriteLine(1, "SPH_END_BEHAVIOR_CREATION(subType)");
		cw.EndFunctionDef();
		cw.WriteEmptyLines(2);

		cw.StartFunctionDef("bool", "IsCompatibleWithParent", "uint32 subType, Sharpheus::GameObject* parent");
		cw.WriteLine(1, "SPH_START_BEHAVIORS(subType)");
		cw.WriteLine(2, "SPH_IS_BEHAVIOR_COMP(Sharpheus::PlaceholderBehavior, 0, parent)");
		for (auto it = oldClasses.begin(); it != oldClasses.end(); ++it) {
			cw.WriteLine(2, "SPH_IS_BEHAVIOR_COMP(" + (*it).second + ", " + wxString::Format("%d", (*it).first) + ", parent)");
		}
		cw.WriteLine(2, "SPH_IS_BEHAVIOR_COMP(" + newClassName + ", " + wxString::Format("%d", newSubType) + ", parent)");
		cw.WriteLine(1, "SPH_END_BEHAVIOR_COMP(subType)");
		cw.EndFunctionDef();
		cw.WriteEmptyLine();
	}


	void ClassWriter::Include(const wxString& file)
	{
		WriteLine("#include \"" + file + "\"");
	}


	void ClassWriter::IncludeCompilerPath(const wxString& file)
	{
		WriteLine("#include <" + file + ">");
	}


	void ClassWriter::WriteLine(uint32 tabs, const wxString& line)
	{
		wxString tabSting('\t', tabs);
		f.AddLine(tabSting + line);
	}


	void ClassWriter::WriteLine(const wxString& line)
	{
		f.AddLine(line);
	}


	void ClassWriter::WriteEmptyLine()
	{
		f.AddLine("");
	}


	void ClassWriter::WriteEmptyLines(uint32 number)
	{
		for (uint32 i = 0; i < number; ++i) {
			WriteEmptyLine();
		}
	}


	void ClassWriter::StartClass(const wxString& inharitanceMode)
	{
		wxString line = "class SPH_EXPORT " + className;
		if (!parent.empty()) {
			line += " : " + inharitanceMode + " " + parent;
		}
		WriteLine(line);
		WriteLine("{");
	}


	void ClassWriter::EndClass()
	{
		WriteLine("};");
	}
	

	void ClassWriter::WriteVisibility(const wxString& visibility)
	{
		WriteLine(visibility + ":");
	}


	void ClassWriter::WriteConstructorDecl(const wxString& params)
	{
		WriteLine(1, className + "(" + params + ");");
	}


	void ClassWriter::WriteDestructorDecl()
	{
		WriteLine(1, "virtual ~" + className + "();");
	}


	void ClassWriter::WriteFunctionDecl(const wxString& retType, const wxString& name, const wxString& params, const wxString& modifiers, bool isOverride, bool constCallable)
	{
		wxString line = modifiers;
		if (!line.empty()) {
			line += " ";
		}

		line += retType + " " + name + "(" + params + ")";

		if (constCallable) {
			line += " const";
		}

		if (isOverride) {
			line += " override";
		}

		WriteLine(1, line + ";");
	}


	void ClassWriter::UsingNamespace(const wxString& nameSpace)
	{
		WriteLine("using namespace " + nameSpace + ";");
	}


	void ClassWriter::StartConstructorDef(const wxString& params)
	{
		WriteLine(className + "::" + className + "(" + params + ")");
		WriteLine("{");
	}


	void ClassWriter::StartConstructorDefWithParent(const wxString& params, const wxString& parentParams)
	{
		WriteLine(className + "::" + className + "(" + params + ")");
		WriteLine(1, ": " + parent + "(" + parentParams + ")");
		WriteLine("{");
	}


	void ClassWriter::StartDestructorDef()
	{
		WriteLine(className + "::~" + className + "()");
		WriteLine("{");
	}


	void ClassWriter::StartFunctionDef(const wxString& retType, const wxString& name, const wxString& params, bool constCallable)
	{
		wxString line = retType + " " + className + "::" + name + "(" + params + ")";

		if (constCallable) {
			line += " const";
		}

		WriteLine(line);
		WriteLine("{");
	}


	void ClassWriter::EndFunctionDef()
	{
		WriteLine("}");
	}

}
#pragma once


// Logs
#define SPHE_LOG(...) ::Sharpheus::Logger::GetEditorLogger()->trace(__VA_ARGS__)
#define SPHE_INFO(...) ::Sharpheus::Logger::GetEditorLogger()->info(__VA_ARGS__)
#define SPHE_WARN(msg, ...) ::Sharpheus::Logger::GetEditorLogger()->warn(msg, __VA_ARGS__); wxMessageBox(msg, "Warning", wxICON_WARNING | wxOK | wxCENTER);
#define SPHE_ERROR(msg, ...) ::Sharpheus::Logger::GetEditorLogger()->error(msg, __VA_ARGS__); wxMessageBox(msg, "Error!", wxICON_ERROR | wxOK | wxCENTER);
#define SPHE_FATAL(msg, ...) ::Sharpheus::Logger::GetEditorLogger()->critical(msg, __VA_ARGS__); wxMessageBox(msg, "!!!FATAL ERROR!!!", wxICON_ERROR | wxOK | wxCENTER);
#define SPHE_ASSERT(cond, ...) { if (!(cond)) { SPHE_ERROR(__VA_ARGS__); } }
#pragma once

// Logs
#define SPHE_LOG(...) ::Sharpheus::Logger::GetEditorLogger()->trace(__VA_ARGS__)
#define SPHE_INFO(...) ::Sharpheus::Logger::GetEditorLogger()->info(__VA_ARGS__)
#define SPHE_WARN(...) ::Sharpheus::Logger::GetEditorLogger()->warn(__VA_ARGS__)
#define SPHE_ERROR(...) ::Sharpheus::Logger::GetEditorLogger()->error(__VA_ARGS__)
#define SPHE_FATAL(...) ::Sharpheus::Logger::GetEditorLogger()->critical(__VA_ARGS__)
#define SPHE_ASSERT(cond, ...) { if (!(cond)) SPHE_ERROR(__VA_ARGS__); }
#include "pch.h"
#include "Logger.hpp"
#include <spdlog/sinks/rotating_file_sink.h>
#include "Engine/FileUnits/OSPaths.hpp"


namespace Sharpheus {

	std::shared_ptr<spdlog::logger> Logger::engineLogger = nullptr;
	std::shared_ptr<spdlog::logger> Logger::editorLogger = nullptr;


	void Logger::Init()
	{
		spdlog::set_pattern("%^[%D %T] %n %l: %v%$");

		std::string logFolder = OSPaths::Get(OSPaths::Folder::APPDATA_ROAMING) + "\\Sharpheus\\logs\\";

		engineLogger = spdlog::rotating_logger_mt("ENGINE", logFolder + "engine.log", 50000, 3);
		engineLogger->set_level(spdlog::level::trace);

		editorLogger = spdlog::rotating_logger_mt("EDITOR", logFolder + "editor.log", 50000, 3);
		editorLogger->set_level(spdlog::level::trace);

		SPH_INFO("Loggers initialized!");
	}


	void Logger::Clear()
	{
	}

}
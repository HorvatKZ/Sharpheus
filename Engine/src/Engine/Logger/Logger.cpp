#include "pch.h"
#include "Logger.hpp"
#include <spdlog/sinks/rotating_file_sink.h>


namespace Sharpheus {

	std::shared_ptr<spdlog::logger> Logger::engineLogger = nullptr;
	std::shared_ptr<spdlog::logger> Logger::editorLogger = nullptr;


	void Logger::Init()
	{
		spdlog::set_pattern("%^[%T] %n %l: %v%$");

		engineLogger = spdlog::rotating_logger_mt("ENGINE", "../logs/engine.log", 5000000, 3);
		engineLogger->set_level(spdlog::level::trace);

		editorLogger = spdlog::rotating_logger_mt("EDITOR", "../logs/editor.log", 5000000, 3);
		editorLogger->set_level(spdlog::level::trace);

		SPH_INFO("Loggers initialized!");
	}


	void Logger::Clear()
	{
	}

}
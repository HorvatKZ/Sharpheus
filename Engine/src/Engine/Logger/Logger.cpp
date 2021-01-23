#include "pch.h"
#include "Logger.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>


namespace Sharpheus {

	std::shared_ptr<spdlog::logger> Logger::engineLogger = nullptr;
	std::shared_ptr<spdlog::logger> Logger::editorLogger = nullptr;


	void Logger::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		engineLogger = spdlog::stdout_color_mt("ENGINE");
		engineLogger->set_level(spdlog::level::trace);

		editorLogger = spdlog::stdout_color_mt("EDITOR");
		editorLogger->set_level(spdlog::level::trace);
	}


	void Logger::Clear()
	{
	}

}
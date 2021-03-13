include "premake_include.lua"

workspace "%{ProjectName}"
	architecture "x64"
	startproject "Editor"

	configurations
	{
		"Debug",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}"
bindir = "%{wks.location}/../bin/" .. outputdir
bintempdir = "%{wks.location}/../bin-temp/" .. outputdir .. "/%{prj.name}"

IncludeDir = {}
IncludeDir["GLFW"] = "%{CommonSource}/external/GLFW/include"
IncludeDir["GLEW"] = "%{CommonSource}/external/GLEW/include"
IncludeDir["glm"] = "%{CommonSource}/Engine/external/glm"
IncludeDir["stb_image"] = "%{CommonSource}/Engine/external/stb_image"
IncludeDir["spdlog"] = "%{CommonSource}/external/spdlog/include"
IncludeDir["wxWidgets"] = "%{CommonSource}/external/wxWidgets/include"
IncludeDir["wxWidgets_win"] = "%{CommonSource}/external/wxWidgets/include/msvc"

LibDir = {}
LibDir["wxWidgets_win"] = "%{CommonSource}/external/wxWidgets/lib/vc_x64_dll/"


group "Dependencies"
	include "GLFW"
	include "GLEW"
	include "spdlog"
group ""


include "Engine"
include "Editor"
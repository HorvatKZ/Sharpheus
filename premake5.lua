workspace "Sharpheus"
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

	configuration "vs*"
        buildoptions { "/bigobj" } 

outputdir = "%{cfg.buildcfg}"
bindir = "%{wks.location}/bin/" .. outputdir
bintempdir = "%{wks.location}/bin-temp/" .. outputdir .. "/%{prj.name}"


IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/external/GLFW/include"
IncludeDir["GLEW"] = "%{wks.location}/external/GLEW/include"
IncludeDir["glm"] = "%{wks.location}/Engine/external/glm"
IncludeDir["stb_image"] = "%{wks.location}/Engine/external/stb_image"
IncludeDir["spdlog"] = "%{wks.location}/external/spdlog/include"
IncludeDir["wxWidgets"] = "%{wks.location}/external/wxWidgets/include"
IncludeDir["wxWidgets_win"] = "%{wks.location}/external/wxWidgets/include/msvc"

LibDir = {}
LibDir["wxWidgets_win"] = "%{wks.location}/external/wxWidgets/lib/vc_x64_dll/"


group "Dependencies"
	include "external/GLFW"
	include "external/GLEW"
	include "external/spdlog"
group ""


include "Engine"
include "Editor"
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

outputdir = "%{cfg.buildcfg}"
bindir = "%{wks.location}/bin/" .. outputdir
bintempdir = "%{wks.location}/bin-temp/" .. outputdir .. "/%{prj.name}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/external/GLFW/include"
IncludeDir["GLEW"] = "%{wks.location}/external/GLEW/include"
IncludeDir["glm"] = "%{wks.location}/Engine/external/glm"
IncludeDir["stb_image"] = "%{wks.location}/Engine/external/stb_image"
IncludeDir["spdlog"] = "%{wks.location}/external/spdlog/include"


group "Dependencies"
	include "external/GLFW"
	include "external/GLEW"
	include "external/spdlog"
group ""


include "Engine"
include "Editor"
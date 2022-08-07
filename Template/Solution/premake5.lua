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
	
	configuration "vs*"
		buildoptions { "/bigobj" } 

outputdir = "%{cfg.buildcfg}"
bindir = "%{wks.location}/../bin/" .. outputdir
bintempdir = "%{wks.location}/../bin-temp/" .. outputdir .. "/%{prj.name}"

pythondir = os.getenv("PY_DIR")

IncludeDir = {}
IncludeDir["GLFW"] = "%{CommonSource}/external/GLFW/include"
IncludeDir["GLEW"] = "%{CommonSource}/external/GLEW/include"
IncludeDir["glm"] = "%{CommonSource}/Engine/external/glm"
IncludeDir["stb_image"] = "%{CommonSource}/Engine/external/stb_image"
IncludeDir["python"] = "%{pythondir}/include"
IncludeDir["pybind"] = "%{CommonSource}/external/pybind11/include"
IncludeDir["spdlog"] = "%{CommonSource}/external/spdlog/include"
IncludeDir["SoLoud"] = "%{CommonSource}/external/SoLoud/include"
IncludeDir["wxWidgets"] = "%{CommonSource}/external/wxWidgets/include"
IncludeDir["wxWidgets_win"] = "%{CommonSource}/external/wxWidgets/include/msvc"

LibDir = {}
LibDir["wxWidgets_win"] = "%{CommonSource}/external/wxWidgets/lib/vc_x64_dll/"
LibDir["python"] = "%{pythondir}/libs"


group "Dependencies"
	include "GLFW"
	include "GLEW"
	include "spdlog"
	include "SoLoud"
group ""


include "Engine"
include "Editor"
include "Project"
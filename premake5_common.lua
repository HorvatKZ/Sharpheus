workspace "%{WorkspaceName}"
	architecture "x64"
	startproject "%{StartProjectName}"

	configurations
	{
		"Debug",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

	filter "action:vs*"
        buildoptions { "/bigobj" } 

outputdir = "%{cfg.buildcfg}"
bindir = "%{wks.location}/bin/" .. outputdir
bintempdir = "%{wks.location}/bin-temp/" .. outputdir .. "/%{prj.name}"

pythondir = os.getenv("PY_DIR")

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/external/GLFW/include"
IncludeDir["GLEW"] = "%{wks.location}/external/GLEW/include"
IncludeDir["glm"] = "%{wks.location}/Engine/external/glm"
IncludeDir["stb_image"] = "%{wks.location}/Engine/external/stb_image"
IncludeDir["python"] = "%{pythondir}/include"
IncludeDir["pybind"] = "%{wks.location}/external/pybind11/include"
IncludeDir["spdlog"] = "%{wks.location}/external/spdlog/include"
IncludeDir["SoLoud"] = "%{wks.location}/external/SoLoud/include"
IncludeDir["wxWidgets"] = "%{wks.location}/external/wxWidgets/include"
IncludeDir["wxWidgets_win"] = "%{wks.location}/external/wxWidgets/include/msvc"

LibDir = {}
LibDir["wxWidgets_win"] = "%{wks.location}/external/wxWidgets/lib/vc_x64_dll/"
LibDir["python"] = "%{pythondir}/libs"

group "Dependencies"
	include "external/GLFW"
	include "external/GLEW"
	include "external/spdlog"
	include "external/SoLoud"
group ""

include "Engine"

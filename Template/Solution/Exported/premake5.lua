include "../premake_include.lua"

workspace "Exported"
	architecture "x64"
	startproject "Exported"

	configurations
	{
		"Release",
		"Final"
	}

	flags
	{
		"MultiProcessorCompile"
	}

	filter "action:vs*"
		buildoptions { "/bigobj", "/wd4018", "/wd4133", "/wd4244", "/wd4251", "/wd4267", "/wd26812" } 

outputdir = "%{cfg.buildcfg}"
bindir = "%{wks.location}/../../Exported/"
bintempdir = "%{wks.location}/../../bin-temp/Exported"

pythondir = os.getenv("PY_DIR")

IncludeDir = {}
IncludeDir["GLFW"] = "%{CommonSource}/external/GLFW/include"
IncludeDir["GLEW"] = "%{CommonSource}/external/GLEW/include"
IncludeDir["glm"] = "%{CommonSource}/Engine/external/glm"
IncludeDir["python"] = "%{pythondir}/include"
IncludeDir["pybind"] = "%{CommonSource}/external/pybind11/include"
IncludeDir["stb_image"] = "%{CommonSource}/Engine/external/stb_image"
IncludeDir["spdlog"] = "%{CommonSource}/external/spdlog/include"
IncludeDir["SoLoud"] = "%{CommonSource}/external/SoLoud/include"

LibDir = {}
LibDir["python"] = "%{pythondir}/libs"

extraEngineDefine = "SPH_EXPORTED"

group "Dependencies"
	include "../GLFW"
	include "../GLEW"
	include "../spdlog"
	include "../SoLoud"
group ""


include "../Engine"


project "Exported"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir (bindir)
	objdir (bintempdir)

	BaseDirEngine = "%{CommonSource}/Engine"

	files
	{
		"%{BaseDirEngine}/src/main.cpp",
		"%{BaseDirEngine}/src/Engine/Game.hpp",
		"%{BaseDirEngine}/src/Engine/Game.cpp",
		"%{wks.location}/../../Source/**.h",
        "%{wks.location}/../../Source/**.hpp",
		"%{wks.location}/../../Source/**.cpp",
		"%{bindir}/Assets/Icon/resource.rc",
		"%{bindir}/Assets/Icon/icon.ico"
	}

	vpaths { ['Resources/*'] = "%{bindir}/Assets/Icon/resource.rc" }

	includedirs
	{
		"%{BaseDirEngine}/src",
		"%{BaseDirEngine}/src/Engine/Gameobjects/Behaviors",
		"%{wks.location}/../../Source",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.SoLoud}"
	}

	links
	{
		"Engine",
		"GLFW",
		"GLEW",
		"spdlog",
		"SoLoud",
		"opengl32.lib",
		"Glu32.lib"
	}
	
	defines
	{
		"SPH_EXPORTED",
		"SPH_BUILD_EXPORTED",
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"GLEW_STATIC",
		"SPDLOG_COMPILED_LIB",
		"_UNICODE"
	}


	filter "system:windows"
		systemversion "latest"

	filter "configurations:Release"
		defines "SPH_RELEASE"
		runtime "Release"
		optimize "on"
	
	filter "configurations:Final"
		defines "SPH_FINAL"
		runtime "Release"
		optimize "on"
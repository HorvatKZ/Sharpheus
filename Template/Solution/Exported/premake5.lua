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

outputdir = "%{cfg.buildcfg}"
bindir = "%{wks.location}/../../Exported/"
bintempdir = "%{wks.location}/../../bin-temp/Exported"

IncludeDir = {}
IncludeDir["GLFW"] = "%{CommonSource}/external/GLFW/include"
IncludeDir["GLEW"] = "%{CommonSource}/external/GLEW/include"
IncludeDir["glm"] = "%{CommonSource}/Engine/external/glm"
IncludeDir["stb_image"] = "%{CommonSource}/Engine/external/stb_image"
IncludeDir["spdlog"] = "%{CommonSource}/external/spdlog/include"


group "Dependencies"
	include "../GLFW"
	include "../GLEW"
	include "../spdlog"
group ""


project "Exported"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir (bindir)
	objdir (bintempdir)

	BaseDirEngine = "%{CommonSource}/Engine"

	pchheader "pch.h"
	pchsource "%{BaseDirEngine}/src/pch.cpp"

	files
	{
        "%{BaseDirEngine}/src/**.h",
        "%{BaseDirEngine}/src/**.hpp",
		"%{BaseDirEngine}/src/**.cpp",
		"%{BaseDirEngine}/external/glm/glm/**.hpp",
		"%{BaseDirEngine}/external/glm/glm/**.inl",
		"%{BaseDirEngine}/external/stb_image/stb_image.h",
		"%{BaseDirEngine}/external/stb_image/stb_image.cpp",
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
		"%{IncludeDir.spdlog}"
	}

	links
	{
		"GLFW",
		"GLEW",
		"spdlog",
		"opengl32.lib",
		"Glu32.lib",
		"Winmm.lib"
	}
	
	defines
	{
		"SPH_EXPORTED",
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"GLEW_STATIC",
		"SPDLOG_COMPILED_LIB"
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
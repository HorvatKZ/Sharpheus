project "Engine"
	kind "SharedLib"
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
		"%{wks.location}/../Source/**.h",
        "%{wks.location}/../Source/**.hpp",
		"%{wks.location}/../Source/**.cpp"
	}

	includedirs
	{
		"%{BaseDirEngine}/src",
		"%{BaseDirEngine}/src/Engine/Gameobjects/Behaviors",
		"%{wks.location}/../Source",
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
		"Winmm.lib",
		"Glu32.lib"
	}
	
	defines
	{
		"SPH_BUILD_ENGINE",
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"GLEW_STATIC",
		"SPDLOG_COMPILED_LIB"
	}


	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "SPH_DEBUG"
		runtime "Debug"
		symbols "on"

		defines 
		{
			"_CRTDBG_MAP_ALLOC"
		}

	filter "configurations:Release"
		defines "SPH_RELEASE"
		runtime "Release"
		optimize "on"
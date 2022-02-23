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
		"%{BaseDirEngine}/external/stb_image/stb_image.cpp"
	}

	includedirs
	{
		"%{BaseDirEngine}/src",
		"%{BaseDirEngine}/src/Engine/Gameobjects/Behaviors",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.python}",
		"%{IncludeDir.pybind}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.SoLoud}"
	}

	libdirs
	{	
		"%{LibDir.python}"
	}

	links
	{
		"GLFW",
		"GLEW",
		"spdlog",
		"SoLoud",
		"opengl32.lib",
		"Glu32.lib"
	}
	
	defines
	{
		"SPH_BUILD_ENGINE",
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING",
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
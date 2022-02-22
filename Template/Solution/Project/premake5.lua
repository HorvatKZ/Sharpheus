project "Project"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir (bindir)
	objdir (bintempdir)

    BaseDirEngine = "%{CommonSource}/Engine"

	files
	{
        "%{wks.location}/../Source/**.h",
        "%{wks.location}/../Source/**.hpp",
		"%{wks.location}/../Source/**.cpp"
	}

	includedirs
	{
        "%{wks.location}/../Source",
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

	links
	{
		"Engine",
		"spdlog"
	}
	
	defines
	{
		"SPH_BUILD_CUSTOM",
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
project "Engine"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir (bindir)
	objdir (bintempdir)

	pchheader "pch.h"
	pchsource "src/pch.cpp"

	files
	{
        "src/**.h",
        "src/**.hpp",
		"src/**.cpp",
		"external/glm/glm/**.hpp",
		"external/glm/glm/**.inl",
		"external/stb_image/stb_image.h",
		"external/stb_image/stb_image.cpp",
		"custom/**.h",
        "custom/**.hpp",
		"custom/**.cpp"
	}

	includedirs
	{
		"src",
		"src/Engine/Gameobjects/Behaviors",
		"custom",
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
		"Winmm.lib"
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
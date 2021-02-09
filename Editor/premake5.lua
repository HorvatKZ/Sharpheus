project "Editor"
	kind "WindowedApp"
    language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir (bindir)
	objdir (bintempdir)
	
	pchheader "editor_pch.h"
	pchsource "src/editor_pch.cpp"

	files
	{
        "src/**.h",
        "src/**.hpp",
		"src/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"_UNICODE",
		"GLEW_STATIC"
	}

	includedirs
	{
        "src",
		"%{wks.location}/Engine/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.wxWidgets}"
	}

	links
	{
		"Engine",
		"GLEW",
		"opengl32.lib",
		"Glu32.lib"
	}


	filter "system:windows"
		systemversion "latest"

		includedirs
		{
			"%{IncludeDir.wxWidgets_win}"
		}

		libdirs
		{	
			"%{LibDir.wxWidgets_win}"
		}

		defines
		{
			"__WXMSW__",
			"WXUSINGDLL"
		}


	filter "configurations:Debug"
		defines "SPH_DEBUG"
		runtime "Debug"
		symbols "on"

		defines
		{
			"__WXDEBUG__"
		}


	filter "configurations:Release"
		defines "SPH_RELEASE"
		runtime "Release"
		optimize "on"
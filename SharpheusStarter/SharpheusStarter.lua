project "SharpheusStarter"
	kind "WindowedApp"
    language "C++"
	cppdialect "C++17"
	staticruntime "off"
	icon "sharpheus_icon.png"

	targetdir (bindir)
	objdir (bintempdir)
	
	pchheader "starter_pch.h"
	pchsource "src/starter_pch.cpp"

	files
	{
        "src/**.h",
        "src/**.hpp",
		"src/**.cpp",
		"resource.rc",
		"sharpheus.ico"
	}

	vpaths { ['Resources/*'] = "resource.rc" }

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"_UNICODE",
		"GLEW_STATIC",
		"SPH_DEV"
	}

	includedirs
	{
        "src",
		"%{wks.location}/Engine/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.SoLoud}",
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
			"__WXDEBUG__",
			"_CRTDBG_MAP_ALLOC"
		}


	filter "configurations:Release"
		defines "SPH_RELEASE"
		runtime "Release"
		optimize "on"
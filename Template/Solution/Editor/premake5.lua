project "Editor"
	kind "WindowedApp"
    language "C++"
	cppdialect "C++17"
	staticruntime "off"
	icon "sharpheus_icon.png"

	targetdir (bindir)
	objdir (bintempdir)

	BaseDirEditor = "%{CommonSource}/Editor"
	
	pchheader "editor_pch.h"
	pchsource "%{BaseDirEditor}/src/editor_pch.cpp"

	files
	{
        "%{BaseDirEditor}/src/**.h",
        "%{BaseDirEditor}/src/**.hpp",
		"%{BaseDirEditor}/src/**.cpp",
		"%{BaseDirEditor}/resource.rc",
		"%{BaseDirEditor}/sharpheus.ico"
	}

	vpaths { ['Resources/*'] = "%{BaseDirEditor}/resource.rc" }

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"_UNICODE",
		"GLEW_STATIC"
	}

	includedirs
	{
        "%{BaseDirEditor}/src",
		"%{CommonSource}/Engine/src",
		"%{CommonSource}/Engine/src/Engine/Gameobjects/Behaviors",
		"%{wks.location}/../Source",
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
			"__WXDEBUG__",
			"_CRTDBG_MAP_ALLOC"
		}


	filter "configurations:Release"
		defines "SPH_RELEASE"
		runtime "Release"
		optimize "on"
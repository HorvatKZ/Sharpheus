project "PyInterface"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir (bindir)
	objdir (bintempdir)

    targetname("Sharpheus")
    targetextension(".pyd")

	pchheader "py_pch.h"
	pchsource "src/py_pch.cpp"

	files
	{
        "src/**.h",
        "src/**.hpp",
		"src/**.cpp"
	}

	includedirs
	{
		"src",
		"%{wks.location}/Engine/src",
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
		"Engine"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
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
project "PyInterface"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir (bindir)
	objdir (bintempdir)

    targetname("Sharpheus")
    targetextension(".pyd")

    BaseDirPyInt = "%{CommonSource}/PyInterface"

	pchheader "py_pch.h"
	pchsource "%{BaseDirPyInt}/src/py_pch.cpp"

	files
	{
        "%{BaseDirPyInt}/src/**.h",
        "%{BaseDirPyInt}/src/**.hpp",
		"%{BaseDirPyInt}/src/**.cpp"
	}

	includedirs
	{
		"%{BaseDirPyInt}/src",
		"%{CommonSource}/Engine/src",
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
project "SoLoud"
    kind "StaticLib"
    language "C++"
    staticruntime "on"

	targetdir ("bin/" .. outputdir)
	objdir ("bin-temp/" .. outputdir .. "/%{prj.name}")

    BaseDirSoLoud = "%{CommonSource}/external/SoLoud"

    files
    {
        "%{BaseDirSoLoud}/include/**.h",
        "%{BaseDirSoLoud}/src/audiosource/**.h",
        "%{BaseDirSoLoud}/src/audiosource/**.c",
        "%{BaseDirSoLoud}/src/audiosource/**.cpp",
        "%{BaseDirSoLoud}/src/c_api/**.cpp",
        "%{BaseDirSoLoud}/src/core/**.cpp",
        "%{BaseDirSoLoud}/src/filter/**.cpp",
        "%{BaseDirSoLoud}/src/tools/**.h",
        "%{BaseDirSoLoud}/src/tools/**.cpp",
        "%{BaseDirSoLoud}/src/backend/miniaudio/**.h",
        "%{BaseDirSoLoud}/src/backend/miniaudio/**.c",
        "%{BaseDirSoLoud}/src/backend/miniaudio/**.cpp"
    }

    defines
	{
		"WITH_MINIAUDIO",
        "_CRT_SECURE_NO_WARNINGS"
	}

    includedirs
    {
        "%{BaseDirSoLoud}/include"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
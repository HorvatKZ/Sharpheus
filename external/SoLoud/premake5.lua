project "SoLoud"
    kind "StaticLib"
    language "C++"
    staticruntime "on"

	targetdir ("bin/" .. outputdir)
	objdir ("bin-temp/" .. outputdir .. "/%{prj.name}")

    files
    {
        "include/**.h",
        "src/audiosource/**.h",
        "src/audiosource/**.c",
        "src/audiosource/**.cpp",
        "src/c_api/**.cpp",
        "src/core/**.cpp",
        "src/filter/**.cpp",
        "src/tools/**.h",
        "src/tools/**.cpp",
        "src/backend/miniaudio/**.h",
        "src/backend/miniaudio/**.c",
        "src/backend/miniaudio/**.cpp"
    }

    defines
	{
		"WITH_MINIAUDIO"
	}

    includedirs
    {
        "include"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
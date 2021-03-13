project "spdlog"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir)
	objdir ("bin-temp/" .. outputdir .. "/%{prj.name}")

	BaseDirSpdlog = "%{CommonSource}/external/spdlog"

	files
	{
        "%{BaseDirSpdlog}/include/**.h",
		"%{BaseDirSpdlog}/src/**.cpp"
	}

	includedirs
	{
		"%{BaseDirSpdlog}/include"
    }
    
    defines
    {
        "SPDLOG_COMPILED_LIB"
    }

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
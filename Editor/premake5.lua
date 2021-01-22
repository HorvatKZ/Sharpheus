project "Editor"
	kind "ConsoleApp"
    language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir)
    objdir ("%{wks.location}/bin-temp/" .. outputdir .. "/%{prj.name}")
    

	files
	{
        "src/**.h",
        "src/**.hpp",
		"src/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
        "src",
        "%{wks.location}/Engine/src"
	}

	links
	{
		"Engine"
	}


	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "SPH_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SPH_RELEASE"
		runtime "Release"
		optimize "on"
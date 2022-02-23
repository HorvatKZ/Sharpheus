project "GLEW"
    kind "StaticLib"
    language "C"
    staticruntime "on"

	targetdir ("bin/" .. outputdir)
	objdir ("bin-temp/" .. outputdir .. "/%{prj.name}")

    files
    {
        "include/GL/glew.h",
        "build/**.rc",
        "src/**.c"
    }

    filter "system:linux"
		systemversion "latest"
		staticruntime "On"

		files
		{
            "include/GL/glxew.h"
		}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		files
		{
            "include/GL/wglew.h"
        }
        
        defines 
		{ 
			"GLEW_STATIC",
            "_CRT_SECURE_NO_WARNINGS"
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
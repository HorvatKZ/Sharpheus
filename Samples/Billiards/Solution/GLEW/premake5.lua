project "GLEW"
    kind "StaticLib"
    language "C"
    staticruntime "on"

	targetdir ("bin/" .. outputdir)
	objdir ("bin-temp/" .. outputdir .. "/%{prj.name}")

    BaseDirGLEW = "%{CommonSource}/external/GLEW"

    files
    {
        "%{BaseDirGLEW}/include/GL/glew.h",
        "%{BaseDirGLEW}/build/**.rc",
        "%{BaseDirGLEW}/src/**.c"
    }

    filter "system:linux"
		systemversion "latest"
		staticruntime "On"

		files
		{
            "%{BaseDirGLEW}/include/GL/glxew.h"
		}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		files
		{
            "%{BaseDirGLEW}/include/GL/wglew.h"
        }
        
        defines 
		{ 
			"GLEW_STATIC",
            "_CRT_SECURE_NO_WARNINGS"
		}


    includedirs
    {
        "%{BaseDirGLEW}/include"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
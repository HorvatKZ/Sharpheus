project "GLFW"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir)
	objdir ("bin-temp/" .. outputdir .. "/%{prj.name}")

	BaseDirGLFW = "%{CommonSource}/external/GLFW"

	files
	{
		"%{BaseDirGLFW}/include/GLFW/glfw3.h",
		"%{BaseDirGLFW}/include/GLFW/glfw3native.h",
		"%{BaseDirGLFW}/src/glfw_config.h",
		"%{BaseDirGLFW}/src/context.c",
		"%{BaseDirGLFW}/src/init.c",
		"%{BaseDirGLFW}/src/input.c",
		"%{BaseDirGLFW}/src/monitor.c",
		"%{BaseDirGLFW}/src/vulkan.c",
		"%{BaseDirGLFW}/src/window.c"
    }
    
	filter "system:linux"
		pic "On"

		systemversion "latest"
		staticruntime "On"

		files
		{
			"%{BaseDirGLFW}/src/x11_init.c",
			"%{BaseDirGLFW}/src/x11_monitor.c",
			"%{BaseDirGLFW}/src/x11_window.c",
			"%{BaseDirGLFW}/src/xkb_unicode.c",
			"%{BaseDirGLFW}/src/posix_time.c",
			"%{BaseDirGLFW}/src/posix_thread.c",
			"%{BaseDirGLFW}/src/glx_context.c",
			"%{BaseDirGLFW}/src/egl_context.c",
			"%{BaseDirGLFW}/src/osmesa_context.c",
			"%{BaseDirGLFW}/src/linux_joystick.c"
		}

		defines
		{
			"_GLFW_X11"
		}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		files
		{
			"%{BaseDirGLFW}/src/win32_init.c",
			"%{BaseDirGLFW}/src/win32_joystick.c",
			"%{BaseDirGLFW}/src/win32_monitor.c",
			"%{BaseDirGLFW}/src/win32_time.c",
			"%{BaseDirGLFW}/src/win32_thread.c",
			"%{BaseDirGLFW}/src/win32_window.c",
			"%{BaseDirGLFW}/src/wgl_context.c",
			"%{BaseDirGLFW}/src/egl_context.c",
			"%{BaseDirGLFW}/src/osmesa_context.c"
		}

		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
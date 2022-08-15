#include "pch.h"
#include "PydExport.hpp"


namespace Sharpheus {


	PYBIND11_MODULE(Engine, handle) {
		handle.doc() = "Sharpheus Game Engine Python Interface [using pybind11]";

		Export_Logger(handle);
		Export_Geometry(handle);
		Export_Renderer(handle);
		Export_ResourceManager(handle);
		Export_GameObjects(handle);

		// TODO file utils
		// TODO events
	}

}
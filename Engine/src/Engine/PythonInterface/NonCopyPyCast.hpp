#pragma once

#include <pybind11/pybind11.h>


namespace pybind11 {
	
	template <typename T>
	inline T* noncopy_cast(const object* obj) {
		return reinterpret_cast<T*>(obj->attr("noncopy_cast_address").cast<size_t>());
	}

	template <typename T, typename PyClass>
	inline void prepare_for_noncopy_cast(PyClass& cls) {
		cls.def_property_readonly("noncopy_cast_address", [](const T* instance) {
			return reinterpret_cast<size_t>(instance);
		});
	}

}

#include "pch.h"
#include "PythonObjectState.hpp"
#include "PythonInterface.hpp"
#include "Engine/ResourceManager/ResourceManager.hpp"
#include "Engine/GameObjects/GameObject.hpp"

#include <pybind11/pybind11.h>

namespace Sharpheus {

	PythonObjectState::~PythonObjectState()
	{
		Clear();
	}


	PythonObjectState::PythonObjectState(const PythonObjectState& other)
	{
		Clear();
		for (auto it = other.states.begin(); it != other.states.end(); ++it) {
			states[it->first] = new py::object(*it->second);
		}
	}

	PythonObjectState& PythonObjectState::operator=(const PythonObjectState& other)
	{
		Clear();
		for (auto it = other.states.begin(); it != other.states.end(); ++it) {
			states[it->first] = new py::object(*it->second);
		}
		return *this;
	}


	void PythonObjectState::FillFrom(const py::object& source, const py::list& argNames)
	{
		Clear();
		for (uint32 i = 0; i < argNames.size(); ++i) {
			std::string argName = argNames[i].cast<std::string>();
			if (SPH_VERIFY(py::hasattr(source, argName.c_str()), "Source should have attr {0}, but does not", argName)) {
				states[argName] = new py::object(source.attr(argName.c_str()));
			}
		}
	}


	void PythonObjectState::FillTo(py::object& target)
	{
		for (auto it = states.begin(); it != states.end(); ++it) {
			target.attr(it->first.c_str()) = it->second;
		}
	}


	void PythonObjectState::Save(FileSaver& fs)
	{
		fs.Write((uint32)states.size());
		PythonInterface::Exec("Saving PythonObjectState", [&] {
			for (auto it = states.begin(); it != states.end(); ++it) {
				fs.Write(it->first);
				SaveObject(fs, it->second);
			}
		});
	}


	void PythonObjectState::Load(FileLoader& fl)
	{
		uint32 size = 0;
		fl.Read(size);
		PythonInterface::Exec("Saving PythonObjectState", [&] {
			for (uint32 i = 0; i < size; ++i) {
				std::string name;
				fl.Read(name);
				py::object* obj = LoadObject(fl);
				states[name] = obj;
			}
		});
	}

	std::vector<CommonProvider*> PythonObjectState::ToProviders()
	{
		std::vector<CommonProvider*> providers;
		PythonInterface::Exec("Saving PythonObjectState", [&] {
			for (auto it = states.begin(); it != states.end(); ++it) {
				CommonProvider* provider = ProviderOf(it->first, it->second);
				if (provider != nullptr) {
					providers.push_back(provider);
				}
			}
		});
		return providers;
	}


	void PythonObjectState::SaveObject(FileSaver& fs, const py::object* obj)
	{
		std::string typeName = GetTypeStr(obj);

		fs.Write(typeName);
		if (typeName == "int") {
			fs.Write(obj->cast<int32>());
		} else if (typeName == "float") {
			fs.Write(obj->cast<float>());
		} else if (typeName == "bool") {
			fs.Write(obj->cast<bool>());
		} else if (typeName == "str") {
			fs.Write(obj->cast<std::string>());
		} else if (typeName == "list") {
			const py::list l = obj->cast<const py::list>();
			fs.Write((uint32)l.size());
			for (uint32 i = 0; i < l.size(); ++i) {
				SaveObject(fs, &static_cast<py::object>(l[i]));
			}
		} else if (typeName == "tuple") {
			const py::tuple t = obj->cast<const py::tuple>();
			fs.Write((uint32)t.size());
			for (uint32 i = 0; i < t.size(); ++i) {
				SaveObject(fs, &static_cast<py::object>(t[i]));
			}
		} else if (typeName == "dict") {
			const py::dict d = obj->cast<const py::dict>();
			fs.Write((uint32)d.size());
			for (auto it = d.begin(); it != d.end(); ++it) {
				fs.Write(it->first.cast<std::string>());
				SaveObject(fs, &it->second.cast<py::object>());
			}
		} else if (IsInternalTypeStr(typeName)) {
			std::string internalType = typeName.substr(PythonInterface::moduleName.length() + 1);
			if (internalType == "Point") {
				fs.Write(obj->cast<Point>());
			} else if (internalType == "Color") {
				fs.Write(obj->cast<Color>());
			} else if (internalType == "Transform") {
				fs.Write(obj->cast<Transform>());
			} else if (internalType == "Image") {
				fs.Write(&obj->cast<Image>());
			} else if (internalType == "Font") {
				fs.Write(&obj->cast<Font>());
			} else if (internalType == "Animation") {
				fs.Write(&obj->cast<Animation>());
			} else if (internalType == "TileSet") {
				fs.Write(&obj->cast<TileSet>());
			} else if (internalType == "Audio") {
				fs.Write(&obj->cast<Audio>());
			} else {
				SPH_ERROR("Unhandled internal type {0} at save", typeName);
			}
		} else {
			SPH_ERROR("Unhandled type {0} at save", typeName);
		}
	}


	py::object* PythonObjectState::LoadObject(FileLoader& fl)
	{
		std::string typeName;
		fl.Read(typeName);
		if (typeName == "int") {
			return LoadBuiltInType<int32, py::int_>(fl);
		} else if (typeName == "float") {
			return LoadBuiltInType<float, py::float_>(fl);
		} else if (typeName == "bool") {
			return LoadBuiltInType<bool, py::bool_>(fl);
		} else if (typeName == "str") {
			return LoadBuiltInType<std::string, py::str>(fl);
		} else if (typeName == "list") {
			uint32 size;
			fl.Read(size);
			py::list* l = new py::list(size);
			for (uint32 i = 0; i < size; ++i) {
				py::object* obj = LoadObject(fl);
				(*l)[i] = *obj;
				delete obj;
			}
		} else if (typeName == "tuple") {
			uint32 size;
			fl.Read(size);
			py::tuple* t = new py::tuple(size);
			for (uint32 i = 0; i < size; ++i) {
				py::object* obj = LoadObject(fl);
				(*t)[i] = *obj;
				delete obj;
			}
		} else if (typeName == "dict") {
			uint32 size;
			fl.Read(size);
			py::dict* d = new py::dict();
			for (uint32 i = 0; i < size; ++i) {
				std::string key;
				fl.Read(key);
				py::object* obj = LoadObject(fl);
				(*d)[key.c_str()] = *obj;
				delete obj;
			}
		} else if (IsInternalTypeStr(typeName)) {
			std::string internalType = typeName.substr(PythonInterface::moduleName.length() + 1);
			if (internalType == "Point") {
				return LoadInternalReference<Point>(fl);
			} else if (internalType == "Color") {
				return LoadInternalReference<Color>(fl);
			} else if (internalType == "Transform") {
				return LoadInternalReference<Transform>(fl);
			} else if (internalType == "Image") {
				return LoadResource<Image>(fl);
			} else if (internalType == "Font") {
				return LoadResource<Font>(fl);
			} else if (internalType == "Animation") {
				return LoadResource<Animation>(fl);
			} else if (internalType == "TileSet") {
				return LoadResource<TileSet>(fl);
			} else if (internalType == "Audio") {
				return LoadResource<Audio>(fl);
			} else {
				SPH_ERROR("Unhandled internal type {0} at load", typeName);
			}
		} else {
			SPH_ERROR("Unhandled type {0} at load", typeName);
		}

		return nullptr;
	}


	void PythonObjectState::Clear()
	{
		for (auto it = states.begin(); it != states.end(); ++it) {
			delete it->second;
		}
		states.clear();
	}


	bool PythonObjectState::IsInternalTypeStr(const std::string& typeName)
	{
		return typeName.length() > PythonInterface::moduleName.length() + 1 &&
			typeName.substr(0, PythonInterface::moduleName.length() + 1) == PythonInterface::moduleName + ".";
	}


	std::string PythonObjectState::GetTypeStr(const py::object* obj)
	{
		std::string typeName = py::str(obj->attr("__class__")).cast<std::string>();
		SPH_ASSERT(typeName.length() > 10 && typeName.substr(0, 8) == "<class '" && typeName.substr(typeName.length() - 2, 2) == "'>", "typeName {0} is not well formed", typeName);
		return typeName.substr(8, typeName.length() - 10);
	}

	template<typename T, class PyType>
	py::object* PythonObjectState::LoadBuiltInType(FileLoader& fl)
	{
		T v;
		fl.Read(v);
		return new PyType(v);
	}

	template<typename T>
	py::object* PythonObjectState::LoadInternalReference(FileLoader& fl)
	{
		T v;
		fl.Read(v);
		return new py::object(py::cast(v));
	}

	template<typename T>
	py::object* PythonObjectState::LoadResource(FileLoader& fl)
	{
		const T* v;
		fl.Read(&v);
		return new py::object(py::cast(v));
	}

	template<class ProviderClass, typename T, class PyType>
	CommonProvider* PythonObjectState::ProviderOfBuiltInType(const std::string& name)
	{
		return new ProviderClass(name,
			[&](GameObject* unused) {
				T result;
				PythonInterface::Exec("PythonObjectState ProviderOfBuiltIn Get", [&] { result = states[name]->cast<T>(); });
				return result;
			},
			[&](GameObject* unused, T value) {
				PythonInterface::Exec("PythonObjectState ProviderOfBuiltIn Set", [&] {
					delete states[name];
					states[name] = new PyType(value);
				});
			}
		);
	}

	template<>
	CommonProvider* PythonObjectState::ProviderOfBuiltInType<StringProvider<GameObject>, const std::string&, py::str>(const std::string& name)
	{
		return new StringProvider<GameObject>(name,
			[&](GameObject* unused) -> const std::string& {
				PythonInterface::Exec("PythonObjectState StringProvider Get", [&] { temp = states[name]->cast<std::string>(); });
				return temp;
			},
			[&](GameObject* unused, const std::string& value) {
				PythonInterface::Exec("PythonObjectState StringProvider Set", [&] {
					delete states[name];
					states[name] = new py::str(value);
				});
			}
		);
	}


	template<class ProviderClass, typename T>
	CommonProvider* PythonObjectState::ProviderOfInternalReference(const std::string& name)
	{
		return new ProviderClass(name,
			[&](GameObject* unused) {
				T result;
				PythonInterface::Exec("PythonObjectState ProviderOfInternalReference Get", [&] { result = states[name]->cast<T>(); });
				return result;
			},
			[&](GameObject* unused, const T& value) {
				PythonInterface::Exec("PythonObjectState ProviderOfInternalReference Set", [&] {
					delete states[name];
					states[name] = new py::object(py::cast(value));
				});
			}
		);
	}


	template<class ProviderClass, typename T>
	CommonProvider* PythonObjectState::ProviderOfResource(const std::string& name)
	{
		return new ProviderClass(name,
			[&](GameObject* unused) {
				const T* result;
				PythonInterface::Exec("PythonObjectState ProviderOfResource Get", [&] { result = states[name]->cast<const T*>(); });
				return result;
			},
			[&](GameObject* unused, const T* value) {
				PythonInterface::Exec("PythonObjectState ProviderOfResource Set", [&] {
					delete states[name];
					states[name] = new py::object(py::cast(value));
				});
			},
			[&](GameObject* unused, const std::string& path) {
			PythonInterface::Exec("PythonObjectState ProviderOfResource SetByPath", [&] {
					delete states[name];
					states[name] = new py::object(py::cast(ResourceManager::GetResource<T>(path)));
				});
			}
		);
	}


	template<>
	CommonProvider* PythonObjectState::ProviderOfResource<ImageProvider<GameObject>, Image>(const std::string& name)
	{
		return new ImageProvider<GameObject>(name,
			[&](GameObject* unused) {
				const Image* result;
				PythonInterface::Exec("PythonObjectState ImageProvider Get", [&] { result = states[name]->cast<const Image*>(); });
				return result;
			},
			[&](GameObject* unused, const Image* value) {
				PythonInterface::Exec("PythonObjectState ImageProvider Set", [&] {
					delete states[name];
					states[name] = new py::object(py::cast(value));
				});
			},
			[&](GameObject* unused, const std::string& path, bool isFiltered) {
				PythonInterface::Exec("PythonObjectState ImageProvider SetByPath", [&] {
					delete states[name];
					states[name] = new py::object(py::cast(ResourceManager::GetImage(path, isFiltered)));
				});
			}
		);
	}


	template<>
	CommonProvider* PythonObjectState::ProviderOfResource<FontProvider<GameObject>, Font>(const std::string& name)
	{
		return new FontProvider<GameObject>(name,
			[&](GameObject* unused) {
				const Font* result;
				PythonInterface::Exec("PythonObjectState FontProvider Get", [&] { result = states[name]->cast<const Font*>(); });
				return result;
			},
			[&](GameObject* unused, const Font* value) {
				PythonInterface::Exec("PythonObjectState ImageProvider Set", [&] {
					delete states[name];
					states[name] = new py::object(py::cast(value));
				});
			},
			[&](GameObject* unused, const std::string& name) {
				PythonInterface::Exec("PythonObjectState FontProvider SetByName", [&] {
					delete states[name];
					states[name] = new py::object(py::cast(ResourceManager::GetFont(name)));
					});
			},
			[&](GameObject* unused, const std::string& fontFile, const std::string& imgFile) {
				PythonInterface::Exec("PythonObjectState FontProvider SetByPath", [&] {
					delete states[name];
					states[name] = new py::object(py::cast(ResourceManager::GetFont(fontFile, imgFile)));
				});
			}
		);
	}


	CommonProvider* PythonObjectState::ProviderOf(const std::string& name, py::object* obj)
	{
		std::string typeName = GetTypeStr(obj);
		if (typeName == "int") {
			return ProviderOfBuiltInType<IntProvider<GameObject>, int32, py::int_>(name);
		} else if (typeName == "float") {
			return ProviderOfBuiltInType<FloatProvider<GameObject>, float, py::float_>(name);
		} else if (typeName == "bool") {
			return ProviderOfBuiltInType<BoolProvider<GameObject>, bool, py::bool_>(name);
		} else if (typeName == "str") {
			return ProviderOfBuiltInType<StringProvider<GameObject>, const std::string&, py::str>(name);
		} else if (IsInternalTypeStr(typeName)) {
			std::string internalType = typeName.substr(PythonInterface::moduleName.length() + 1);
			if (internalType == "Point") {
				return ProviderOfInternalReference<PointProvider<GameObject>, Point>(name);
			} else if (internalType == "Color") {
				return ProviderOfInternalReference<ColorProvider<GameObject>, Color>(name);
			} else if (internalType == "Transform") {
				return ProviderOfInternalReference<TrafoProvider<GameObject>, Transform>(name);
			} else if (internalType == "Image") {
				return ProviderOfResource<ImageProvider<GameObject>, Image>(name);
			} else if (internalType == "Font") {
				return ProviderOfResource<FontProvider<GameObject>, Font>(name);
			} else if (internalType == "Animation") {
				return ProviderOfResource<AnimationProvider<GameObject>, Animation>(name);
			} else if (internalType == "TileSet") {
				return ProviderOfResource<TileSetProvider<GameObject>, TileSet>(name);
			} else if (internalType == "Audio") {
				return ProviderOfResource<AudioProvider<GameObject>, Audio>(name);
			}
		}
		return nullptr;
	}


}
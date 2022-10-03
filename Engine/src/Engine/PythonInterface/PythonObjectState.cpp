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
			int32 v;
			fl.Read(v);
			return new py::int_(v);
		} else if (typeName == "float") {
			float v;
			fl.Read(v);
			return new py::float_(v);
		} else if (typeName == "bool") {
			bool v;
			fl.Read(v);
			return new py::bool_(v);
		} else if (typeName == "str") {
			std::string v;
			fl.Read(v);
			return new py::str(v);
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
				Point v;
				fl.Read(v);
				return new py::object(py::cast(v));
			} else if (internalType == "Color") {
				Color v;
				fl.Read(v);
				return new py::object(py::cast(v));
			} else if (internalType == "Transform") {
				Transform v;
				fl.Read(v);
				return new py::object(py::cast(v));
			} else if (internalType == "Image") {
				const Image* v;
				fl.Read(&v);
				return new py::object(py::cast(v));
			} else if (internalType == "Font") {
				const Font* v;
				fl.Read(&v);
				return new py::object(py::cast(v));
			} else if (internalType == "Animation") {
				const Animation* v;
				fl.Read(&v);
				return new py::object(py::cast(v));
			} else if (internalType == "TileSet") {
				const TileSet* v;
				fl.Read(&v);
				return new py::object(py::cast(v));
			} else if (internalType == "Audio") {
				const Audio* v;
				fl.Read(&v);
				return new py::object(py::cast(v));
			} else {
				SPH_ERROR("Unhandled internal type {0} at load", typeName);
			}
		} else {
			SPH_ERROR("Unhandled type {0} at load", typeName);
		}
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

	CommonProvider* PythonObjectState::ProviderOf(const std::string& name, py::object* obj)
	{
		std::string typeName = GetTypeStr(obj);
		if (typeName == "int") {
			return new IntProvider<GameObject>(name,
				[&](GameObject* unused) {
					int32 result;
					PythonInterface::Exec("PythonObjectState IntProvider Get", [&] { result = states[name]->cast<int32>(); });
					return result;
				},
				[&](GameObject* unused, int32 value) {
					PythonInterface::Exec("PythonObjectState IntProvider Set", [&] {
						delete states[name];
						states[name] = new py::int_(value);
					});
				}
			);
		} else if (typeName == "float") {
			return new FloatProvider<GameObject>(name,
				[&](GameObject* unused) {
					float result;
					PythonInterface::Exec("PythonObjectState FloatProvider Get", [&] { result = states[name]->cast<float>(); });
					return result;
				},
				[&](GameObject* unused, float value) {
					PythonInterface::Exec("PythonObjectState FloatProvider Set", [&] {
						delete states[name];
						states[name] = new py::float_(value);
					});
				}
			);
		} else if (typeName == "bool") {
			return new BoolProvider<GameObject>(name,
				[&](GameObject* unused) {
					bool result;
					PythonInterface::Exec("PythonObjectState BoolProvider Get", [&] { result = states[name]->cast<bool>(); });
					return result;
				},
				[&](GameObject* unused, bool value) {
					PythonInterface::Exec("PythonObjectState BoolProvider Set", [&] {
						delete states[name];
						states[name] = new py::bool_(value);
					});
				}
			);
		} else if (typeName == "str") {
			return new StringProvider<GameObject>(name,
				[&](GameObject* unused) {
					std::string result;
					PythonInterface::Exec("PythonObjectState StringProvider Get", [&] { result = states[name]->cast<std::string>(); });
					return result;
				},
				[&](GameObject* unused, const std::string& value) {
					PythonInterface::Exec("PythonObjectState StringProvider Set", [&] {
						delete states[name];
						states[name] = new py::str(value);
					});
				}
			);
		} else if (IsInternalTypeStr(typeName)) {
			std::string internalType = typeName.substr(PythonInterface::moduleName.length() + 1);
			if (internalType == "Point") {
				return new PointProvider<GameObject>(name,
					[&](GameObject* unused) {
						Point result;
						PythonInterface::Exec("PythonObjectState PointProvider Get", [&] { result = states[name]->cast<Point>(); });
						return result;
					},
					[&](GameObject* unused, const Point& value) {
						PythonInterface::Exec("PythonObjectState PointProvider Set", [&] {
							delete states[name];
							states[name] = new py::object(py::cast(value));
						});
					}
				);
			} else if (internalType == "Color") {
				return new ColorProvider<GameObject>(name,
					[&](GameObject* unused) {
						Color result;
						PythonInterface::Exec("PythonObjectState ColorProvider Get", [&] { result = states[name]->cast<Color>(); });
						return result;
					},
					[&](GameObject* unused, const Color& value) {
						PythonInterface::Exec("PythonObjectState ColorProvider Set", [&] {
							delete states[name];
							states[name] = new py::object(py::cast(value));
						});
					}
				);
			} else if (internalType == "Transform") {
				return new TrafoProvider<GameObject>(name,
					[&](GameObject* unused) {
						Transform result;
						PythonInterface::Exec("PythonObjectState TrafoProvider Get", [&] { result = states[name]->cast<Transform>(); });
						return result;
					},
					[&](GameObject* unused, const Transform& value) {
						PythonInterface::Exec("PythonObjectState TrafoProvider Set", [&] {
							delete states[name];
							states[name] = new py::object(py::cast(value));
						});
					}
				);
			}
		}
		return nullptr;
	}

}
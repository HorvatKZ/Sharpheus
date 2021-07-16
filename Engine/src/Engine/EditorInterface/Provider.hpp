#pragma once

#include "pch.h"


namespace Sharpheus {

	template <class Class, class T> using Getter = std::function<T(Class*)>;
	template <class Class, class T> using Setter = std::function<void(Class*, T)>;

	
	class CommonProvider
	{
	public:
		enum class Type {
			BOOL, ONEWAYBOOL, INT, UINT, FLOAT, UFLOAT, POINT, TRAFO, COLOR, IMAGE, FONT, FONTSTYLE, ANIM, TILESET, SOUND, STRING, STRINGLIST, BEHAVIOR, TILEMAP
		};

		CommonProvider(const std::string& name) : name(name) {}
		virtual ~CommonProvider() = default;

		inline const std::string& GetName() { return name; }
		virtual inline Type GetType() = 0;

	protected:
		std::string name;
	};


	template <CommonProvider::Type type>
	class SignalOnlyProvider : public CommonProvider
	{
	public:
		SignalOnlyProvider(const std::string& name) : CommonProvider(name) {}
		virtual ~SignalOnlyProvider() = default;

		virtual inline Type GetType() override { return type; }
	};

	using BehaviorProvider = SignalOnlyProvider<CommonProvider::Type::BEHAVIOR>;
	using TileMapProvider = SignalOnlyProvider<CommonProvider::Type::TILEMAP>;


	template <class Class, class T, CommonProvider::Type type>
	class Provider : public CommonProvider
	{
	public:
		Provider(const std::string& name, Getter<Class, T>&& getter, Setter<Class, T>&& setter)
			: CommonProvider(name), getter(std::move(getter)), setter(std::move(setter)) {}
		virtual ~Provider() = default;

		virtual inline T Get(Class* obj) { return getter(obj); }
		virtual inline void Set(Class* obj, T value) { setter(obj, value); }

		virtual inline Type GetType() override { return type; }

	protected:
		Getter<Class, T> getter;
		Setter<Class, T> setter;
	};


	template <class Class, class T, CommonProvider::Type type>
	class RangeableProvider : public Provider<Class, T, type>
	{
	public:
		RangeableProvider(const std::string& name, Getter<Class, T>&& getter, Setter<Class, T>&& setter)
			: Provider<Class, T, type>(name, std::move(getter), std::move(setter)), hasRange(false) {}
		RangeableProvider(const std::string& name, Getter<Class, T>&& getter, Setter<Class, T>&& setter, T min, T max)
			: Provider<Class, T, type>(name, std::move(getter), std::move(setter)), hasRange(true), min(min), max(max) {}
		virtual ~RangeableProvider() = default;

		virtual inline void Set(Class * obj, T value) override { if (!hasRange || min <= value && value <= max) setter(obj, value); }

		virtual inline bool HasRange() { return hasRange; }
		virtual inline T GetMin() { return min; }
		virtual inline T GetMax() { return max; }

	protected:
		bool hasRange;
		T min, max;
	};


	template <class Class> using IntProvider	= RangeableProvider<Class, int32_t,		CommonProvider::Type::INT>;
	template <class Class> using UIntProvider	= RangeableProvider<Class, uint32_t,		CommonProvider::Type::UINT>;
	template <class Class> using FloatProvider	= RangeableProvider<Class, float,		CommonProvider::Type::FLOAT>;

	template <class Class> using BoolProvider		= Provider<Class, bool,					CommonProvider::Type::BOOL>;
	template <class Class> using FontStyleProvider	= Provider<Class, uint8_t,				CommonProvider::Type::FONTSTYLE>;
	template <class Class> using PointProvider		= Provider<Class, const Point&,			CommonProvider::Type::POINT>;
	template <class Class> using TrafoProvider		= Provider<Class, const Transform&,		CommonProvider::Type::TRAFO>;
	template <class Class> using ColorProvider		= Provider<Class, const Color&,			CommonProvider::Type::COLOR>;
	template <class Class> using StringProvider		= Provider<Class, const std::string&,	CommonProvider::Type::STRING>;
	template <class Class> using SoundProvider		= Provider<Class, const std::string&,	CommonProvider::Type::SOUND>;


	template <class Class>
	class UFloatProvider : public RangeableProvider<Class, float, CommonProvider::Type::UFLOAT>
	{
	public:
		UFloatProvider(const std::string& name, Getter<Class, float>&& getter, Setter<Class, float>&& setter)
			: RangeableProvider<Class, float, Type::UFLOAT>(name, std::move(getter), std::move(setter)) {}
		UFloatProvider(const std::string& name, Getter<Class, float>&& getter, Setter<Class, float>&& setter, float min, float max)
			: RangeableProvider<Class, float, Type::UFLOAT>(name, std::move(getter), std::move(setter), min, max){}
		virtual ~UFloatProvider() = default;

		virtual inline void Set(Class * obj, float value) override {
			if (value >= 0.f && (!hasRange || min <= value && value <= max)) {
				setter(obj, value);
			}
		}
	};


	template <class Class>
	class ImageProvider : public Provider<Class, class Image*, CommonProvider::Type::IMAGE>
	{
	public:
		ImageProvider(const std::string& name, Getter<Class, class Image*>&& getter, Setter<Class, class Image*>&& setter,
			std::function<void(Class*, const std::string&, bool)>&& pathSetter)
			: Provider<Class, class Image*, CommonProvider::Type::IMAGE>(name, std::move(getter), std::move(setter)), pathSetter(std::move(pathSetter)) {}
		virtual ~ImageProvider() = default;

		virtual inline void SetByPath(Class* obj, const std::string& path, bool filtered) { pathSetter(obj, path, filtered); }

	protected:
		std::function<void(Class*, const std::string&, bool)> pathSetter;
	};


	template <class Class, class T, CommonProvider::Type type>
	class ResourceProvider : public Provider<Class, T, type>
	{
	public:
		ResourceProvider(const std::string& name, Getter<Class, T>&& getter, Setter<Class, T>&& setter,
			std::function<void(Class*, const std::string&)>&& pathSetter)
			: Provider<Class, T, type>(name, std::move(getter), std::move(setter)), pathSetter(std::move(pathSetter)) {}
		virtual ~ResourceProvider() = default;

		virtual inline void SetByPath(Class* obj, const std::string& path) { pathSetter(obj, path); }

	protected:
		std::function<void(Class*, const std::string&)> pathSetter;
	};

	template <class Class> using AnimationProvider	= ResourceProvider<Class, class Animation*, CommonProvider::Type::ANIM>;
	template <class Class> using TileSetProvider	= ResourceProvider<Class, class TileSet*, CommonProvider::Type::TILESET>;


	template <class Class>
	class FontProvider : public Provider<Class, class Font*, CommonProvider::Type::FONT>
	{
	public:
		FontProvider(const std::string& name, Getter<Class, class Font*>&& getter, Setter<Class, class Font*>&& setter,
			std::function<void(Class*, const std::string&)>&& setByName, std::function<void(Class*, const std::string&, const std::string&)>&& setByPath,
			const std::unordered_map<std::string, Font*>* fontTable)
			: Provider<Class, class Font*, CommonProvider::Type::FONT>(name, std::move(getter), std::move(setter)),
			setByName(std::move(setByName)), setByPath(std::move(setByPath)), fontTable(fontTable) {}
		virtual ~FontProvider() = default;

		virtual inline void SetByName(Class* obj, const std::string& name) { setByName(obj, name); }
		virtual inline void SetByPath(Class* obj, const std::string& fontFile, const std::string& imgFile) { setByPath(obj, fontFile, imgFile); }
		virtual inline const std::unordered_map<std::string, Font*>* GetAllFonts() { return fontTable; }

	protected:
		std::function<void(Class*, const std::string&)> setByName;
		std::function<void(Class*, const std::string&, const std::string&)> setByPath;
		const std::unordered_map<std::string, Font*>* fontTable;
	};


	template <class Class>
	class OneWayBoolProvider : public Provider<Class, bool, CommonProvider::Type::ONEWAYBOOL>
	{
	public:
		OneWayBoolProvider(const std::string& name, Getter<Class, bool>&& getter, Setter<Class, bool>&& setter, bool way)
			: Provider<Class, bool, Type::ONEWAYBOOL>(name, std::move(getter), std::move(setter)), way(way) {}
		virtual ~OneWayBoolProvider() = default;

		virtual inline void Set(Class* obj, bool value) override { if (value == way) setter(obj, value); }
		virtual inline void Set(Class* obj) { setter(obj, way); }
		virtual inline bool GetWay() { return way; }

	protected:
		bool way;
	};


	template <class Class>
	class StringListProvider : public CommonProvider
	{
	public:
		StringListProvider(const std::string& name,
			std::function<uint32_t(Class*)>&& getCount,
			std::function<uint32_t(Class*)>&& getCurr,
			std::function<const std::string& (Class*, uint32_t)>&& getString,
			std::function<void(Class*, uint32_t)>&& setCurr,
			std::function<void(Class*, uint32_t, const std::string&)>&& setString,
			std::function<void(Class*, const std::string&)>&& addString,
			std::function<void(Class*, uint32_t)>&& removeString)
			: CommonProvider(name),
			getCount(std::move(getCount)), getCurr(std::move(getCurr)), getString(std::move(getString)), setCurr(std::move(setCurr)),
			setString(std::move(setString)), addString(std::move(addString)), removeString(std::move(removeString)) {}
		virtual ~StringListProvider() = default;

		virtual inline Type GetType() override { return Type::STRINGLIST; }

		virtual inline uint32_t GetCount(Class* obj) { return getCount(obj); }
		virtual inline uint32_t GetCurr(Class* obj) { return getCurr(obj); }
		virtual inline const std::string& GetString(Class* obj, uint32_t ind) { return getString(obj, ind); }
		virtual inline void SetCurr(Class* obj, uint32_t ind) { return setCurr(obj, ind); }
		virtual inline void SetString(Class* obj, uint32_t ind, const std::string& str) { return setString(obj, ind, str); }
		virtual inline void AddString(Class* obj, const std::string& str) { return addString(obj, str); }
		virtual inline void RemoveString(Class* obj, uint32_t ind) { return removeString(obj, ind); }

	private:
		std::function<uint32_t(Class*)> getCount;
		std::function<uint32_t(Class*)> getCurr;
		std::function<const std::string& (Class*, uint32_t)> getString;
		std::function<void(Class*, uint32_t)> setCurr;
		std::function<void(Class*, uint32_t, const std::string&)> setString;
		std::function<void(Class*, const std::string&)> addString;
		std::function<void(Class*, uint32_t)> removeString;
	};

}
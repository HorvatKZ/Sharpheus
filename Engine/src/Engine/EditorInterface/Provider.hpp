#pragma once

#include "pch.h"


namespace Sharpheus {

	template <class Class, class T> using Getter = std::function<T(Class*)>;
	template <class Class, class T> using Setter = std::function<void(Class*, T)>;

	
	class CommonProvider
	{
	public:
		enum class Type {
			BOOL, ONEWAYBOOL, INT, UINT, FLOAT, UFLOAT, POINT, TRAFO, COLOR, IMAGE, STRING, BEHAVIOR
		};

		CommonProvider(const std::string& name) : name(name) {}
		virtual ~CommonProvider() = default;

		inline const std::string& GetName() { return name; }
		virtual inline Type GetType() = 0;

	protected:
		std::string name;
	};


	class BehaviorProvider : public CommonProvider
	{
	public:
		BehaviorProvider(const std::string& name) : CommonProvider(name) {}
		virtual ~BehaviorProvider() = default;

		virtual inline Type GetType() override { return Type::BEHAVIOR; }
	};


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
	class RangableProvider : public Provider<Class, T, type>
	{
	public:
		RangableProvider(const std::string& name, Getter<Class, T>&& getter, Setter<Class, T>&& setter)
			: Provider<Class, T, type>(name, std::move(getter), std::move(setter)), hasRange(false) {}
		RangableProvider(const std::string& name, Getter<Class, T>&& getter, Setter<Class, T>&& setter, T min, T max)
			: Provider<Class, T, type>(name, std::move(getter), std::move(setter)), hasRange(true), min(min), max(max) {}
		virtual ~RangableProvider() = default;

		virtual inline void Set(Class * obj, T value) override { if (!hasRange || min <= value && value <= max) setter(obj, value); }

		virtual inline bool HasRange() { return hasRange; }
		virtual inline T GetMin() { return min; }
		virtual inline T GetMax() { return max; }

	protected:
		bool hasRange;
		T min, max;
	};


	template <class Class> using IntProvider	= RangableProvider<Class, int32_t,		CommonProvider::Type::INT>;
	template <class Class> using UIntProvider	= RangableProvider<Class, uint32_t,		CommonProvider::Type::UINT>;
	template <class Class> using FloatProvider	= RangableProvider<Class, float,		CommonProvider::Type::FLOAT>;

	template <class Class> using BoolProvider	= Provider<Class, bool,					CommonProvider::Type::BOOL>;
	template <class Class> using PointProvider	= Provider<Class, const Point&,			CommonProvider::Type::POINT>;
	template <class Class> using TrafoProvider	= Provider<Class, const Transform&,		CommonProvider::Type::TRAFO>;
	template <class Class> using ColorProvider	= Provider<Class, const Color&,			CommonProvider::Type::COLOR>;
	template <class Class> using StringProvider = Provider<Class, const std::string&,	CommonProvider::Type::STRING>;


	template <class Class>
	class UFloatProvider : public RangableProvider<Class, float, CommonProvider::Type::UFLOAT>
	{
	public:
		UFloatProvider(const std::string& name, Getter<Class, float>&& getter, Setter<Class, float>&& setter)
			: RangableProvider<Class, float, Type::UFLOAT>(name, std::move(getter), std::move(setter)) {}
		UFloatProvider(const std::string& name, Getter<Class, float>&& getter, Setter<Class, float>&& setter, float min, float max)
			: RangableProvider<Class, float, Type::UFLOAT>(name, std::move(getter), std::move(setter), min, max){}
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
			: Provider<Class, class Image*, Type::IMAGE>(name, std::move(getter), std::move(setter)), pathSetter(std::move(pathSetter)) {}
		virtual ~ImageProvider() = default;

		virtual inline void SetPath(Class* obj, const std::string& path, bool filtered) { pathSetter(obj, path, filtered); }

	protected:
		std::function<void(Class*, const std::string&, bool)> pathSetter;
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
}
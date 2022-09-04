#pragma once
#include <string>

namespace Sharpheus {

	class SPH_EXPORT EngineVersion
	{
	public:
		EngineVersion();
		EngineVersion(const std::string& vname);

		inline uint32 GetVNum() const { return vnum; }
		inline const std::string& GetVName() const { return vname; }

		inline bool operator==(const EngineVersion& other) const { return vnum == other.vnum; }
		inline bool operator!=(const EngineVersion& other) const { return vnum != other.vnum; }
		inline bool operator<(const EngineVersion& other) const { return vnum < other.vnum; }
		inline bool operator<=(const EngineVersion& other) const { return vnum <= other.vnum; }
		inline bool operator>(const EngineVersion& other) const { return vnum > other.vnum; }
		inline bool operator>=(const EngineVersion& other) const { return vnum >= other.vnum; }

		inline bool IsUnknown() const { return vnum == 0; }
		inline bool IsValid() const { return vnum != 0; }
		inline bool IsLatest() const { return vnum == latest.vnum; }
		inline bool IsAtLeast(uint32 vnum) const { return this->vnum >= vnum; }
		inline bool IsAtLeast(const std::string& vname) const { return IsAtLeast(GetVNumOf(vname)); };
		inline bool IsAtLeast(const EngineVersion& other) const { return *this >= other; };

		static inline uint32 GetVNumOf(const std::string& vname) {
			auto it = versions.find(vname);
			return (it == versions.end()) ? 0 : it->second;
		}

		inline static const EngineVersion& GetCurrent() { return current; }
		inline static void SetCurrent(const EngineVersion& curr) { current = curr; }

		static const EngineVersion latest;

	private:
		uint32 vnum;
		std::string vname;

		static EngineVersion current;
		static const std::unordered_map<std::string, uint32> versions;
	};

}


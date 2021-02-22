#pragma once


namespace Sharpheus {

	class EditorData
	{
	public:
		static void Init();
		static void Clear();

		static void SetLevel(class Level* _level);
		static inline class Level* GetLevel() { return level; }

		static void SetCurrent(const std::string& currName);
		static void SetCurrent(class GameObject* _curr);
		static inline class GameObject* GetCurrent() { return curr; }
	
	private:
		static class Level* level;
		static class GameObject* curr;
	};

}
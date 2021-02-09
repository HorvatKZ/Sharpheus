#pragma once


namespace Sharpheus {

	class EditorData
	{
	public:
		static void Init();
		static void Clear();

		static void SetLevel(class Level* _level);
		static inline class Level* GetLevel() { return level; }
	
	private:
		static class Level* level;
	};

}
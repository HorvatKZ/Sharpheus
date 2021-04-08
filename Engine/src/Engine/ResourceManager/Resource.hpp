#pragma once

#include "pch.h"


namespace Sharpheus {

	class SPH_EXPORT Resource
	{
	public:
		Resource();
		Resource(const std::string& path);
		virtual ~Resource();

		inline bool HasPath() { return !path.empty(); }
		inline const std::string& GetPath() { return path; }
		inline const std::string& GetFullPath() { return fullPath; }

	protected:
		std::string path, fullPath;

		void SetPath(const std::string& path);
	};

}
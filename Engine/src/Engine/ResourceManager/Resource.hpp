#pragma once

#include "pch.h"


namespace Sharpheus {

	class SPH_EXPORT Resource
	{
	public:
		Resource();
		Resource(const std::string& path);
		virtual ~Resource();

		inline bool IsValid() const { return valid; }
		inline bool HasPath() const { return !path.empty(); }
		inline const std::string& GetPath() const { return path; }
		inline const std::string& GetFullPath() const { return fullPath; }

	protected:
		std::string path, fullPath;
		bool valid = false;

		void SetPath(const std::string& path);
	};

}
#pragma once

#include "starter_pch.h"
#include "StarterWindow.hpp"


namespace Sharpheus {

	class StarterApp : public wxApp
	{
	public:
		StarterApp();
		virtual ~StarterApp();

		virtual bool OnInit();

	private:
		StarterWindow* win = nullptr;

		bool CheckCommonFiles();
	};

}
#pragma once

#include "editor_pch.h"
#include "EditorWindow.hpp"


namespace Sharpheus {

	class Editor : public wxApp
	{
	public:
		Editor();
		virtual ~Editor();

		virtual bool OnInit();

	private:
		EditorWindow* win = nullptr;
	};

}
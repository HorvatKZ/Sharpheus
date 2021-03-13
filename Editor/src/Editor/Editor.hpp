#pragma once

#include "editor_pch.h"
#include "EditorWindow/EditorWindow.hpp"


namespace Sharpheus {

	class Editor : public wxApp
	{
	public:
		Editor();
		virtual ~Editor();

		virtual bool OnInit() override;

	private:
		EditorWindow* win = nullptr;
	};

}
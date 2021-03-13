#include "starter_pch.h"
#include "StarterApp.hpp"
#include "ProcessControl.hpp"


wxIMPLEMENT_APP(Sharpheus::StarterApp);

namespace Sharpheus {

	StarterApp::StarterApp()
	{
		ProcessControl::Init();
	}


	StarterApp::~StarterApp()
	{
		ProcessControl::Clear();
	}


	bool StarterApp::OnInit()
	{
		wxInitAllImageHandlers();

		if (argc == 2) {
			ProcessControl::OpenFile(argv[1]);
		}

		win = new StarterWindow("Sharpheus Starter", 1000, 600);
		win->Show();

		return true;
	}
}
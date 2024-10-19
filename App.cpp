#include "App.h"
#include "MainFrame.h"
#include <wx/wx.h>

wxIMPLEMENT_APP(App);

bool App::OnInit() 
{
	MainFrame* mainFrame = new MainFrame("ATM Simulation");
	mainFrame->SetClientSize(wxSize(1300, 768));
	mainFrame->Center();
	mainFrame->Show();
	return true;
}
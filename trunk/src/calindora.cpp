#include <iostream>

#include "wx/wx.h"
#include "config.h"

class HelloWorldApp : public wxApp
{
	public:
		virtual bool OnInit();
};

DECLARE_APP(HelloWorldApp)

IMPLEMENT_APP(HelloWorldApp)

bool HelloWorldApp::OnInit()
{
	wxFrame *frame = new wxFrame((wxFrame*) NULL, -1, _("Hello wxWidgets World"));
	frame->CreateStatusBar();
	frame->SetStatusText(_("Hello World"));
	frame->Show(TRUE);
	SetTopWindow(frame);
	return true;
}

/*
int main ()
{
	std::cout << "Calindora IRC Client v" << VERSION << std::endl;
	return 0;
}
*/

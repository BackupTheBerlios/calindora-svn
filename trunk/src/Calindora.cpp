#include "wx/wx.h"
#include "config.h"
#include "Calindora.h"

IMPLEMENT_APP(Calindora)

bool Calindora::OnInit()
{
	wxFrame *frame = new wxFrame((wxFrame*) NULL, -1, _("Calindora " VERSION));
	frame->CreateStatusBar();
	frame->SetStatusText(_("Calindora IRC Client"));
	frame->Show(TRUE);
	SetTopWindow(frame);
	return true;
}

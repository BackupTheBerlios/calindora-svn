#ifndef CALINDORA_H
#define CALINDORA_H

#include "wx/wx.h"
#include "config.h"

class Calindora : public wxApp
{
	public:
		virtual bool OnInit();
};

DECLARE_APP(Calindora)

#endif

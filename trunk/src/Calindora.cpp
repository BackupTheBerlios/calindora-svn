//-----------------------------------------------------------------------------
// Calindora.cpp
//
// Date:        16 Jul 2004
// Copyright:   Copyright (C) Jason Lynch 2004
// Website:     http://calindora.berlios.de
// Author:      Jason Lynch (aexoden@aexoden.com)
//-----------------------------------------------------------------------------
// $Id$
//-----------------------------------------------------------------------------
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// For full license details, see COPYING.
//-----------------------------------------------------------------------------

#include "Calindora.h"

IMPLEMENT_APP(Calindora)

bool Calindora::OnInit()
{
	// Adjust logging to output to cerr, so debug messages are readily available.
	_logChain = new wxLogChain(new wxLogStream());
	
	_core = new Core();
	_frame = new ClientFrame(_core);
	
	_core->createServer();
	
	_frame->Show(TRUE);
	SetTopWindow(_frame);
	
	// Create a new server
	
	return true;
}

/*
wxString Core::getPreference(const wxString& section, const wxString& item)
{
	if (section == _("General"))
	{
		if (item == _("CommandPrefix"))
		{
				return _("/");
		}
	}
	else if (section == _("Server"))
	{
		if (item == _("ConnectionTimeout"))
		{
			return _("10"); 
		}
	}
	
	return _("");
}

void Core::setPreference(const wxString& section, const wxString& item, const wxString& data)
{
	// Do nothing yet
}
*/

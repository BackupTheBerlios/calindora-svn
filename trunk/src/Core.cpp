//-----------------------------------------------------------------------------
// Core.cpp
//
// Date:        19 Jul 2004
// Copyright:   Copyright (C) Jason Lynch 2004
// Website:     http://calindora.berlios.de
// Author:      Jason Lynch (aexoden@aexoden.com)
//-----------------------------------------------------------------------------
// $Id: Calindora.cpp 7 2004-07-18 03:58:08Z aexoden $
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

#include "Core.h"

Core::Core()
{
	_serverList = new std::list<Server*>();
}

Core::~Core()
{
	delete _serverList;
}

void Core::createServer()
{
	Server *server = new Server(this);
	_serverList->push_back(server);

	if (_view != NULL)
	{
		_view->onCoreNewServer(server);
	}
}

void Core::input(const wxString& input, Server *server)
{
	// Received a new input from some context.
	
	// Check for a server command
	if (input.Left(6) == _("server"))
	{
		wxIPaddress *address = new wxIPV4address();
		address->Hostname(_("irc.ffchat.net"));
		address->Service(6669);
		
		server->connect(address);
	}
	else
	{
		// We don't have anything else to do here at the moment, so we'll send it back down
		// to the server as a raw command..
		server->rawCommand(input);
	}
}

void Core::setView(CoreView *coreView)
{
	_view = coreView;
}

//-----------------------------------------------------------------------------
// Core.cpp
//
// Date:        19 Jul 2004
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

#include "Core.h"

#include "wx/tokenzr.h"

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
	// Check for a command.
	// TODO:
	// -- May want to detect the prefix as long as only whitespace precedes it.
	// -- Perhaps make it possible for prefix to be more than a single character.
	if (input.Left(1) == this->getPreference(_("General"), _("CommandPrefix")))
	{
		wxArrayString *tokens = new wxArrayString();
		
		wxStringTokenizer tokenizer(input, _(" \t"), wxTOKEN_STRTOK);
		while (tokenizer.HasMoreTokens())
		{
			tokens->Add(tokenizer.GetNextToken());
		}

		wxString command = (*tokens)[0];
		command = command.Right(command.length() - 1).MakeLower();
		
		if (command == _("server"))
		{
			// SERVER <server> <port>
			wxString hostname = (*tokens)[1];
			long port;
			(*tokens)[2].ToLong(&port, 10);
			
			wxIPaddress *address = new wxIPV4address();
			address->Hostname(hostname);
			address->Service(port);
			
			server->connect(address);
		}
		
		delete tokens;
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

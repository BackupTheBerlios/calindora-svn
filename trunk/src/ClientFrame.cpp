//-----------------------------------------------------------------------------
// ClientFrame.cpp
//
// Date:		22 Jul 2004
// Copyright:	Copyright (C) Jason Lynch 2004
// Website:		http://calindora.aexoden.com
// Author:		Jason Lynch (aexoden@aexoden.com)
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
// For full license details, see doc/COPYING.
//-----------------------------------------------------------------------------

#include "ClientFrame.h"

#include "wx/tokenzr.h"
#include "wx/regex.h"

ClientFrame::ClientFrame() : wxFrame(NULL, -1, _("Calindora " VERSION), wxDefaultPosition, wxDefaultSize)
{
	// Menus
	_menuBar = new wxMenuBar();

	// File menu
	_fileMenu = new wxMenu();
	_fileMenu->Append(MENU_FILE_EXIT, _("E&xit"));
	_menuBar->Append(_fileMenu, _("&File"));

	SetMenuBar(_menuBar);

	// Notebook control
	_notebook = new wxNotebook(this, -1);
	_notebookSizer = new wxNotebookSizer(_notebook);
	_notebook->Show(true);
	
	// Create the list of servers.
	_serverList = new std::list<Server*>();
}

ClientFrame::~ClientFrame()
{
	delete _serverList;
}

BEGIN_EVENT_TABLE(ClientFrame, wxFrame)
	EVT_MENU(MENU_FILE_EXIT, ClientFrame::OnMenuFileExit)
END_EVENT_TABLE()

/*
void ClientFrame::onCoreNewServer(Server *server)
{
	ServerPanel *serverPanel = new ServerPanel(_notebook, server);

	_notebook->AddPage(serverPanel, _("Server Panel"));	
}
*/

void ClientFrame::OnMenuFileExit(wxCommandEvent &event)
{
	Close(false);
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
			// SERVER [-n] <server> <port>
			wxRegEx reServer(this->getPreference(_("General"), _("CommandPrefix")) + wxT("server( (-n))?( ([[:alnum:].-]+))?((:| )([0-9]+))?"));
			if (reServer.Matches(input))
			{
				if (reServer.GetMatch(input, 2) == wxT("-n"))
				{
					// Create a new server object.
					server = this->createServer();
				}
				wxString hostname = reServer.GetMatch(input, 4);
				long port;
				reServer.GetMatch(input, 7).ToLong(&port, 10);
				
				wxIPaddress *address = new wxIPV4address();
				address->Hostname(hostname);
				address->Service(port);
				
				server->connect(address);
			}
			else
			{
				//_view->onServerMessage(_("Invalid command syntax."));	
			}
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

void ClientFrame::createServer()
{
	Server *server = new Server(this);
	_serverList->push_back(server);

	if (_view != NULL)
	{
		_view->onCoreNewServer(server);
	}
	
	return server;
}

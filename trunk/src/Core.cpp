//-----------------------------------------------------------------------------
// Core.cpp
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

#include <wx/wx.h>
#include <wx/regex.h>

#include "config.h"

#include "Server.h"

#include "Core.h"

BEGIN_EVENT_TABLE(Core, wxFrame)
	EVT_MENU(MENU_FILE_EXIT, Core::OnMenuFileExit)
END_EVENT_TABLE()

Core::Core() : wxFrame(NULL, -1, _("Calindora " VERSION), wxDefaultPosition, wxDefaultSize)
{
	// Create the menubar.
	_menuBar = new wxMenuBar();
	
	// File menu.
	_fileMenu = new wxMenu();
	_fileMenu->Append(MENU_FILE_EXIT, _("E&xit"));
	_menuBar->Append(_fileMenu, _("&File"));
	
	SetMenuBar(_menuBar);
	
	// Notebook control.
	_notebook = new wxNotebook(this, -1);
	_notebookSizer = new wxNotebookSizer(_notebook);
	_notebook->Show(true);
	
	// Create the list of servers.
	_serverList = new std::list<Server*>();
}

Core::~Core()
{
	delete _serverList;
}

void Core::OnMenuFileExit(wxCommandEvent &event)
{
	this->Close(false);
}

Server* Core::createServer()
{
	Server *server = new Server(_notebook, this);
	_notebook->AddPage(server, _("Default Text"));
	
	_serverList->push_back(server);
	
	return server;
}

void Core::processInput(const wxString& input, Server *server)
{
	// Check for a command, and process it accordingly.
	// TODO:
	// -- Want to delegate this processing to the script engine, when it's eventually put in place.
	// -- May want to detect the prefix as long as it's only preceded by whitespace.
	// -- May make it possible for the prefix to be more than one character.
	// -- Extract the prefix from a preference.
	if (input.Left(1) == wxT("/"))
	{
		wxRegEx re;
		
		// SERVER [-n] <server> <port>
		re.Compile(wxT("/server( (-n))?( ([[:alnum:].-]+))?((:| )([0-9]+))?"));
		if (re.Matches(input))
		{
			if (re.GetMatch(input, 2) == wxT("-n"))
			{
				// Create a new server object.
				server = this->createServer();
			}
			
			wxString hostname = re.GetMatch(input, 4);
			long port;
			re.GetMatch(input, 7).ToLong(&port, 10);
			
			// We allocate the memory for the address, but the server is responsible for any deletion
			wxIPaddress *address = new wxIPV4address();
			address->Hostname(hostname);
			address->Service(port);
			
			server->connect(address);
			
			return;
		}
		
		// TODO:
		// -- Print some kind of error message if the command is invalid.
	}
	else
	{
		// TODO:
		// -- Send the command directly to the context as a raw command. This means messages typed in a channel
		//    will go to that channel. Messages typed in a server window will be sent as raw commands.
	}
}

/*
#include "wx/tokenzr.h"
#include "wx/regex.h"

void Core::input(const wxString& input, Server *server)
{
	// Check for a command.
	// TODO:
	// -- May want to detect the prefix as long as only whitespace precedes it.
	// -- Perhaps make it possible for prefix to be more than a single character.
	if (input.Left(1) == wxT("/")) // this->getPreference(_("General"), _("CommandPrefix")))
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
			wxRegEx reServer(wxT("/server( (-n))?( ([[:alnum:].-]+))?((:| )([0-9]+))?"));
		}
		
		delete tokens;
	}
}


*/

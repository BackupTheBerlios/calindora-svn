//-----------------------------------------------------------------------------
// Server.cpp
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

#include <iostream>

#include "Server.h"

// wxWidgets event table
BEGIN_EVENT_TABLE(Server, wxEvtHandler)
	EVT_SOCKET(SOCKET_ID, Server::onSocketEvent)
END_EVENT_TABLE()

int Server::nextID = 0;

Server::Server(Core *core)
{
	_core = core;
	_id = Server::nextID;
	Server::nextID++;
	
	// Create the socket and set the event handler.
	_socket = new wxSocketClient();
	_socket->SetEventHandler(*this, SOCKET_ID);
	_socket->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_OUTPUT_FLAG);
	_socket->Notify(true);
	
	_connected = false;
	_currentServer = NULL;
}

Server::~Server()
{
	delete _socket;
}

void Server::connect(wxIPaddress *server)
{
	if (_currentServer != NULL)
	{
		delete _currentServer;
	}
	_currentServer = server;

	_socket->Connect(*_currentServer, false);
	
	// Add a message indicating we're connecting.
	_view->onServerMessage(_("Connecting to server..."));
}

void Server::disconnect()
{
	_socket->Close();
}

void Server::setView(ServerView *view)
{
	_view = view;
}

void Server::onSocketEvent(wxSocketEvent& event)
{
	switch (event.GetSocketEvent())
	{
		case wxSOCKET_CONNECTION:
			_view->onServerMessage(_("Successfully connected."));
			_connected = true;
			break;
			
		case wxSOCKET_LOST:
			_view->onServerMessage(_("Failed to connect or connection lost."));
			_connected = false;
			break;
			
		case wxSOCKET_INPUT:
			{
				char* buffer = new char[512];
				_socket->Read(buffer, 512);
				wxString *input = new wxString(buffer, wxConvUTF8);
				_view->onServerMessage(*input);
				delete input;
				delete buffer;
			}
			break;
			
		case wxSOCKET_OUTPUT:
			break;
			
		default:
			break;
			
	}
}

void Server::onInput(const wxString& input)
{
	_view->onServerMessage(input);
	
	// We probably don't have any processing of our own, so we'll delegate it to the core.
	_core->input(input, this);
}

void Server::rawCommand(const wxString& input)
{
	if (_connected == true)
	{
		// For now, basic IO. Should eventually make sure the operations succeed.
		const wxWX2MBbuf buffer = input.mb_str(wxConvUTF8);
		int byteCount = strlen(buffer);
		
		wxString *string = new wxString();
		string->Printf(_("Writing %d bytes to socket."), byteCount);
		_view->onServerMessage(*string);
		delete string;
		
		_socket->Write(buffer, byteCount);
		_socket->Write("\n", 1);
	}
	else
	{
		// Display an error message, I suppose.
		_view->onServerMessage(_("Not connected."));
	}
}

bool Server::operator== (const Server& right) const
{
	if (this->_id == right._id)
	{
		return true;
	}
	else
	{
		return false;
	}
}

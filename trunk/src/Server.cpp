//-----------------------------------------------------------------------------
// Server.cpp
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
	
	_status = STATUS_DISCONNECTED;
	_currentServer = NULL;
}

Server::~Server()
{
	_socket->Destroy();
}

void Server::connect(wxIPaddress *server)
{
	if (_currentServer != NULL)
	{
		delete _currentServer;
	}
	_currentServer = server;

	_status = STATUS_CONNECTING;
	_socket->Connect(*_currentServer, false);
	
	_view->onServerMessage(_("Connecting to server..."));
	
	if (_socket->WaitOnConnect(10) == true && _socket->IsConnected())
	{
		_view->onServerMessage(_("Successful connection!"));
		_status = STATUS_CONNECTED;
	}
	else
	{
		_view->onServerMessage(_("Failed to connect!"));
		_status = STATUS_DISCONNECTED;
	}
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
			// Nothing to handle here.
			break;
			
		case wxSOCKET_LOST:
			if (_status != STATUS_CONNECTING)
			{
				_view->onServerMessage(_("Connection lost."));
			}
			_status = STATUS_DISCONNECTED;
			break;
			
		case wxSOCKET_INPUT:
			{
				char readBuffer[1024];
				std::string input;
				size_t index;
				_socket->Read(readBuffer, 1024);
				_inputBuffer.append(readBuffer, _socket->LastCount());
				
				index = _inputBuffer.find_first_of('\n');
				while (index != std::string::npos)
				{
					// Get the input substring.
					input = _inputBuffer.substr(0, index-1);
					
					// Remove any extra carriage returns.
					while (input[0] == '\r')
					{
						input = input.substr(1);
					}
					
					// Send the incoming data to the view.
					wxString *message = new wxString(input.c_str(), wxConvUTF8);
					if (message->Len() == 0)
					{
						delete message;
						//wxCSConv *conv = new wxCSConv(_("windows-1252"));
						message = new wxString(input.c_str(), wxConvISO8859_1);
						//delete conv;
					}
					_view->onServerMessage(*message);
					delete message;
					
					// Alter the input buffer to remove this line.
					_inputBuffer = _inputBuffer.substr(index + 1);
					index = _inputBuffer.find_first_of('\n');
				}
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
	if (_status == STATUS_CONNECTED)
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

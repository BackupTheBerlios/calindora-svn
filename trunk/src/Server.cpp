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

#include <wx/wx.h>

#include "Core.h"

#include "Server.h"

Server::Server(wxWindow *parent, Core *core) : wxPanel(parent)
{
	_core = core;
	
	_outputControl = new wxTextCtrl(this, -1, _(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_RICH | wxTE_READONLY);
	_inputControl = new wxTextCtrl(this, -1, _(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	
	_sizer = new wxBoxSizer(wxVERTICAL);
	
	this->SetSizer(_sizer);
	
	_sizer->Add(_outputControl, 1, wxEXPAND);
	_sizer->Add(_inputControl, 0, wxEXPAND);
}

Server::~Server()
{
	
}

/*
#include <iostream>

#include "Server.h"

#include "Core.h"

// wxWidgets event table
BEGIN_EVENT_TABLE(Server, wxEvtHandler)
	EVT_SOCKET(SOCKET_ID, Server::onSocketEvent)
	EVT_TEXT_ENTER(CONTROL_TEXT_INPUT, Server::OnInput)
END_EVENT_TABLE()

int Server::nextID = 0;

Server::Server(wxWindow *parent, Core *core) : wxPanel(parent)
{
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
	
	//_view->onServerMessage(_("Connecting to server..."));
	
	long timeout;
	//if (!(_core->getPreference(_("Server"), _("ConnectionTimeout")).ToLong(&timeout, 10)))
	//{
		timeout = 10;
	//}
	if (_socket->WaitOnConnect(timeout) == true && _socket->IsConnected())
	{
		//_view->onServerMessage(_("Successful connection!"));
		_status = STATUS_CONNECTED;
	}
	else
	{
		//_view->onServerMessage(_("Failed to connect!"));
		_status = STATUS_DISCONNECTED;
	}
}

void Server::disconnect()
{
	_socket->Close();
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
				//_view->onServerMessage(_("Connection lost."));
			}
			_status = STATUS_DISCONNECTED;
			break;
			
		case wxSOCKET_INPUT:
			{
				char readBuffer[1024];
				std::string input;
				size_t index;
				_socket->Read(readBuffer, 1024);
				
				if (_socket->Error())
				{
					wxSocketError error = _socket->LastError();
					
					//_view->onServerMessage(_("***** A socket I/O error has occured."));
				}
				
				_inputBuffer.append(readBuffer, _socket->LastCount());
				
				index = _inputBuffer.find_first_of('\n');
				while (index != std::string::npos)
				{
					// Get the input substring.
					input = _inputBuffer.substr(0, index);
					
					// Remove any extra carriage returns.
					// RFC 1459 says lines are terminated with a CRLF, but looping safeguards against extra.
					// May want to add a similar check at the beginning of the string, and change it to completely
					// eliminate all newline characters. Depends on how well different servers conform.
					while (input[input.length()-1] == '\r')
					{
						input = input.substr(0, input.length() - 1);
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
					//_view->onServerMessage(*message);
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

void Server::OnInput(wxCommandEvent& event)
{
	wxString text = _inputControl->GetLineText(0);
	
	_inputControl->Clear();	
	
	// We probably don't have any processing of our own, so we'll delegate it to the core.
	_core->input(text, this);
}

void Server::rawCommand(const wxString& input)
{
	if (_status == STATUS_CONNECTED)
	{
		// Convert the input buffer into a UTF-8 multibyte string. More sophisticated character set
		// handling may eventually be needed.
		const wxWX2MBbuf buffer = input.mb_str(wxConvUTF8);
		int byteCount = strlen(buffer);
		
		// Write the buffer to the socket and check for an error. Better error handling should be a todo.
		_socket->Write(buffer, byteCount);
		if (_socket->Error())
		{
			wxSocketError error = _socket->LastError();
			
		//	_view->onServerMessage(_("***** A socket I/O error has occured."));
		}
		
		// Finish the write with a newline. May be more practical to append to the buffer and only write once.
		_socket->Write("\n", 1);
		if (_socket->Error())
		{
			wxSocketError error = _socket->LastError();
			
			//_view->onServerMessage(_("***** A socket I/O error has occured."));
		}
	}
	else
	{
	//	_view->onServerMessage(_("Not connected to a server."));
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
*/

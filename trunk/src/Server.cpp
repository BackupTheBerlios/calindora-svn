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

// wxWidgets event table.
BEGIN_EVENT_TABLE(Server, wxPanel)
	EVT_SOCKET(SOCKET_ID, Server::OnSocketEvent)
	EVT_TEXT_ENTER(CONTROL_TEXT_INPUT, Server::OnInput)
END_EVENT_TABLE()

Server::Server(wxWindow *parent, Core *core) : wxPanel(parent)
{
	_core = core;
	
	_outputControl = new wxTextCtrl(this, -1, _(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_RICH | wxTE_READONLY);
	_inputControl = new wxTextCtrl(this, CONTROL_TEXT_INPUT, _(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	
	_sizer = new wxBoxSizer(wxVERTICAL);
	
	this->SetSizer(_sizer);
	
	_sizer->Add(_outputControl, 1, wxEXPAND);
	_sizer->Add(_inputControl, 0, wxEXPAND);
	
	_socket = new wxSocketClient();
	_socket->SetEventHandler(*this, SOCKET_ID);
	_socket->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_OUTPUT_FLAG);
	_socket->Notify(true);
	
	_socketStatus = STATUS_DISCONNECTED;
	_currentAddress = NULL;
}

Server::~Server()
{
	// Need to call Destroy to clean up wxWidgets event handling. wxWidgets will free the memory.
	_socket->Destroy();
}

void Server::OnInput(wxCommandEvent &event)
{
	wxString text = _inputControl->GetLineText(0);
	
	_inputControl->Clear();
	
	// Send the input to the core for processing.
	_core->processInput(text, this);
}

void Server::OnSocketEvent(wxSocketEvent &event)
{
	switch (event.GetSocketEvent())
	{
		case wxSOCKET_CONNECTION:
			// Nothing to handle here.
			break;
			
		case wxSOCKET_LOST:
			if (_socketStatus != STATUS_CONNECTING)
			{
				(*_outputControl) << _("Connection to server lost.\n");
			}
			_socketStatus = STATUS_DISCONNECTED;
			break;
			
		case wxSOCKET_INPUT:
			{
				char readBuffer[1024];
				std::string input;
				size_t index;
				
				// Read the first 1024 bytes into our buffer.
				_socket->Read(readBuffer, 1024);
				
				// Check for a socket error.
				if (!this->checkSocketError());
				{
					// Append the read buffer into the input buffer.
					_inputBuffer.append(readBuffer, _socket->LastCount());
					
					// If a newline appears in the input buffer, we have a complete line to parse.
					index = _inputBuffer.find_first_of('\n');
					while (index != std::string::npos)
					{
						// Get the input substring prior to the first newline
						input = _inputBuffer.substr(0, index);
						
						// Remove any carriage returns at the beginning or end of the string.
						// RFC 1459 says that lines should be terminated with a CRLF, but at least one server
						// has been known to violate this, so this is a nice safety measure.
						while (input[input.length()-1] == '\r')
						{
							input = input.substr(0, input.length() - 1);
						}
						while (input[0] == '\r')
						{
							input = input.substr(1);
						}
						
						// Convert the line as if it were ISO-8859-1. Later, individual channels can reinterpret
						// the encoding as necessary.
						// TODO: ISO-8859-1 may be a problem for UTF-16 strings, so we may want to use a character
						//       buffer of some sort that remembers its length. This has ramifications for the above
						//       as well, if null bytes appear in random locations, and affect the C++ strings.
						wxString *message = new wxString(input.c_str(), wxConvISO8859_1);
						
						// Print the line to our window. 
						// TODO: This is temporary until channels are available. Want to build a surrogate function
						//       that can dispatch the message to the appropriate channel(s).
						(*_outputControl) << (*message) << wxT("\n");
						delete message;
						
						// Adjust the input buffer to remove the line we just processed.
						_inputBuffer = _inputBuffer.substr(index + 1);
						index = _inputBuffer.find_first_of('\n');
					}
				}
			}
			break;

		case wxSOCKET_OUTPUT:
			break;
			
		default:
			break;
	}
}


void Server::rawCommand(const wxString& input)
{
	// If we're not connected to a server, we shouldn't bother sending the command.
	if (_socketStatus == STATUS_CONNECTED)
	{
		// Convert the input to the desired encoding. For now, that's UTF-8.
		// TODO:
		// -- Determine the encoding based on a preference.
		// -- Determine the semantics of mb_str(). We need some way to handle invalid
		//    characters for a particular encoding.
		const wxWX2MBbuf buffer = input.mb_str(wxConvUTF8);
		int byteCount = strlen(buffer);
		
		// Write the buffer to the socket and check for an error.
		_socket->Write(buffer, byteCount);
		if (!this->checkSocketError())
		{
			_socket->Write("\n", 1);
			this->checkSocketError();
		}
	}
	else
	{
		(*_outputControl) << _("Error: Not connected to a server.\n");
	}
}

bool Server::checkSocketError()
{
	if (_socket->Error())
	{
		wxSocketError error = _socket->LastError();
		
		// Based on the error, create an error message.
		// TODO:
		// -- The wxSOCKET_WOULDBLOCK error may have a more appropriate solution, depending
		//    on the exact semantics of when it occurs.
		wxString errorMessage;
		switch (error)
		{
			case wxSOCKET_NOERROR:
				errorMessage = _("There was a socket error, but the error code is that of no error. This is probably a bug.");
				break;
				
			case wxSOCKET_INVOP:
				errorMessage = _("Invalid operation.");
				break;
				
			case wxSOCKET_IOERR:
				errorMessage = _("I/O error.");
				break;
				
			case wxSOCKET_INVADDR:
				errorMessage = _("Invalid address.");
				break;
				
			case wxSOCKET_INVSOCK:
				errorMessage = _("Invalid socket.");
				break;
			
			case wxSOCKET_NOHOST:
				errorMessage = _("No corresponding host.");
				break;
			
			case wxSOCKET_INVPORT:
				errorMessage = _("Invalid port.");
				break;
			
			case wxSOCKET_WOULDBLOCK:
				errorMessage = _("The socket is non-blocking and the operation would block.");
				break;
			
			case wxSOCKET_TIMEDOUT:
				errorMessage = _("The timeout for this operation expired.");
				break;
			
			case wxSOCKET_MEMERR:
				errorMessage = _("Memory exhausted.");
				break;
				
			default:
				errorMessage = _("An unknown error has occured.");
				break;
		}
		
		(*_outputControl) << _("Socket error: ") << errorMessage << wxT("\n");
		
		return true;
	}
	else
	{
		return false;
	}
}

void Server::connect(wxIPaddress *address)
{
	// If our current address isn't null, we should free its memory first.
	if (_currentAddress != NULL)
	{
		delete _currentAddress;
	}
	_currentAddress = address;
	
	// Set our status to connecting and initiate the connection.
	_socketStatus = STATUS_CONNECTING;
	_socket->Connect(*_currentAddress, false);
	
	// Print a status message to our own window.
	(*_outputControl) << _("Connecting to ") << (_currentAddress->Hostname()) << wxT(":") << (_currentAddress->Service()) << wxT("\n");
	
	// Set the timeout. 
	// TODO: This should be extracted from a preference as soon as that framework is ready.
	long timeout = 10;
	
	// Wait on the connection up to the timeout, and set the socket status as appropriate.
	// TODO:
	// -- There may be a GUI blocking problem here. Some solutions include splitting the socket code
	//    off into another thread, but I'd like to avoid that if possible. Alternately, we can get rid
	//    of the custom timeout, but I don't particularly like that, either. The other option is some
	//    kind of timed callback. If we want timers in the scripting language, that may be needed anyway.
	if (_socket->WaitOnConnect(timeout) == true && _socket->IsConnected())
	{
		(*_outputControl) << _("Successful connection!\n");
		_socketStatus = STATUS_CONNECTED;
	}
	else
	{
		(*_outputControl) << _("Failed to connect!\n");
		_socketStatus = STATUS_DISCONNECTED;
	}
}

void Server::disconnect()
{
	if (_currentAddress != NULL)
	{
		delete _currentAddress;
	}
	_socket->Close();
}

/*
int Server::nextID = 0;

Server::Server(wxWindow *parent, Core *core) : wxPanel(parent)
{
	_id = Server::nextID;
	Server::nextID++;
}
*/

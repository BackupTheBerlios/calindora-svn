//-----------------------------------------------------------------------------
// Server.h
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

#ifndef SERVER_H
#define SERVER_H

#include <string>

#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/socket.h>
#include <wx/textctrl.h>
#include <wx/window.h>

class Core;

class Server : public wxPanel
{
	public:
		Server(wxWindow *parent, Core *core);
		~Server();
		
		// wxWidgets event handlers.
		void OnInput(wxCommandEvent& event);
		void OnSocketEvent(wxSocketEvent& event);
		
		// Socket related methods
		bool checkSocketError();
		void connect(wxIPaddress *address);
		void disconnect();
		
	private:
		// Determines the current state of the socket.
		int _socketStatus;
		
		std::string _inputBuffer;
	
		Core *_core;
		
		wxIPaddress *_currentAddress;
		wxSizer *_sizer;
		wxSocketClient *_socket;
		wxTextCtrl *_outputControl;
		wxTextCtrl *_inputControl;
		
		// Declare the wxWidgets event table.
		DECLARE_EVENT_TABLE()
		
		enum
		{
			CONTROL_TEXT_INPUT = 1000,
			
			SOCKET_ID,
			
			STATUS_DISCONNECTED,
			STATUS_CONNECTING,
			STATUS_CONNECTED
		};
};

/*
class Core;

#include <string>

class Server : public wxPanel
{
	public:
		bool operator== (const Server& right) const;
		void rawCommand(const wxString& input);
		
	private:
		// Buffer to hold incoming data until we have a full line.
		std::string _inputBuffer;		
		int _status;
		static int nextID;
};
*/

#endif

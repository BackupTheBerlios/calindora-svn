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

class Core;

#include <string>

#include "wx/wx.h"

#include "wx/event.h"
#include "wx/socket.h"

#include "Core.h"
#include "ServerView.h"

class Server : public wxPanel
{
	public:
		Server(wxWindow *parent);
		~Server();
		
		bool operator== (const Server& right) const;
		
		void connect(wxIPaddress *server);
		void disconnect();
		void setView(ServerView *serverView);
		void onSocketEvent(wxSocketEvent& event);
		
		// Called whenever an associated ServerView receives a new input line.
		void onInput(const wxString& input);
		void rawCommand(const wxString& input);
		
	private:
		wxSocketClient *_socket;
		
		// Using wxIPaddress to presumably pave the way for future IPV6 support.
		wxIPaddress *_currentServer;
		
		// Buffer to hold incoming data until we have a full line.
		std::string _inputBuffer;		
		int _status;
		static int nextID;
		
		ServerView *_view;
		Core *_core;
		
		int _id;
		
		// Declare wxWidgets event table
		DECLARE_EVENT_TABLE()
		
		// IDs for various items
		enum
		{
			SOCKET_ID = 1000,
			
			STATUS_DISCONNECTED,
			STATUS_CONNECTING,
			STATUS_CONNECTED,
			
			CONTROL_TEXT_INPUT
		};
		
		wxTextCtrl *_textControl;
		wxTextCtrl *_inputControl;
		wxSizer *_sizer;
};

#endif

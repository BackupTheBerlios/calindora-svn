//-----------------------------------------------------------------------------
// ServerPanel.h
//
// Date:		25 Jul 2004
// Copyright:	Copyright (C) Jason Lynch 2004
// Website:		http://calindora.aexoden.com
// Author:		Jason Lynch (aexoden@aexoden.com)
//-----------------------------------------------------------------------------
// $Id: contentwindow.h 3 2004-05-25 23:04:18Z Jason Lynch $
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

#ifndef SERVERPANEL_H
#define SERVERPANEL_H

#include "wx/wx.h"

#include "Server.h"
#include "ServerView.h"

class ServerPanel : public wxPanel, public ServerView
{
	public:
		ServerPanel(wxWindow *parent, Server *server);
		~ServerPanel();

		void OnInput(wxCommandEvent &event);
		void onServerMessage(const wxString& message);

	protected:
		DECLARE_EVENT_TABLE()

	private:
		wxTextCtrl *_textControl;
		wxTextCtrl *_inputControl;
		wxSizer *_sizer;
		
		Server *_server;
		
		enum
		{
			CONTROL_TEXT_INPUT
		};
};

#endif

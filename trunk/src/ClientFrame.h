//-----------------------------------------------------------------------------
// ClientFrame.h
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

#ifndef CLIENTFRAME_H
#define CLIENTFRAME_H

#include <list>

#include "wx/wx.h"
#include "wx/notebook.h"

#include "config.h"

#include "ServerPanel.h"

class ClientFrame : public wxFrame
{
	public:
		ClientFrame();
		~ClientFrame();
		
		void OnMenuFileExit(wxCommandEvent &event);
		
		// Triggered on input. Indicates the context of the input. For now, just servers, but later
		// a more elaborate solution will be needed to handle channels or other windows as well.
		void input(const wxString& input, Server *server);		

	protected:
		DECLARE_EVENT_TABLE()

	private:
		wxNotebook *_notebook;
		wxNotebookSizer *_notebookSizer;

		wxMenuBar *_menuBar;
		wxMenu *_fileMenu;
		
		std::list<Server*> *_serverList;		
		
		void createServer();
		
		enum
		{
			MENU_FILE_EXIT
		};
};

#endif

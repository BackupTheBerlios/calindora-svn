//-----------------------------------------------------------------------------
// Core.h
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

#ifndef CORE_H
#define CORE_H

#include <list>

#include <wx/event.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/sizer.h>

class Server;

class Core : public wxFrame
{
	public:
		Core();
		~Core();
		
		// wxWidgets event handlers
		void OnMenuFileExit(wxCommandEvent &event);
		
		// Other various methods.
		Server* createServer();
		void processInput(const wxString& input, Server *server);
		
	protected:
		DECLARE_EVENT_TABLE()
		
	private:
		// Menu-related items.
		wxMenuBar *_menuBar;
		wxMenu *_fileMenu;
		
		wxNotebook *_notebook;
		wxNotebookSizer *_notebookSizer;
		
		std::list<Server*> *_serverList;
		
		// Various constants
		enum
		{
			MENU_FILE_EXIT = 1000
		};
};

#endif

//-----------------------------------------------------------------------------
// ClientFrame.cpp
//
// Date:		22 Jul 2004
// Copyright:	Copyright (C) Jason Lynch 2004
// Website:		http://calindora.aexoden.com
// Author:		Jason Lynch (aexoden@aexoden.com)
//-----------------------------------------------------------------------------
// $Id: clientframe.cpp 3 2004-05-25 23:04:18Z Jason Lynch $
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

#include "ClientFrame.h"

ClientFrame::ClientFrame(Core* core) : wxFrame(NULL, -1, _("Calindora " VERSION), wxDefaultPosition, wxDefaultSize)
{
	_core = core;
	_core->setView(this);
	
	// Menus
	_menuBar = new wxMenuBar();

	// File menu
	_fileMenu = new wxMenu();
	_fileMenu->Append(MENU_FILE_EXIT, _("E&xit"));
	_menuBar->Append(_fileMenu, _("&File"));

	SetMenuBar(_menuBar);

	// Notebook control
	_notebook = new wxNotebook(this, -1);
	_notebookSizer = new wxNotebookSizer(_notebook);
	_notebook->Show(true);

	// Temporary control
	//ContentWindow *cw = new ContentWindow(_notebook);
	//_notebook->AddPage(cw, _("Default Window"));
	
	// Scan the core and determine its current status.
	/* const std::list<Server> * const serverList = _core->getServerList();  
	std::list<Server>::iterator iter;
	
	for (iter = serverList->begin(); iter != serverList->end(); iter++)
	{
		this->onCoreNewServer(*iter);
	}
	this->onCoreNewServer(*iter); */
}

ClientFrame::~ClientFrame()
{

}

BEGIN_EVENT_TABLE(ClientFrame, wxFrame)
	EVT_MENU(MENU_FILE_EXIT, ClientFrame::OnMenuFileExit)
END_EVENT_TABLE()

void ClientFrame::onCoreNewServer(Server *server)
{
	ServerPanel *serverPanel = new ServerPanel(_notebook, server);

	_notebook->AddPage(serverPanel, _("Server Panel"));	
}

void ClientFrame::OnMenuFileExit(wxCommandEvent &event)
{
	Close(false);
}

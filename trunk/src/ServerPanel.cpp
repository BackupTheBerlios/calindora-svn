//-----------------------------------------------------------------------------
// ServerPanel.cpp
//
// Date:		25 Jul 2004
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

#include "ServerPanel.h"

ServerPanel::ServerPanel(wxWindow *parent, Server *server) : wxPanel(parent)
{
	_server = server;
	_server->setView(this);

	_textControl = new wxTextCtrl(this, -1, _(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_RICH); // | wxTE_READONLY);

	_inputControl = new wxTextCtrl(this, CONTROL_TEXT_INPUT, _(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);

	_sizer = new wxBoxSizer(wxVERTICAL);

	this->SetAutoLayout(true);
	this->SetSizer(_sizer);

	_sizer->Add(_textControl, 1, wxGROW);
	_sizer->Add(_inputControl, 0, wxGROW);
}

ServerPanel::~ServerPanel()
{

}

BEGIN_EVENT_TABLE(ServerPanel, wxPanel)
	EVT_TEXT_ENTER(CONTROL_TEXT_INPUT, ServerPanel::OnInput)
END_EVENT_TABLE()

void ServerPanel::OnInput(wxCommandEvent &event)
{
	wxString text = _inputControl->GetLineText(0);
	
	_inputControl->Clear();	
	
	_server->onInput(text);
}

void ServerPanel::onServerMessage(const wxString& message)
{
	(*_textControl) << message << _("\n");	
}

//-----------------------------------------------------------------------------
// Core.h
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

#ifndef CORE_H
#define CORE_H

#include <list>

#include "wx/wx.h"
#include "config.h"

#include "CoreView.h"
#include "Server.h"

class Core
{
	public:
		Core();
		~Core();
		
		void createServer();
		const std::list<Server*> * const getServerList() { return _serverList; }
		void setView(CoreView *view);
		
		// Configuration access and setting commands. Anything that wants to set preferences is able to.
		// This allows the GUI, for example, to provide an interface for setting configuration values.
		// TODO:
		// -- Add support for per-server and per-channel configurations.
		// -- Develop a good configuration hierarchy.
		// -- Support XML based configuration storage.
		// -- Provide an interface for working with the configuration hierarchy. For example, different kinds of
		//    options (yes/no, textbox) and so on, each with defined values. This could allow a GUI to generate
		//    a configuration dialog on the fly, without needing to know about each option.
		// -- Add options for configuration detail level, allowing novices to suppress less-used options.
		wxString getPreference(const wxString& section, const wxString& item);
		void setPreference(const wxString& section, const wxString& item, const wxString& data);
		
		// Triggered on input. Indicates the context of the input. For now, just servers, but later
		// a more elaborate solution will be needed to handle channels or other windows as well.
		void input(const wxString& input, Server *server);
		
	private:
		std::list<Server*> *_serverList;
		
		CoreView *_view;
};

#endif

//-----------------------------------------------------------------------------
// Core.h
//
// Date:        19 Jul 2004
// Copyright:   Copyright (C) Jason Lynch 2004
// Website:     http://calindora.berlios.de
// Author:      Jason Lynch (aexoden@aexoden.com)
//-----------------------------------------------------------------------------
// $Id: Calindora.h 7 2004-07-18 03:58:08Z aexoden $
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
		
		// Triggered on input. Indicates the context of the input. For now, just servers, but later
		// a more elaborate solution will be needed to handle channels or other windows as well.
		void input(const wxString& input, Server *server);
		
	private:
		std::list<Server*> *_serverList;
		
		CoreView *_view;
};

#endif

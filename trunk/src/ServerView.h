//-----------------------------------------------------------------------------
// ServerView.h
//
// Date:        21 Jul 2004
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

#ifndef SERVERVIEW_H
#define SERVERVIEW_H

#include "wx/wx.h"

class ServerView
{
	public:
		//virtual ChannelView* newChannelView(Server* server) = 0;
		virtual void onServerMessage(const wxString& message) = 0;
		
	private:
};

#endif

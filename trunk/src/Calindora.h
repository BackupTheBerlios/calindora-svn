//-----------------------------------------------------------------------------
// Calindora.h
//
// Date:        16 Jul 2004
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

#ifndef CALINDORA_H
#define CALINDORA_H

#include "wx/wx.h"
#include "config.h"

#include "Core.h"

#include "ClientFrame.h"

class Calindora : public wxApp
{
	public:
		virtual bool OnInit();
		
	private:
		ClientFrame *_frame;
		Core *_core;
};

DECLARE_APP(Calindora)

#endif

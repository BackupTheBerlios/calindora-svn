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

#include "ClientFrame.h"

class Calindora : public wxApp
{
	public:
		virtual bool OnInit();
		
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
		//wxString getPreference(const wxString& section, const wxString& item);
		//void setPreference(const wxString& section, const wxString& item, const wxString& data);
		
	private:
		ClientFrame *_frame;
		Core *_core;
		
		wxLogChain *_logChain;
};

DECLARE_APP(Calindora)

#endif

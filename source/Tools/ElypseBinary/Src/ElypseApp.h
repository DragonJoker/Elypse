/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___ELYPSE_APP_H___
#define ___ELYPSE_APP_H___

#include "ElypseBinaryPrerequisites.h"

#include <Module_Main.h>

namespace ElypseBinary
{
	class ElypseApp
		: public wxApp
	{
	private:
		ElypseFrame * m_frame;
		ElypseCtrl * m_control;
		GuiCommon::wxElypsePluginSPtr m_plugin;
		std::shared_ptr< ElypseInstance > m_elypse;

		uint32_t m_width;
		uint32_t m_height;

	private:
		void _readConfigFile( String const & p_filepath );

	public:
		virtual bool OnInit();
		virtual bool InitParams( String const & p_filepath );
		virtual int OnExit();

		void Close();
	};
}

#endif

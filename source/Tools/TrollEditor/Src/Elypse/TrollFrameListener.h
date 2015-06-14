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
#ifndef ___TROLL_FRAME_LISTENER___
#define ___TROLL_FRAME_LISTENER___

#include "Elypse/TrollEditorElypsePrerequisites.h"

#include <ElypseFrameListener.h>

BEGIN_TROLL_ELYPSE_NAMESPACE
{
	class TrollFrameListener
		: public ElypseFrameListener
	{
	public:
		TrollFrameListener( TrollInstance * p_oa, RenderWindow * m_window, const String & p_baseURL, const String & p_installPath, const String & p_appStringIndex );
		TrollFrameListener( TrollInstance * p_oa, TrollFrameListener * p_ofl, RenderWindow * p_window, const String & p_instanceNum );
		virtual ~TrollFrameListener();

		virtual void Initialise();
		bool frameStarted( Real p_timeSinceLastFrame );

	protected:
		virtual void _setupInterfaces();
		virtual void _setupScript();

	protected:
		TrollInstance * m_ogre;
	};
}
END_TROLL_ELYPSE_NAMESPACE

#endif

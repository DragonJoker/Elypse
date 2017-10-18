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
#ifndef ___EMOVERLAYGROUP_H___
#define ___EMOVERLAYGROUP_H___

#include "Module_Gui.h"

namespace Elypse
{
	namespace Gui
	{
		class d_dll_export EMOverlayGroup
			: public General::Theory::named
			, public General::Theory::owned_by< EMGui >
			, d_noncopyable
		{
			friend class EMOverlay;

		protected:
			Ogre::Overlay * m_overlay;
			EMOverlay * m_first;
			EMOverlayMap m_elements;

			bool m_visible;

		public:
			EMOverlayGroup( String const & p_name, EMGui & p_owner );
			~EMOverlayGroup();

		public:
			EMOverlay * CreateElement( String const & p_name );
			void ShowImmediate( bool p_visible );
			bool Contains( Real p_x, Real p_y )const;
			void DestroyElement( String const & p_name );

		protected:
			void _finaliseAddElement( EMOverlay * p_element );
			void _removeElement( String const & p_name );

		public:
			inline void					SetVisible( bool p_visible )
			{
				m_visible = p_visible;
			}
			inline bool					IsVisible()const
			{
				return m_visible;
			}
			inline Ogre::Overlay 	*	GetOverlay()const
			{
				return m_overlay;
			}
			inline EMOverlay 	*		GetFirstElement()const
			{
				return m_first;
			}
			inline const EMOverlayMap &	GetElementMap()const
			{
				return m_elements;
			}

			inline EMOverlay * GetElement( String const & p_name )const
			{
				return General::Utils::map::findOrNull( m_elements, p_name );
			}
		};
	}
}

#endif

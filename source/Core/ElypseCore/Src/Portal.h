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
#ifndef ___PORTAL_H___
#define ___PORTAL_H___

#include "Module_Zone.h"
#include "Module_Physics.h"

namespace Elypse
{
	namespace Scene
	{
		class d_dll_export Portal : public named, noncopyable
		{
		private:
			Zone * m_target;
			bool m_active;

			PhysicsObject * m_trigger;

			String m_targetName;

		public:
			Portal( String const & p_name );
			~Portal();

		public:
			void SetSpace( Space * p_space );

		public:
			inline Zone		*	GetTargetZone()const
			{
				return m_target;
			}
			inline String const &	GetTargetName()const
			{
				return m_targetName;
			}
			inline bool				IsActive()const
			{
				return m_active;
			}
			inline PhysicsObject *	GetTrigger()const
			{
				return m_trigger;
			}

			inline void				SetActive( bool p_active )
			{
				m_active = p_active;
			}
			inline void 			SetTargetZone( Zone * p_targetZone )
			{
				m_target = p_targetZone;
			}
			inline void 			SetTargetName( String const & p_name )
			{
				m_targetName = p_name;
			}
			inline void				SetTrigger( PhysicsObject * p_object )
			{
				m_trigger = p_object;
			}
		};
	}
}

#endif

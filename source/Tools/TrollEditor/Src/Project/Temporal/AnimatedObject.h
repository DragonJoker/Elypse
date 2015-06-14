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
#ifndef ___TrollAnimatedObject___
#define ___TrollAnimatedObject___

#include "TemporalObject.h"

BEGIN_TROLL_PROJECT_TEMPORAL_NAMESPACE
{
	class TrollAnimatedObject
		: public TemporalObject
	{
	public:
		TrollAnimatedObject( Object * p_object, TrollAnimationGroup * p_group, const wxString & p_fileName );
		~TrollAnimatedObject();

		virtual int BuildPanel( wxWindow * p_parent, int p_width );

		void AddAnimation( TrollAnimation * p_anim );
		void Write( wxTextOutputStream * p_stream );

		inline Object * GetObject()const
		{
			return m_object;
		}
		inline TrollAnimationGroup * GetGroup()const
		{
			return m_group;
		}

	private:
		TrollAnimationGroup * m_group;
		Object * m_object;
		TrollAnimationStrMap m_animations;
	};
}
END_TROLL_PROJECT_TEMPORAL_NAMESPACE

#endif


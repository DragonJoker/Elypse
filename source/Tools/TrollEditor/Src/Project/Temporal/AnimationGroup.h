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
#ifndef ___TrollAnimationGroup___
#define ___TrollAnimationGroup___

#include "TemporalObject.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Temporal
		{
			class TrollAnimationGroup
				: public TemporalObject
			{
			public:
				TrollAnimationGroup( wxString const & p_name, wxString const & p_fileName );
				~TrollAnimationGroup();

				TrollAnimatedObject * CreateAnimatedObject( Object * p_object, wxString const & p_fileName );

				void AddChainedAnimations( wxString const & p_anim1, wxString const & p_anim2 );
				void Write( wxTextOutputStream & p_stream );

			private:
				GUI::Properties::ObjectProperty * DoCreateProperties()override;

			private:
				TrollAnimatedObjectStrMap m_objects;
				StrStrMap m_chainedAnimations;
			};
		}
	}
}

#endif

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
#ifndef ___SoundObjectProperties___
#define ___SoundObjectProperties___

#include "GUI/Properties/Common/ObjectProperty.h"

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			class SoundObjectProperties
				: public ObjectProperty
			{
			public:
				SoundObjectProperties( ProjectComponents::Media::TrollSoundObject & p_soundObject );
				~SoundObjectProperties();

			private:
				/**
				 *\copydoc		Troll::GUI::Properties::ObjectProperty::DoCreateProperties
				 */
				virtual void DoCreateProperties( wxPGEditor * p_editor );

			private:
				ProjectComponents::Media::TrollSoundObject & m_soundObject;
			};
		}
	}
}

#endif

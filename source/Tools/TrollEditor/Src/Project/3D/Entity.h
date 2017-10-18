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
#ifndef ___TEENTITY_H___
#define ___TEENTITY_H___

#include "3DObject.h"

#include "Project/Media/Module_Media.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Objects3D
		{
			class TrollEntity
				: public Troll3DObject
			{
			public:
				TrollEntity( wxString const & p_name, wxString const & p_fileName );
				virtual ~TrollEntity();

				virtual void AttachTo( TrollSceneNode * p_node );

				void SetOgreEntity( Ogre::Entity * p_entity );
				void SetMaterial( wxString const & p_name, bool p_updateOgre = true );
				void SetMesh( TrollMesh * p_mesh );

				void Write( wxTextOutputStream & p_stream );

				inline TrollMesh * GetMesh()const
				{
					return m_mesh;
				}
				inline wxString GetMaterial()const
				{
					return m_material;
				}
				inline Ogre::Entity * GetOgreEntity()const
				{
					return m_ogreEntity;
				}
				inline Media::TrollVideoObject * GetVideo()const
				{
					return m_video;
				}
				inline void SetVideo( Media::TrollVideoObject * p_video )
				{
					m_video = p_video;
				}

			private:
				GUI::Properties::ObjectProperty * DoCreateProperties()override;

			protected:
				TrollMesh * m_mesh;
				wxString m_material;
				Ogre::Entity * m_ogreEntity;
				Media::TrollVideoObject * m_video;
			};
		}
	}
}

#endif

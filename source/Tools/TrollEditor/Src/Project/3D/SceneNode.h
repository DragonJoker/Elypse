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
#ifndef ___TESCENENODE_H___
#define ___TESCENENODE_H___

#include "3DObject.h"

#include "Project/Media/Module_Media.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Objects3D
		{
			class TrollSceneNode
				: public Troll3DObject
			{
			public:
				TrollSceneNode( wxString const & p_name, wxString const & p_fileName, const Vector3 & p_position = Vector3::ZERO, const Quaternion & p_orientation = Quaternion::IDENTITY, const Vector3 & p_scale = Vector3( 1, 1, 1 ) );
				virtual ~TrollSceneNode();

				virtual void AttachTo( TrollSceneNode * p_node );

				bool AddObject( Troll3DObject * p_object );
				bool AddChild( TrollSceneNode * p_node );

				void SetOgreNode( Ogre::SceneNode * p_node );
				void SetPosition( const Vector3 & p_position, bool p_updateOgre = true );
				void SetOrientation( const Quaternion & p_orientation, bool p_updateOgre = true );
				void SetScale( const Vector3 & p_scale, bool p_updateOgre = true );

				void RemoveObject( wxString const & p_name );
				void RemoveChild( wxString const & p_name );

				void Write( wxTextOutputStream & p_stream );

				inline Ogre::SceneNode * GetOgreNode()const
				{
					return m_ogreNode;
				}
				inline Ogre::Vector3 GetPosition()const
				{
					return m_position;
				}
				inline Ogre::Quaternion GetOrientation()const
				{
					return m_orientation;
				}
				inline Ogre::Vector3 GetScale()const
				{
					return m_scale;
				}
				inline Media::TrollSoundObject * GetSound()const
				{
					return m_sound;
				}
				inline void SetSound( Media::TrollSoundObject * p_sound )
				{
					m_sound = p_sound;
				}

			private:
				GUI::Properties::ObjectProperty * DoCreateProperties()override;

			private:
				Ogre::Quaternion m_orientation;
				Ogre::Vector3 m_scale;
				Ogre::Vector3 m_position;
				Troll3DObjectMap m_objects;
				TrollSceneNodeMap m_childs;
				Ogre::SceneNode * m_ogreNode;
				Media::TrollSoundObject * m_sound;
				bool m_inheritOrientation;
				bool m_inheritScale;
			};
		}
	}
}

#endif

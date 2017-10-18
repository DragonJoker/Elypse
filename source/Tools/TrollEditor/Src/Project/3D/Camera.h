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
#ifndef ___TECAMERA_H___
#define ___TECAMERA_H___

#include "3DObject.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Objects3D
		{
			class TrollCamera
				: public Troll3DObject
			{
			public:
				TrollCamera( wxString const & p_name, wxString const & p_fileName );
				virtual ~TrollCamera();

				virtual void AttachTo( TrollSceneNode * p_node );

				void SetOgreCamera( Ogre::Camera * p_camera );
				void SetPosition( const Ogre::Vector3 & p_pos, bool p_updateOgre = true );
				void SetDirection( const Ogre::Vector3 & p_dir, bool p_updateOgre = true );
				void SetOrientation( const Ogre::Quaternion & p_orientation, bool p_updateOgre = true );
				void SetPolygonMode( TrollCameraPolygonMode p_cpm, bool p_updateOgre = true );
				void SetYFOV( float p_fov, bool p_updateOgre = true );

				void Write( wxTextOutputStream & p_stream );

				inline Ogre::Camera * GetOgreCamera()const
				{
					return m_ogreCamera;
				}
				inline Ogre::Vector3 GetPosition()const
				{
					return m_position;
				}
				inline Ogre::Vector3 GetDirection()const
				{
					return m_direction;
				}
				inline Ogre::Quaternion GetOrientation()const
				{
					return m_orientation;
				}
				inline TrollCameraPolygonMode GetPolygonMode()const
				{
					return m_polygonMode;
				}
				inline float GetYFOV()const
				{
					return m_yFOV;
				}

			private:
				GUI::Properties::ObjectProperty * DoCreateProperties()override;

			private:
				Ogre::Vector3 m_position;
				Ogre::Vector3 m_direction;
				Ogre::Quaternion m_orientation;
				TrollCameraPolygonMode m_polygonMode;
				float m_yFOV;
				Ogre::Camera * m_ogreCamera;
			};
		}
	}
}

#endif


#ifndef ___TESCENENODE_H___
#define ___TESCENENODE_H___

#include "../Media/Module_Media.h"
#include "Module_3D.h"
#include "3DObject.h"

namespace Troll
{
	namespace Objects3D
	{
		class TrollSceneNode : public Troll3DObject
		{
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

		public:
			TrollSceneNode( const wxString & p_name, const wxString & p_fileName, const Vector3 & p_position = Vector3::ZERO,
							const Quaternion & p_orientation = Quaternion::IDENTITY, const Vector3 & p_scale = Vector3( 1, 1, 1 ) );
			virtual ~TrollSceneNode();

			virtual void AttachTo( TrollSceneNode * p_node );
			virtual int BuildPanel( wxWindow * p_parent, int p_width );

			bool AddObject( Troll3DObject * p_object );
			bool AddChild( TrollSceneNode * p_node );

			void SetOgreNode( Ogre::SceneNode * p_node );
			void SetPosition( const Vector3 & p_position, bool p_updateOgre = true );
			void SetOrientation( const Quaternion & p_orientation, bool p_updateOgre = true );
			void SetScale( const Vector3 & p_scale, bool p_updateOgre = true );

			void RemoveObject( const wxString & p_name );
			void RemoveChild( const wxString & p_name );

			void Write( wxTextOutputStream * p_stream );

		public:

			inline Ogre::SceneNode 	*	GetOgreNode()const
			{
				return m_ogreNode;
			}
			inline Ogre::Vector3			GetPosition()const
			{
				return m_position;
			}
			inline Ogre::Quaternion			GetOrientation()const
			{
				return m_orientation;
			}
			inline Ogre::Vector3			GetScale()const
			{
				return m_scale;
			}
			inline Media::TrollSoundObject *	GetSound()const
			{
				return m_sound;
			}

			inline void SetSound( Media::TrollSoundObject * p_sound )
			{
				m_sound = p_sound;
			}
		};
	}
}

#endif



#ifndef ___TEENTITY_H___
#define ___TEENTITY_H___

#include "Module_3D.h"
#include "../Media/Module_Media.h"
#include "3DObject.h"

namespace Troll
{
	namespace Objects3D
	{
		class TrollEntity : public Troll3DObject
		{
		protected:
			TrollMesh * m_mesh;
			wxString m_material;
			Ogre::Entity * m_ogreEntity;
			Media::TrollVideoObject * m_video;

		public:
			TrollEntity( const wxString & p_name, const wxString & p_fileName );
			virtual ~TrollEntity();

			virtual void AttachTo( TrollSceneNode * p_node );
			virtual int BuildPanel( wxWindow * p_parent, int p_width );

			void SetOgreEntity( Ogre::Entity * p_entity );
			void SetMaterial( const wxString & p_name, bool p_updateOgre = true );
			void SetMesh( TrollMesh * p_mesh );

			void Write( wxTextOutputStream * p_stream );

		public:
			inline TrollMesh 		*			GetMesh()const
			{
				return m_mesh;
			}
			inline wxString					GetMaterial()const
			{
				return m_material;
			}
			inline Ogre::Entity 	*		GetOgreEntity()const
			{
				return m_ogreEntity;
			}
			inline Media::TrollVideoObject *	GetVideo()const
			{
				return m_video;
			}

			inline void SetVideo( Media::TrollVideoObject * p_video )
			{
				m_video = p_video;
			}
		};
	}
}

#endif


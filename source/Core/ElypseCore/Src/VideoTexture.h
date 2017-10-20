/*
See LICENSE file in root folder
*/
#ifndef ___VIDEOTEXTURE_H___
#define ___VIDEOTEXTURE_H___

#include <OgreExternalTextureSource.h>
#include <OgreFrameListener.h>

#include <Mutex.h>

#include "Module_Media.h"

namespace Elypse
{
	namespace Media
	{
		class d_dll_export VideoTexture
			: public Ogre::ExternalTextureSource
			, owned_by< VideoObject >
		{
		public:
			VideoTexture( VideoObject & p_owner );
			~VideoTexture();

		public:
			virtual bool initialise();
			virtual void shutDown()	{}
			virtual void createDefinedTexture( String const & p_materialName, String const & p_groupName = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
			virtual void destroyAdvancedTexture( String const & p_materialName, String const & p_groupName = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME )
			{
			}

		public:
			void CopyToBuffer( unsigned char * p_buffer, uint32_t p_bufferSize );
			void Update();
			void RedrawInBlack();

		private:
			void _createTexture( String const & p_materialName, String const & p_groupName, bool p_createMat );

		private:
			bool m_bufferChange;
			uint32_t m_bufferSize;

			unsigned char * m_tmpBuffer;
			Texture * m_texture;

			std::mutex m_mutex;
		};
	}
}
#endif

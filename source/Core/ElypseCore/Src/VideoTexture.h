/*********************************************************************************************************************

	Author :	Sylvain DOREMUS

	Company :	ForDev Studio - Copyright 2006

	Files :		VideoTexture.h - VideoTexture.cpp

	Desc :		Class used to put a video in a texture.

*********************************************************************************************************************/
#ifndef ___VIDEOTEXTURE_H___
#define ___VIDEOTEXTURE_H___

#include <OgreExternalTextureSource.h>
#include <OgreFrameListener.h>

#include <Mutex.h>

#include "Module_Media.h"

namespace EMuse
{
	namespace Media
	{
		class d_dll_export VideoTexture : public Ogre::ExternalTextureSource, owned_by <VideoObject>
		{
		private:
			bool m_bufferChange;
			unsigned int m_bufferSize;

			unsigned char * m_tmpBuffer;
			Texture * m_texture;

			Mutex m_mutex;

		public:
			VideoTexture( VideoObject * p_owner );
			~VideoTexture();

		public:
			virtual bool initialise();
			virtual void shutDown()	{}
			virtual void createDefinedTexture(
				const String & p_materialName,
				const String & p_groupName = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
			virtual void destroyAdvancedTexture(
				const String & p_materialName,
				const String & p_groupName = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME )	{}
		public:
			void CopyToBuffer( unsigned char * p_buffer, unsigned int p_bufferSize );
			void Update();
			void RedrawInBlack();

		private:
			void _createTexture( const String & p_materialName, const String & p_groupName, bool p_createMat );
		};
	}
}
#endif

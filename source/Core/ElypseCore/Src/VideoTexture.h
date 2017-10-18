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

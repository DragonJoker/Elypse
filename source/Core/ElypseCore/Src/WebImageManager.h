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
#ifndef ___WEB_IMAGE_MANAGER_H___
#define ___WEB_IMAGE_MANAGER_H___

#include "Module_SpecialEffects.h"

#include <Singleton.h>
#include <Mutex.h>
#include <ThreadPool.h>

namespace Elypse
{
	namespace SpecialEffects
	{
		class WebImageManager : public General::Theory::Singleton<WebImageManager>
		{
		private:
			CURLManager * m_curl;

			WebImageMap m_images;

			std::mutex m_mutex;

			ThreadPool m_threadPool;

		public:
			WebImageManager();
			~WebImageManager();

		public:
			WebImage * Create( String const & p_name );
			bool DestroyImage( String const & p_name );
			void Lock();
			void Unlock();

			void ThreadedLoad( WebImage * p_image );

		public:
			inline WebImage * GetWebImage( String const & p_name )const
			{
				return General::Utils::map::findOrNull( m_images, p_name );
			}
			inline CURLManager * GetCurl()const
			{
				return m_curl;
			}
		};
	}
}

#endif

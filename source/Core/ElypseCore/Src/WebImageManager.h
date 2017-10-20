/*
See LICENSE file in root folder
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

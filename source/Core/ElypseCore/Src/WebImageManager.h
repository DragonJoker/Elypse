#ifndef ___WEB_IMAGE_MANAGER_H___
#define ___WEB_IMAGE_MANAGER_H___

#include "Module_SpecialEffects.h"

#include <Singleton.h>
#include <Mutex.h>
#include <ThreadPool.h>

namespace EMuse
{
	namespace SpecialEffects
	{
		class WebImageManager : public General::Theory::Singleton<WebImageManager>
		{
		private:
			CURLManager * m_curl;

			WebImageMap m_images;

			Mutex m_mutex;

			ThreadPool m_threadPool;

		public:
			WebImageManager();
			~WebImageManager();

		public:
			WebImage * Create( const String & p_name );
			bool DestroyImage( const String & p_name );
			void Lock();
			void Unlock();

			void ThreadedLoad( WebImage * p_image );

		public:
			inline WebImage * GetWebImage( const String & p_name )const
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

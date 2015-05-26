#ifndef ___WEB_IMAGE_H___
#define ___WEB_IMAGE_H___

#include "Module_SpecialEffects.h"

#include <Url.h>
#include <Mutex.h>

namespace EMuse
{
	namespace SpecialEffects
	{
		class d_dll_export WebImage : public named
		{
		private:
			Material * m_material;
			Texture * m_texture;
			Url m_url;

			Mutex m_mutex;

			MaterialTargetArray m_targets;

			bool m_ready;
			bool m_loading;

		public:
			WebImage( const String & p_name );
			~WebImage();

		private:
			void _delete();
			void _setupImage();
			void _loadFromMemory( const String & p_ext, const String & p_buffer, Image * p_image );

		public:
			void Reload();
			void AddTarget( MaterialTarget * p_target );
			void Load();

		public:
			inline bool IsReady()const
			{
				return m_ready;
			}
			inline void SetUrl( const Url & p_url )
			{
				m_url = p_url;
			}

		};
	}
}

#endif

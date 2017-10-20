/*
See LICENSE file in root folder
*/
#ifndef ___WEB_IMAGE_H___
#define ___WEB_IMAGE_H___

#include "Module_SpecialEffects.h"

#include <Url.h>
#include <Mutex.h>

namespace Elypse
{
	namespace SpecialEffects
	{
		class d_dll_export WebImage : public named
		{
		private:
			Material * m_material;
			Texture * m_texture;
			Url m_url;

			std::mutex m_mutex;

			MaterialTargetArray m_targets;

			bool m_ready;
			bool m_loading;

		public:
			WebImage( String const & p_name );
			~WebImage();

		private:
			void _delete();
			void _setupImage();
			void _loadFromMemory( String const & p_ext, String const & p_buffer, Image * p_image );

		public:
			void Reload();
			void AddTarget( MaterialTarget * p_target );
			void Load();

		public:
			inline bool IsReady()const
			{
				return m_ready;
			}
			inline void SetUrl( Url const & p_url )
			{
				m_url = p_url;
			}

		};
	}
}

#endif

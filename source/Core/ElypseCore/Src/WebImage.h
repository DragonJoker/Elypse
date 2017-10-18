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

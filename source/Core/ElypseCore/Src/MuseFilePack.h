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
#ifndef ___MUSEFILE_H___
#define ___MUSEFILE_H___

#include "Module_Download.h"

#include <PreciseTimer.h>
#include <OwnedBy.h>

namespace Elypse
{
	namespace Download
	{
		class d_dll_export MuseFilePack
			: public owned_by< MusePack >
			, d_noncopyable
		{
		public:
			MuseFilePack( MusePack & p_pack )
				: owned_by< MusePack >( p_pack )
			{}

			~MuseFilePack()
			{
				if ( m_fileDescriptor )
				{
					fclose( m_fileDescriptor );
					m_fileDescriptor = nullptr;
				}
			}

			bool ReadHeader( size_t & p_indexBuffer, const DataArray & p_waitingDatas );
			bool ReadContents( size_t & p_indexBuffer, const DataArray & p_waitingDatas );
			void SetFinished();

			inline bool IsFinished()const
			{
				return m_finished /*&& IsDownloadFinished()*/;
			}
			inline bool IsDownloadFinished()const
			{
				return m_fileSize == m_downloadedBytes;
			}

			inline void SetFilename( char const * const p_buffer )
			{
				m_filename.assign( p_buffer, m_filenameSize );
			}
			inline void SetFilenameSize( size_t p_size )
			{
				m_filenameSize = p_size;
			}
			inline bool IsHeaderCompleted()const
			{
				return m_status == HEADERCOMPLETE;
			}
			inline String const & GetFilename()const
			{
				return m_filename;
			}
			inline String const & GetMD5()const
			{
				return m_md5;
			}
			inline size_t GetFileSize()const
			{
				return m_fileSize;
			}
			inline size_t GetFilenameSize()const
			{
				return m_filenameSize;
			}
			inline DataBlockType GetType()const
			{
				return m_type;
			}
			inline HeaderStatus GetStatus()const
			{
				return m_status;
			}

		protected:
			HeaderStatus m_status{ HEADEREMPTY };
			DataBlockType m_type{ EM_BLOCK_UNKNOWN };
			String m_filename;
			String m_md5;
			size_t m_filenameSize{ 0 };
			FILE * m_fileDescriptor{ nullptr };
			size_t m_fileSize{ 0 };
			size_t m_downloadedBytes{ 0 };
			bool m_finished{ false };
		};

		struct EM_SndHeader
		{
			uint32_t fileSize{ 0 };
			uint16_t fileNameSize{ 0 };
			std::vector< char > fileName;
		};
	}
}

#endif

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
#ifndef ___EMUSE_PLUGIN_H___
#define ___EMUSE_PLUGIN_H___

#include "Module_Main.h"

#include "Module_Script.h"

#include <Mutex.h>

namespace Elypse
{
	namespace Main
	{
		enum PluginGraphicalStatus
		{
			StatusErrorRenderer,
			StatusErrorOgre,
			StatusErrorDirectories,
			StatusErrorUnknown,
			StatusErrorUpdating,
			StatusLoading,
			StatusNone
		};

		enum CursorType
		{
			TextCursor,
			HandCursor,
			SizeNSCursor,
			SizeWECursor,
			ArrowCursor
		};

		class d_dll_export ElypsePlugin d_debug_check_noncopyable
		{
		public:
			ElypseInstance * m_instance;
			PluginGraphicalStatus m_graphicalStatus;
			unsigned int m_width;
			unsigned int m_height;
			CursorType m_baseCursor;

			CURLManager * m_curl;

			Mutex m_mutex;

		public:
			ElypsePlugin();
			virtual ~ElypsePlugin();

		public:
			void ThreadedStringFromUrl( const String & p_url, const String & p_postParams, ScriptNode * p_caller, ScriptNode * p_execAtAnd );
			String GetStringFromUrl( const String & p_url, const String & p_postParams );
			void SetSessionCookie( const String & p_cookieParams );

		public:
			virtual bool OpenURL( const String & p_url, bool p_newWindow, bool p_usePostMethod ) = 0;
			virtual void MajorError( const String & p_errorText, const String & p_title ) = 0;
			virtual void Quit() = 0;
			virtual void PostChangeGraphicalStatus() = 0;
			virtual bool ExecuteJavascript( const String & p_functionName ) = 0;
			virtual void ChangeCursorTo( CursorType p_cursorType ) = 0;

		public:
			inline void SetInstance( ElypseInstance * p_instance )
			{
				m_instance = p_instance;
			}

			inline ElypseInstance * GetInstance()const
			{
				return m_instance;
			}
			inline void SetGraphicalStatus( PluginGraphicalStatus p_status )
			{
				m_graphicalStatus = p_status;
				PostChangeGraphicalStatus();
			}
			inline void SetSize( unsigned int p_width , unsigned int p_height )
			{
				m_width = p_width ;
				m_height = p_height;
			}
			virtual inline void LockGui() 	{}
			virtual inline void UnlockGui() {}
			inline CursorType GetBaseCursor()
			{
				return m_baseCursor;
			}
			inline void SetBaseCursor( CursorType p_cursor )
			{
				m_baseCursor = p_cursor;
			}
		};
	}
}

#endif

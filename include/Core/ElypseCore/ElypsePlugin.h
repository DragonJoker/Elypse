/*
See LICENSE file in root folder
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
			uint32_t m_width;
			uint32_t m_height;
			CursorType m_baseCursor;

			CURLManager * m_curl;

			std::mutex m_mutex;

		public:
			ElypsePlugin();
			virtual ~ElypsePlugin();

		public:
			void ThreadedStringFromUrl( String const & p_url, String const & p_postParams, ScriptNode * p_caller, ScriptNode * p_execAtAnd );
			String GetStringFromUrl( String const & p_url, String const & p_postParams );
			void SetSessionCookie( String const & p_cookieParams );

		public:
			virtual bool OpenURL( String const & p_url, bool p_newWindow, bool p_usePostMethod ) = 0;
			virtual void MajorError( String const & p_errorText, String const & p_title ) = 0;
			virtual void Quit() = 0;
			virtual void PostChangeGraphicalStatus() = 0;
			virtual bool ExecuteJavascript( String const & p_functionName ) = 0;
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
			inline void SetSize( uint32_t p_width , uint32_t p_height )
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

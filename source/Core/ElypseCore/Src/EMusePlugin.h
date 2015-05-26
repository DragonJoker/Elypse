#ifndef ___EMUSE_PLUGIN_H___
#define ___EMUSE_PLUGIN_H___

#include "Module_Main.h"

#include "Module_Script.h"

#include <Mutex.h>

namespace EMuse
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

		class d_dll_export EMusePlugin d_debug_check_noncopyable
		{
		public:
			EMuseInstance * m_instance;
			PluginGraphicalStatus m_graphicalStatus;
			unsigned int m_width;
			unsigned int m_height;
			CursorType m_baseCursor;

			CURLManager * m_curl;

			Mutex m_mutex;

		public:
			EMusePlugin();
			virtual ~EMusePlugin();

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
			inline void SetInstance( EMuseInstance * p_instance )
			{
				m_instance = p_instance;
			}

			inline EMuseInstance * GetInstance()const
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

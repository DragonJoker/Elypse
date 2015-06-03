#ifndef ___EMUSE_PLUGIN_BIN_H___
#define ___EMUSE_PLUGIN_BIN_H___


#include "Module_Binary.h"
#include <Module_Main.h>

#include <EMusePlugin.h>

namespace EMuse
{
	namespace Binary
	{
		class EMusePlugin_Binary : public EMusePlugin
		{
		private:
			wxWindow * m_handle;

		public:
			EMusePlugin_Binary();
			virtual ~EMusePlugin_Binary();

		public:
			virtual bool OpenURL( const String & p_url, bool p_newWindow, bool p_usePostMethod )
			{
				return true;
			}
			virtual void MajorError( const String & p_errorText, const String & p_title );
			virtual void Quit();
			virtual void PostChangeGraphicalStatus() {}
			virtual void LockGui();
			virtual void UnlockGui();
			virtual void ChangeCursorTo( CursorType p_cursorType );

			virtual bool ExecuteJavascript( const String & p_functionName )
			{
				return true;
			};
		public:
			inline void SetHandle( wxWindow * p_handle )
			{
				m_handle = p_handle;
			}
		};
	}
}

#endif

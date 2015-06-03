#ifndef ___EMUSE_PLUGIN_WX_H___
#define ___EMUSE_PLUGIN_WX_H___

#include <EMusePlugin.h>
//#include <gdiplus.h>
#include <wx/defs.h>
#include <wx/dynlib.h>

namespace EMuse
{
	namespace TEWidget
	{
//	typedef std::vector<Gdiplus::Image*> ImageArray;

		class EMusePlugin_wxWidget : public EMusePlugin
		{
		private:
			wxWindow * m_handle;
			String m_programFiles;
			String m_baseUrl;
			bool m_protectedMode;
			wxDynamicLibrary * m_pLibrary;
//		HMODULE m_module;

//		Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
			unsigned long m_gdiplusToken;
		public:
			EMusePlugin_wxWidget();
			virtual ~EMusePlugin_wxWidget();

		public:
			virtual String GetStringFromUrl( const String & p_url , bool p_usePostMethod );
			virtual bool OpenURL( const String & p_url , bool p_newWindow , bool p_usePostMethod );
			virtual void MajorError( const String & p_errorText , const String & p_title );
			virtual void Quit();
			virtual void PostChangeGraphicalStatus();
			virtual bool ExecuteJavascript( const String & p_functionName )
			{
				return true;
			};
			virtual void LockGui();
			virtual void UnlockGui();
			virtual void ChangeCursorTo( CursorType p_cursorType );

		public:
			void InitGDI();
			void CloseGDI();
			void Paint();

		public:
			inline void SetHandle( wxWindow * p_handle )
			{
				m_handle = p_handle;
			}
			inline void SetProgramFilesDir( const String & p_fullPath )
			{
				m_programFiles = p_fullPath;
			}
			inline void UseProtectedMode()
			{
				m_protectedMode = true;
			}
			inline void SetBaseUrl( const String & p_baseUrl )
			{
				m_baseUrl = p_baseUrl;
			}
			inline void SetDllInstance( wxDynamicLibrary * p_pLibrary )
			{
				m_pLibrary = p_pLibrary;
			}
		};
	}
}

#endif

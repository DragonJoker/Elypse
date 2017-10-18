#ifndef ___EMUSE_PLUGIN_OCX_H___
#define ___EMUSE_PLUGIN_OCX_H___

#include <windows.h>
#include <gdiplus.h>
#include <afxinet.h>
#include <Mshtml.h>

#include <ElypsePlugin.h>

#include <Path.h>
#include <ThreadedQueue.h>

interface IWebBrowser;

namespace Elypse
{
	namespace OCX
	{
		typedef std::vector< Gdiplus::Image * > ImageArray;

		class ElypseOcxPlugin
			: public Elypse::Main::ElypsePlugin
		{
		private:
			HWND m_handle;
			String m_programFiles;
			String m_baseUrl;
			bool m_protectedMode;
			IOleClientSite * m_clientSite;
			CComPtr<IWebBrowser> m_spDoc;

			Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
			ULONG_PTR m_gdiplusToken;

			Path m_installPath;
			ThreadedQueue<String> m_jsScriptExcutionQueue;

		public:
			ElypseOcxPlugin();
			virtual ~ElypseOcxPlugin();

		public:
			virtual bool OpenURL( const String & p_url , bool p_newWindow , bool p_usePostMethod );
			virtual void MajorError( const String & p_errorText , const String & p_title );
			virtual void Quit( );
			virtual void PostChangeGraphicalStatus();
			virtual bool ExecuteJavascript( const String & p_functionName );
			virtual void ChangeCursorTo( CursorType p_cursorType );

		public:
			void InitGDI();
			void CloseGDI();
			void Paint();
			void ExecJS();

			//void CallJavaScript();
			//bool CallJScript(const CString & strFunc, const CStringArray& paramArray,CComVariant* pVarResult);
			//void BSTRtoASC (BSTR str, char * &strRet);

			void SetClientSite( IOleClientSite * p_clientSite );

		private:
			void _convertNodeToVariant( ScriptNode * p_node, VARIANT * p_variant );
			void _printImage( const String & p_imagePath );
			void _printText( const String & p_text );

		public:
			inline void SetHandle( HWND p_handle )
			{
				m_handle = p_handle;
			}
			inline void SetProgramFilesDir( const String & p_fullPath )
			{
				m_programFiles = p_fullPath;
			}
			inline void UseProtectedMode( )
			{
				m_protectedMode = true;
			}
			inline void SetBaseUrl( const String & p_baseUrl )
			{
				m_baseUrl = p_baseUrl;
			}
			inline void SetInstallPath( const Path & p_installPath )
			{
				m_installPath = p_installPath;
			}
		};
	}
}

#endif

#ifndef ___ELYPSE_PLUGIN_FIREFOX_H___
#define ___ELYPSE_PLUGIN_FIREFOX_H___

#define ____NO_ODE_INT_TYPES____

#define ____ELYPSE_NO_MEMORY_DEBUG____
#include "NpApi/npapi.h"
#include <ElypsePlugin.h>
#include <OgrePrerequisites.h>

namespace Elypse
{
	namespace Firefox
	{
		class ElypsePlugin_Firefox
			: public ElypsePlugin
		{
#if ELYPSE_WINDOWS
		private:
			ULONG_PTR m_gdiplusToken;
			HWND m_handle;
		public:
			inline void SetHandle( HWND p_handle )
			{
				m_handle = p_handle;
			}
#endif
		private:
			NPP m_instance;
			String m_baseUrl;
			std::wstring m_imagePath;
			Path m_installPath;
		public:
			ElypsePlugin_Firefox();
			~ElypsePlugin_Firefox();

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
			void _printImage( const String & p_imagePath );
			void SetImagePath( const String & p_imagePath );
			void _printText( const String & p_text );

		public:
			inline void SetNPPInstance( NPP p_instance )
			{
				m_instance = p_instance ;
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

using namespace Elypse::Firefox;

#endif

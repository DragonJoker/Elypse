#ifndef __EMUSE_LOADING_BAR_H___
#define __EMUSE_LOADING_BAR_H___

#include "Module_Main.h"
#include <OgreResourceGroupManager.h>

namespace EMuse
{
	namespace Main
	{
		class d_dll_export EMuseLoadingBar : public ResourceGroupListener, d_noncopyable
		{
		protected:
			RenderWindow * m_window;

			Overlay * m_loadOverlay;
			OverlayElement * m_loadingBarElement;
			OverlayElement * m_loadingDescriptionElement;
			OverlayElement * m_loadingPercentElement;

			Real m_initProportion;

			unsigned short m_numGroupsInit;
			unsigned short m_numGroupsLoad;

			Real m_progressBarMaxSize;
			Real m_progressBarScriptSize;
			Real m_progressBarInc;
			String m_dlres;

			//Customizable values :
			String m_loadingOverlayName;
			String m_loadingBarProgressName;
			String m_loadingBarPercentageName;
			String m_loadingBarName;
			int m_loadingBarOffset;

			bool m_started;
			bool m_startByDownload;

			Real m_previousWidth;

		public:
			EMuseLoadingBar();
			~EMuseLoadingBar();

		private:
			void _updateWindow();

		public:
			void finish();
			void StartDownloadOfFile();
			void EndDownloadOfFile();
			void start( unsigned short numGroupsInit = 1, unsigned short numGroupsLoad = 1, Real initProportion = 0.70f );
			void resourceGroupScriptingStarted( const String & groupName, size_t scriptCount );
			void resourceGroupLoadStarted( const String & groupName, size_t resourceCount );
			void resourceLoadEnded();
			void resourceDownloadStarted( const String & name );
			void setPercent( Real p_percent, Real p_downSpeed );
			void ResetBar();

			void UpdateBar();

			void scriptParseEnded( const String & );

			//Unused
			void scriptParseStarted( const String &, bool & )
			{
				;
			}
			void scriptParseEnded( const String &, bool )
			{
				;
			}
			void resourceGroupScriptingEnded( const String & )
			{
				;
			}
			void resourceLoadStarted( const Ogre::ResourcePtr & )
			{
				;
			}
			void worldGeometryStageStarted( const String & )
			{
				;
			}
			void worldGeometryStageEnded()
			{
				;
			}
			void resourceGroupLoadEnded( const String & )
			{
				;
			}
			void resourceDownloadEnded( const String & )
			{
				;
			}

		public:
			inline void SetLoadingBarOverlayName( const String & p_name )
			{
				m_loadingOverlayName = p_name;
			}
			inline void SetLoadingBarProgressName( const String & p_name )
			{
				m_loadingBarProgressName = p_name;
			}
			inline void SetLoadingBarPercentName( const String & p_name )
			{
				m_loadingBarPercentageName = p_name;
			}
			inline void SetLoadingBarGroupName( const String & p_name )
			{
				m_loadingBarName = p_name;
			}
			inline void SetLoadingBarOffset( int p_offset )
			{
				m_loadingBarOffset = p_offset;
			}
			inline void SetWindow( RenderWindow * p_window )
			{
				m_window = p_window;
			}
			inline bool IsStarted()const
			{
				return m_started;
			}
		};
	}
}

#endif

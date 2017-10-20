/*
See LICENSE file in root folder
*/
#ifndef __EMUSE_LOADING_BAR_H___
#define __EMUSE_LOADING_BAR_H___

#include "Module_Main.h"
#include <OgreResourceGroupManager.h>

namespace Elypse
{
	namespace Main
	{
		class d_dll_export ElypseLoadingBar : public ResourceGroupListener, d_noncopyable
		{
		protected:
			RenderWindow * m_window;

			Overlay * m_loadOverlay;
			OverlayElement * m_loadingBarElement;
			OverlayElement * m_loadingDescriptionElement;
			OverlayElement * m_loadingPercentElement;

			Real m_initProportion;

			uint16_t m_numGroupsInit;
			uint16_t m_numGroupsLoad;

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
			ElypseLoadingBar();
			~ElypseLoadingBar();

		private:
			void _updateWindow();

		public:
			void finish();
			void StartDownloadOfFile();
			void EndDownloadOfFile();
			void start( uint16_t numGroupsInit = 1, uint16_t numGroupsLoad = 1, Real initProportion = 0.70f );
			void resourceGroupScriptingStarted( String const & groupName, size_t scriptCount );
			void resourceGroupLoadStarted( String const & groupName, size_t resourceCount );
			void resourceLoadEnded();
			void resourceDownloadStarted( String const & name );
			void setPercent( Real p_percent, Real p_downSpeed );
			void ResetBar();

			void UpdateBar();

			void scriptParseEnded( String const & );

			//Unused
			void scriptParseStarted( String const &, bool & )
			{
				;
			}
			void scriptParseEnded( String const &, bool )
			{
				;
			}
			void resourceGroupScriptingEnded( String const & )
			{
				;
			}
			void resourceLoadStarted( const Ogre::ResourcePtr & )
			{
				;
			}
			void worldGeometryStageStarted( String const & )
			{
				;
			}
			void worldGeometryStageEnded()
			{
				;
			}
			void resourceGroupLoadEnded( String const & )
			{
				;
			}
			void resourceDownloadEnded( String const & )
			{
				;
			}

		public:
			inline void SetLoadingBarOverlayName( String const & p_name )
			{
				m_loadingOverlayName = p_name;
			}
			inline void SetLoadingBarProgressName( String const & p_name )
			{
				m_loadingBarProgressName = p_name;
			}
			inline void SetLoadingBarPercentName( String const & p_name )
			{
				m_loadingBarPercentageName = p_name;
			}
			inline void SetLoadingBarGroupName( String const & p_name )
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

/*
See LICENSE file in root folder
*/
#ifndef ___VIDEOOVERLAY_H___
#define ___VIDEOOVERLAY_H___

#include "Module_Media.h"

#include "ZoneObject.h"
#include "EMOverlay.h"

namespace Elypse
{
	namespace Media
	{
		class d_dll_export VideoOverlay
			: public ZoneObject
			, public owned_by< VideoObject >
		{
		private:
			EMOverlay * m_EMOverlay;
			String m_oldMaterialName;
			bool m_delayLoad;

		public:
			VideoOverlay( VideoObject & p_owner, EMOverlay * p_EMOverlay )
				: owned_by< VideoObject >( p_owner )
				, m_EMOverlay( p_EMOverlay )
				, m_delayLoad( false )
			{
				genlib_assert( m_EMOverlay != nullptr );
			}

			~VideoOverlay()
			{
			}

			void Initialise( String const & p_videoTextureName );
			void Reset();

		public:
			inline EMOverlay *	GetEMOverlay()const
			{
				return m_EMOverlay;
			}
			inline bool			IsLoadDelayed()const
			{
				return m_delayLoad;
			}

			inline void		DelayLoad()
			{
				m_delayLoad = true;
			}

		public:
			virtual void AddToZone( Zone * p_zone );
			virtual void RemoveFromZone( Zone * p_zone );
		};
	}
}

#endif

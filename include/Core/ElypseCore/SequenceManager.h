/*
See LICENSE file in root folder
*/
#ifndef ___SEQUENCE_MANAGER_H___
#define ___SEQUENCE_MANAGER_H___

#include "Module_Sequences.h"

#include <Manager.h>

#include "Sequence.h"

namespace Elypse
{
	namespace Sequences
	{
		class d_dll_export SequenceManager
			: public Manager<Sequence>
		{
		public:
			SequenceManager();
			~SequenceManager();

		public:
			void RegisterInterpolator( String const & p_name, Vector3_Interpolator * p_interpolator );
			void RegisterInterpolator( String const & p_name, Quaternion_Interpolator * p_interpolator );
			void RegisterInterpolator( String const & p_name, Real_Interpolator * p_interpolator );

			void RegisterFactory( String const & p_name, BaseContinuousEventFactory * p_factory );
			void RegisterFactory( String const & p_name, BasePonctualEventFactory * p_factory );

			BasePonctualEvent * CreatePonctualEvent( String const & p_name, const StringArray & p_params );
			BaseContinuousEvent * CreateContinuousEvent( String const & p_name );

		public:
			void Update( Real p_time );
			StringArray GetAvailablePonctualEvents();
			StringArray GetAvailableContinuousEvents();

			Vector3_Interpolator * GetInterpolator_V3( String const & p_name );
			Quaternion_Interpolator * GetInterpolator_Q( String const & p_name );
			Real_Interpolator * GetInterpolator_R( String const & p_name );

		private:
			V3InterpolatorMap m_interpolators_V3;
			QInterpolatorMap m_interpolators_Q;
			RInterpolatorMap m_interpolators_R;

			BaseContinuousEventFactoryMap m_continuousEventFactories;
			BasePonctualEventFactoryMap m_ponctualEventFactories;
		};
	}
}

#endif

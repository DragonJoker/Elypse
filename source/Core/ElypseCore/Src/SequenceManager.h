#ifndef ___SEQUENCE_MANAGER_H___
#define ___SEQUENCE_MANAGER_H___

#include "Module_Sequences.h"

#include <Manager.h>

#include "Sequence.h"

namespace EMuse
{
	namespace Sequences
	{
		class d_dll_export SequenceManager : public Manager<Sequence>
		{
		private:
			V3InterpolatorMap m_interpolators_V3;
			QInterpolatorMap m_interpolators_Q;
			RInterpolatorMap m_interpolators_R;

			BaseContinuousEventFactoryMap m_continuousEventFactories;
			BasePonctualEventFactoryMap m_ponctualEventFactories;

		public:
			SequenceManager();
			~SequenceManager();

		public:
			void RegisterInterpolator( const String & p_name, Vector3_Interpolator * p_interpolator );
			void RegisterInterpolator( const String & p_name, Quaternion_Interpolator * p_interpolator );
			void RegisterInterpolator( const String & p_name, Real_Interpolator * p_interpolator );

			void RegisterFactory( const String & p_name, BaseContinuousEventFactory * p_factory );
			void RegisterFactory( const String & p_name, BasePonctualEventFactory * p_factory );

			BasePonctualEvent * CreatePonctualEvent( const String & p_name, const StringArray & p_params );
			BaseContinuousEvent * CreateContinuousEvent( const String & p_name );

		public:
			void Update( Real p_time );
			StringArray GetAvailablePonctualEvents();
			StringArray GetAvailableContinuousEvents();

			Vector3_Interpolator * GetInterpolator_V3( const String & p_name );
			Quaternion_Interpolator * GetInterpolator_Q( const String & p_name );
			Real_Interpolator * GetInterpolator_R( const String & p_name );
		};
	}
}

#endif

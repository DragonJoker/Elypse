/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
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

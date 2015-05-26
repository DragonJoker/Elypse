#ifndef ___BASE_EVENT_H___
#define ___BASE_EVENT_H___

#include "Module_Sequences.h"
#include "BaseFrame.h"
#include "Interpolators.h"

namespace EMuse
{
	namespace Sequences
	{
		class d_abstract BasePonctualEvent
		{
		protected:
			void * m_target;

		public:
			BasePonctualEvent()
			{}
			virtual ~BasePonctualEvent() {}

		public:
			virtual void Apply() = 0;
			virtual void Rollback() = 0;

		public:
			inline void SetTarget( void * p_target )
			{
				m_target = p_target;
			}
			inline void * GetTarget()const
			{
				return m_target;
			}
		};

		class d_abstract BaseContinuousEvent
		{
		protected:
			void * m_target;

			SequenceTrackType m_type;

			Real m_length;
			Real m_currentTime;
			Real m_startTime;

		public:
			BaseContinuousEvent( SequenceTrackType p_type )
				:	m_target( NULL ),
					m_type( p_type ),
					m_length( 0.0 ),
					m_currentTime( 0.0 ),
					m_startTime( 0.0 )
			{}
			virtual ~BaseContinuousEvent() {}

		protected:
			inline void _getRemainingTime( Real p_time )
			{
				m_currentTime += p_time;
				m_currentTime = min( m_currentTime, m_length );
			}

		public:

			inline bool IsFinished()const
			{
				return m_currentTime >= m_length;
			}
			inline void SetTarget( void * p_target )
			{
				m_target = p_target;
			}
			inline SequenceTrackType GetType()const
			{
				return m_type;
			};
			inline Real GetLength()const
			{
				return m_length;
			}
			inline Real GetStartTime()const
			{
				return m_startTime;
			}

		public:
			virtual void Apply( Real p_time ) = 0;
			virtual void Rollback( Real p_time )
			{
				_getRemainingTime( p_time );
				Real l_time = m_currentTime;
				Reset();
				Apply( l_time );
			}
			virtual void UpdateTime( Real p_newStartTime ) = 0;
			virtual void CalcLength() = 0;
			virtual void Reset() = 0;
		};

		class d_abstract BaseContinuousV3Event : public BaseContinuousEvent
		{
		protected:
			BaseVector3FrameMap m_frames;
			Vector3_Interpolator * m_interpolator;

		public:
			BaseContinuousV3Event()
				:	BaseContinuousEvent( VECTOR3 )
			{}
			~BaseContinuousV3Event() {}

		public:
			virtual void Reset()
			{
				m_currentTime = 0.0;
				m_interpolator( m_frames, 0.0 );
			}

			inline void AddFrame( Real p_time, const Vector3 & p_vector3 )
			{
				if ( m_frames.empty() )
				{
					m_startTime = p_time;
				}

				m_frames.insert( BaseVector3FrameMap::value_type( p_time - m_startTime, BaseVector3Frame( p_time - m_startTime, p_vector3 ) ) );
			}

			virtual void CalcLength()
			{
				m_length = ( --m_frames.end() )->first - m_frames.begin()->first;
			}

			virtual void UpdateTime( Real p_newStartTime )
			{
				Real l_diff = p_newStartTime - m_startTime;
				m_startTime = p_newStartTime;
				BaseVector3FrameMap l_newMap;
				BaseVector3FrameMap::iterator l_it = m_frames.begin();

				while ( l_it != m_frames.end() )
				{
					l_newMap.insert( BaseVector3FrameMap::value_type( l_it->first + l_diff, l_it->second ) );
					++l_it;
				}

				m_frames.clear();
				m_frames = l_newMap;
			}

			inline void SetInterpolator( Vector3_Interpolator * p_interpolator )
			{
				m_interpolator = p_interpolator;
			}

			inline size_t GetNumFrames()const
			{
				return m_frames.size();
			}
		};

		class d_abstract BaseContinuousQEvent : public BaseContinuousEvent
		{
		protected:
			BaseQuaternionFrameMap m_frames;
			Quaternion_Interpolator * m_interpolator;

		public:
			BaseContinuousQEvent()
				:	BaseContinuousEvent( QUATERNION )
			{}
			~BaseContinuousQEvent() {}

		public:
			virtual void Reset()
			{
				m_currentTime = 0.0;
				m_interpolator( m_frames, 0.0 );
			}

			inline void AddFrame( Real p_time, const Quaternion & p_orientation )
			{
				if ( m_frames.empty() )
				{
					m_startTime = p_time;
				}

				m_frames.insert( BaseQuaternionFrameMap::value_type( p_time - m_startTime, BaseQuaternionFrame( p_time - m_startTime, p_orientation ) ) );
			}

			virtual void CalcLength()
			{
				m_length = ( --m_frames.end() )->first - m_frames.begin()->first;
			}

			virtual void UpdateTime( Real p_newStartTime )
			{
				Real l_diff = p_newStartTime - m_startTime;
				m_startTime = p_newStartTime;
				BaseQuaternionFrameMap l_newMap;
				BaseQuaternionFrameMap::iterator l_it = m_frames.begin();

				while ( l_it != m_frames.end() )
				{
					l_newMap.insert( BaseQuaternionFrameMap::value_type( l_it->first + l_diff, l_it->second ) );
					++l_it;
				}

				m_frames.clear();
				m_frames = l_newMap;
			}

			inline void SetInterpolator( Quaternion_Interpolator * p_interpolator )
			{
				m_interpolator = p_interpolator;
			}

			inline size_t GetNumFrames()const
			{
				return m_frames.size();
			}
		};

		class d_abstract BaseContinuousREvent : public BaseContinuousEvent
		{
		protected:
			BaseRealFrameMap m_frames;
			Real_Interpolator * m_interpolator;

		public:
			BaseContinuousREvent()
				:	BaseContinuousEvent( REAL )
			{}
			~BaseContinuousREvent() {}

		public:
			virtual void Reset()
			{
				m_currentTime = 0.0;
				m_interpolator( m_frames, 0.0 );
			}

			inline void AddFrame( Real p_time, Real p_real )
			{
				if ( m_frames.empty() )
				{
					m_startTime = p_time;
				}

				m_frames.insert( BaseRealFrameMap::value_type( p_time - m_startTime, BaseRealFrame( p_time - m_startTime, p_real ) ) );
			}

			virtual void CalcLength()
			{
				m_length = ( --m_frames.end() )->first - m_frames.begin()->first;
			}

			virtual void UpdateTime( Real p_newStartTime )
			{
				Real l_diff = p_newStartTime - m_startTime;
				m_startTime = p_newStartTime;
				BaseRealFrameMap l_newMap;
				BaseRealFrameMap::iterator l_it = m_frames.begin();

				while ( l_it != m_frames.end() )
				{
					l_newMap.insert( BaseRealFrameMap::value_type( l_it->first + l_diff, l_it->second ) );
					++l_it;
				}

				m_frames.clear();
				m_frames = l_newMap;
			}

			inline void SetInterpolator( Real_Interpolator * p_interpolator )
			{
				m_interpolator = p_interpolator;
			}

			inline size_t GetNumFrames()const
			{
				return m_frames.size();
			}
		};
	}
}

#endif

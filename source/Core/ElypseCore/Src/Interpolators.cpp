#include "PrecompiledHeader.h"

#include "Interpolators.h"
#include "BaseFrame.h"

#include <Bezier.h>

bool Interpolators::_getFramesV3( const BaseVector3FrameMap & p_frames, const BaseVector3Frame *& p_start, const BaseVector3Frame *& p_end, Real & p_time )
{
	BaseVector3FrameMap::const_iterator ileft = p_frames.lower_bound( p_time );

	if ( ileft == p_frames.end() )
	{
		-- ileft;
		p_start = & ileft->second;
		p_end = & ileft->second;
		p_time = 0.0;
		return true;
	}

	if ( ileft == p_frames.begin() )
	{
		p_start = & ileft->second;
		++ ileft;
		p_end = & ileft->second;
	}
	else
	{
		p_end = & ileft->second;
		-- ileft;
		p_start = & ileft->second;
	}

	p_time -= p_start->time;
	p_time /= ( p_end->time - p_start->time );
	return true;
}

bool Interpolators::_getFramesQ( const BaseQuaternionFrameMap & p_frames, const BaseQuaternionFrame *& p_start, const BaseQuaternionFrame *& p_end, Real & p_time )
{
	BaseQuaternionFrameMap::const_iterator ileft = p_frames.lower_bound( p_time );

	if ( ileft == p_frames.end() )
	{
		-- ileft;
		p_start = & ileft->second;
		p_end = & ileft->second;
		p_time = 0.0;
		return true;
	}

	if ( ileft == p_frames.begin() )
	{
		p_start = & ileft->second;
		++ ileft;
		p_end = & ileft->second;
	}
	else
	{
		p_end = & ileft->second;
		-- ileft;
		p_start = & ileft->second;
	}

	p_time -= p_start->time;
	p_time /= ( p_end->time - p_start->time );
	return true;
}

bool Interpolators::_getFramesR( const BaseRealFrameMap & p_frames, const BaseRealFrame *& p_start, const BaseRealFrame *& p_end, Real & p_time )
{
	BaseRealFrameMap::const_iterator ileft = p_frames.lower_bound( p_time );

	if ( ileft == p_frames.end() )
	{
		-- ileft;
		p_start = & ileft->second;
		p_end = & ileft->second;
		p_time = 0.0;
		return true;
	}

	if ( ileft == p_frames.begin() )
	{
		p_start = & ileft->second;
		++ ileft;
		p_end = & ileft->second;
	}
	else
	{
		p_end = & ileft->second;
		-- ileft;
		p_start = & ileft->second;
	}

	p_time -= p_start->time;
	p_time /= ( p_end->time - p_start->time );
	return true;
}

Vector3 Interpolators::V3_Linear( const BaseVector3FrameMap & p_frames, Real p_time )
{
	const BaseVector3Frame * l_start;
	const BaseVector3Frame * l_end;

	if ( ! _getFramesV3( p_frames, l_start, l_end, p_time ) )
	{
		return Vector3::ZERO;
	}

	return Vector3( l_start->vector3 + ( l_end->vector3 - l_start->vector3 ) * p_time );
}

Vector3 Interpolators::V3_Sinus( const BaseVector3FrameMap & p_frames, Real p_time )
{
	const BaseVector3Frame * l_start;
	const BaseVector3Frame * l_end;

	if ( ! _getFramesV3( p_frames, l_start, l_end, p_time ) )
	{
		return Vector3::ZERO;
	}

	Radian l_angle = Radian( Math::PI * p_time - Math::HALF_PI );
	Real l_interpolatedValue = Real( ( Math::Sin( l_angle ) + 1.0 ) / 2.0 );
	return Vector3( l_end->vector3 * ( l_interpolatedValue ) + l_start->vector3 * ( 1 - l_interpolatedValue ) );
}

Quaternion Interpolators::Q_Linear( const BaseQuaternionFrameMap & p_frames, Real p_time )
{
	const BaseQuaternionFrame * l_start;
	const BaseQuaternionFrame * l_end;

	if ( ! _getFramesQ( p_frames, l_start, l_end, p_time ) )
	{
		return Quaternion::IDENTITY;
	}

	return Quaternion::Slerp( p_time, l_start->quaternion, l_end->quaternion );
}

Real Interpolators::R_Linear( const BaseRealFrameMap & p_frames, Real p_time )
{
	const BaseRealFrame * l_start;
	const BaseRealFrame * l_end;

	if ( ! _getFramesR( p_frames, l_start, l_end, p_time ) )
	{
		return 0.0;
	}

	return l_start->real + ( l_end->real - l_start->real ) * p_time;
}


Vector3 Interpolators::V3_Bezier3( const BaseVector3FrameMap & p_frames, Real p_time )
{
	if ( p_frames.size() != 3 )
	{
		return Vector3::ZERO;
	}

	BaseVector3FrameMap::const_iterator i = p_frames.begin();
	const Vector3 & l_a = i->second.vector3;
	const Vector3 & l_b = ( ++ i )->second.vector3;
	const Vector3 & l_c = ( ++ i )->second.vector3;
	p_time /= i->first;
	Real * l_factors = General::Math::Bezier<3>::GetSingletonPtr()->Get( p_time );
	return Vector3( l_a * l_factors[0] + l_b * l_factors[1] + l_c * l_factors[2] );
}

Vector3 Interpolators::V3_Bezier4( const BaseVector3FrameMap & p_frames, Real p_time )
{
	if ( p_frames.size() != 4 )
	{
		return Vector3::ZERO;
	}

	BaseVector3FrameMap::const_iterator i = p_frames.begin();
	const Vector3 & l_a = i->second.vector3;
	const Vector3 & l_b = ( ++ i )->second.vector3;
	const Vector3 & l_c = ( ++ i )->second.vector3;
	const Vector3 & l_d = ( ++ i )->second.vector3;
	p_time /= i->first;
	Real * l_factors = General::Math::Bezier<4>::GetSingletonPtr()->Get( p_time );
	return Vector3( l_a * l_factors[0] + l_b * l_factors[1] + l_c * l_factors[2] + l_d * l_factors[3] );
}

Vector3 Interpolators::V3_Bezier5( const BaseVector3FrameMap & p_frames, Real p_time )
{
	if ( p_frames.size() != 5 )
	{
		return Vector3::ZERO;
	}

	BaseVector3FrameMap::const_iterator i = p_frames.begin();
	const Vector3 & l_a = i->second.vector3;
	++ i;
	const Vector3 & l_b = ( ++ i )->second.vector3;
	const Vector3 & l_c = ( ++ i )->second.vector3;
	const Vector3 & l_d = ( ++ i )->second.vector3;
	const Vector3 & l_e = ( ++ i )->second.vector3;
	p_time /= i->first;
	Real * l_factors = General::Math::Bezier <5> ::GetSingletonPtr()->Get( p_time );
	return Vector3( l_a * l_factors[0] + l_b * l_factors[1] + l_c * l_factors[2] + l_d * l_factors[3] + l_e * l_factors[4] );
}

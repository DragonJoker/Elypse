/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "SequenceManager.h"

#include "Interpolators.h"
#include "BaseFrame.h"

#include "EventFactory.h"

#include "Context.h"
#include "ElypseLogs.h"

SequenceManager::SequenceManager()
{
	RegisterInterpolator( "linear", Interpolators::V3_Linear );
	RegisterInterpolator( "linear", Interpolators::Q_Linear );
	RegisterInterpolator( "linear", Interpolators::R_Linear );
	RegisterInterpolator( "sinus", Interpolators::V3_Sinus );
	RegisterInterpolator( "bezier3", Interpolators::V3_Bezier3 );
	RegisterInterpolator( "bezier4", Interpolators::V3_Bezier4 );
	RegisterInterpolator( "bezier5", Interpolators::V3_Bezier5 );
	RegisterFactory( "SceneNode_SetPosition", new SceneNode_SetPosition_Factory );
	RegisterFactory( "SceneNode_SetScale", new SceneNode_SetScale_Factory );
	RegisterFactory( "Overlay_Hide", new Overlay_Hide_Factory );
	RegisterFactory( "Overlay_Show", new Overlay_Show_Factory );
	RegisterFactory( "Function_Execute", new ScriptNode_Execution_Factory );
	RegisterFactory( "Sequence_Start", new Sequence_Starting_Factory );
	RegisterFactory( "Sequence_Pause", new Sequence_Pausing_Factory );
	RegisterFactory( "Sequence_Unpause", new Sequence_Unpausing_Factory );
	RegisterFactory( "Sequence_Stop", new Sequence_Stopping_Factory );
	RegisterFactory( "SceneNode_Translate", new SceneNode_Translation_Factory );
	RegisterFactory( "SceneNode_Rotate", new SceneNode_Rotation_Factory );
	RegisterFactory( "SceneNode_Scale", new SceneNode_Scale_Factory );
	RegisterFactory( "Overlay_Translate", new Overlay_Translation_Factory );
	RegisterFactory( "Overlay_Rotate", new Overlay_Rotation_Factory );
	RegisterFactory( "Overlay_Resize", new Overlay_Resizing_Factory );
	RegisterFactory( "Overlay_TranslateRelative", new Overlay_RelativeTranslation_Factory );
	RegisterFactory( "Overlay_ResizeRelative", new Overlay_RelativeResizing_Factory );
}

SequenceManager::~SequenceManager()
{
	General::Utils::map::deleteAll( m_continuousEventFactories );
	General::Utils::map::deleteAll( m_ponctualEventFactories );
}

void SequenceManager::Update( Real p_time )
{
	for ( auto l_it : m_objectMap )
	{
		l_it.second->Update( p_time );
	}
}

StringArray SequenceManager::GetAvailablePonctualEvents()
{
	StringArray l_result;
	BasePonctualEventFactoryMap::iterator l_it = m_ponctualEventFactories.begin();

	while ( l_it != m_ponctualEventFactories.end() )
	{
		l_result.push_back( l_it->first );
		++l_it;
	}

	return l_result;
}

StringArray SequenceManager::GetAvailableContinuousEvents()
{
	StringArray l_result;
	BaseContinuousEventFactoryMap::iterator l_it = m_continuousEventFactories.begin();

	while ( l_it != m_continuousEventFactories.end() )
	{
		l_result.push_back( l_it->first );
		++l_it;
	}

	return l_result;
}

void SequenceManager::RegisterInterpolator( String const & p_name, Vector3_Interpolator * p_interpolator )
{
	m_interpolators_V3.insert( std::make_pair( p_name, p_interpolator ) );
}

void SequenceManager::RegisterInterpolator( String const & p_name, Quaternion_Interpolator * p_interpolator )
{
	m_interpolators_Q.insert( std::make_pair( p_name, p_interpolator ) );
}

void SequenceManager::RegisterInterpolator( String const & p_name, Real_Interpolator * p_interpolator )
{
	m_interpolators_R.insert( std::make_pair( p_name, p_interpolator ) );
}

void SequenceManager::RegisterFactory( String const & p_name, BaseContinuousEventFactory * p_factory )
{
	if ( General::Utils::map::has( m_continuousEventFactories, p_name ) )
	{
		General::Utils::map::deleteValue( m_continuousEventFactories, p_name );
	}

	m_continuousEventFactories.insert( std::make_pair( p_name, p_factory ) );
}

void SequenceManager::RegisterFactory( String const & p_name, BasePonctualEventFactory * p_factory )
{
	if ( General::Utils::map::has( m_ponctualEventFactories, p_name ) )
	{
		General::Utils::map::deleteValue( m_ponctualEventFactories, p_name );
	}

	m_ponctualEventFactories.insert( std::make_pair( p_name, p_factory ) );
}

Vector3_Interpolator * SequenceManager::GetInterpolator_V3( String const & p_name )
{
	const V3InterpolatorMap::iterator & ifind = m_interpolators_V3.find( p_name );

	if ( ifind != m_interpolators_V3.end() )
	{
		return ifind->second;
	}

	return Interpolators::V3_Linear;
}

Quaternion_Interpolator * SequenceManager::GetInterpolator_Q( String const & p_name )
{
	const QInterpolatorMap::iterator & ifind = m_interpolators_Q.find( p_name );

	if ( ifind != m_interpolators_Q.end() )
	{
		return ifind->second;
	}

	return Interpolators::Q_Linear;
}

Real_Interpolator * SequenceManager::GetInterpolator_R( String const & p_name )
{
	const RInterpolatorMap::iterator & ifind = m_interpolators_R.find( p_name );

	if ( ifind != m_interpolators_R.end() )
	{
		return ifind->second;
	}

	return Interpolators::R_Linear;
}

BasePonctualEvent * SequenceManager::CreatePonctualEvent( String const & p_name, const StringArray & p_params )
{
	BasePonctualEventFactory * l_factory = General::Utils::map::findOrNull( m_ponctualEventFactories, p_name );

	if ( l_factory == NULL )
	{
		EMUSE_MESSAGE_RELEASE( "SequenceManager::CreatePonctualEvent - Can't find ponctual event factory for " + p_name );
		return NULL;
	}

	return l_factory->CreateEvent( p_params );
}

BaseContinuousEvent * SequenceManager::CreateContinuousEvent( String const & p_name )
{
	BaseContinuousEventFactory * l_factory = General::Utils::map::findOrNull( m_continuousEventFactories, p_name );

	if ( l_factory == NULL )
	{
		EMUSE_MESSAGE_RELEASE( "SequenceManager::CreateContinuousEvent - Can't find continuous event factory for " + p_name );
		return NULL;
	}

	return l_factory->CreateEvent();
}

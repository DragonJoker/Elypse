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
#ifndef ___EMUSE_FRAME_LISTENER___
#define ___EMUSE_FRAME_LISTENER___

#include "Module_Main.h"

#include "Module_Media.h"
#include "Module_Animation.h"
#include "Module_Download.h"
#include "Module_Script.h"
#include "Module_Gui.h"
#include "Module_Physics.h"
#include "Module_SpecialEffects.h"
#include "Module_SceneFile.h"
#include "Module_Zone.h"
#include "Module_Sequences.h"
#include "Module_Network.h"
#include "Module_Data.h"

#include <Mutex.h>
#include <ThreadedQueue.h>
#include <OwnedBy.h>

namespace Elypse
{
	namespace Main
	{
		class d_dll_export ElypseFrameListener
			: private noncopyable
			, private owned_by< ElypseInstance >
		{
		protected:
			enum 	//variables
			{
				MOUSE_POSITION_X,
				MOUSE_POSITION_Y,
				MOUSE_DIFFERENCE_X,
				MOUSE_DIFFERENCE_Y,
				GENERAL_ELAPSED_TIME,
				KEY_CURRENT,
				NETWORK_CURRENT_MESSAGE,
				NUM_SCRIPT_VAR
			};

		protected:
			void _updateStats();
			void _updateMedia( Real p_time );

			virtual void _initialiseFromExisting( ElypseFrameListener * p_other );

			virtual void _setupEffectsManager();
			virtual void _setupUniverse();
			virtual void _setupInterfaces();
			virtual void _setupScript();
			virtual void _setupMainScript();
			virtual void _setupBinds();
			void _clearAndSetupBind( ScriptNode *& p_bind, ScriptNode * p_node );
			void _destroyBinds();

			char _keycodeToChar( uint32_t p_keycode );

			inline void _tryPush( ScriptNode * p_node )
			{
				if ( p_node != NULL )
				{
					m_scriptQueue.Push( p_node );
				}
			}

		public:
			ElypseFrameListener( ElypseInstance & p_instance,
								 RenderWindow * m_window, Url const & p_baseURL,
								 const Path & p_installPath, String const & p_appStringIndex );
			ElypseFrameListener( ElypseInstance & p_instance, ElypseFrameListener * p_ofl,
								 RenderWindow * p_window, String const & p_instanceNum );
			virtual ~ElypseFrameListener();

		public:
			virtual void Initialise();
			virtual void PreInitialise();

			void ShowDebugOverlay( bool p_show );
			virtual bool frameStarted( Real p_timeSinceLastFrame );
			virtual bool frameEnded( Real p_timeSinceLastFrame );

			void MouseMove( Real x , Real y );
			void SetMouse( Real x, Real y );
			void MouseRDown( Real x, Real y );
			void MouseLDown( Real x, Real y );
			void MouseMDown( Real x, Real y );
			void MouseRUp( Real x, Real y );
			void MouseLUp( Real x, Real y );
			void MouseMUp( Real x, Real y );

			void MouseWheel( short p_direction );

			void ResetAll();

			void KeyDown( uint32_t p_keyCode );
			void KeyUp( uint32_t p_keyCode );
			void KeyRepeat( uint32_t p_keyCode, uint32_t p_numRepeat );
			void OnChar( uint32_t p_keyCode );
			void OnCharRepeat( uint32_t p_keyCode, uint32_t p_numRepeat );

			void AddToScriptQueue( ScriptNode * p_node );
			void AddToScriptQueue( String const & p_script );

			void SetCallback( CallbackType p_type, ScriptNode * p_node );
		public:
			inline ScriptEngine * GetScriptEngine()const
			{
				return m_scriptEngine;
			}
			inline bool IsReady()const
			{
				return m_ready;
			}
			inline Context * GetContext()const
			{
				return m_context;
			}
			inline String const &	GetFileName()const
			{
				return m_filename;
			}
			inline const Path 	&	GetInstallDir()const
			{
				return m_installDir;
			}

			inline void	SetStartupScript( String const & p_script )
			{
				m_startupScript = p_script;
			}

		protected:
			Overlay * m_debugOverlay{ nullptr };
			RenderWindow * m_window{ nullptr };

			SceneManager * m_sceneManager{ nullptr };
			Camera * m_camera{ nullptr };

			EMGui * m_gui{ nullptr };
			PhysicsEngine * m_physics{ nullptr };
			ScriptEngine * m_scriptEngine{ nullptr };
			SceneFileParser * m_parser{ nullptr };
			MirrorManager * m_mirrorManager{ nullptr };
			UniverseManager * m_universeManager{ nullptr };
			CamTexManager * m_camTexManager{ nullptr };
			PostEffectManager * m_postEffectsManager{ nullptr };
			AnimationManager * m_animationManager{ nullptr };
			ObjectMaterialManager * m_objectMaterialManager{ nullptr };
			SequenceManager * m_sequenceManager{ nullptr };
			Network::NetworkManager * m_network{ nullptr };

			MuseFile * m_museFile{ nullptr };

			Context * m_context{ nullptr };

			std::array< ScriptNode *, NUM_SCRIPT_VAR > m_scriptVars;
			std::array< ScriptNode *, NUM_SCRIPT_BINDS > m_scriptBinds;

			ScriptNode * m_startUpScriptNode{ nullptr };

			ThreadedQueue< ScriptNode * > m_scriptQueue{ 50 };
			ThreadedQueue< ScriptNode * > m_deletableScriptQueue{ 50 };

			bool * m_keysDown{ nullptr };

			bool m_main{ true };
			bool m_showFPS{ false };
			bool m_ready{ false };

			bool m_mouseMove{ false };
			bool m_mouseRDown{ false };
			bool m_mouseLDown{ false };
			bool m_mouseMDown{ false };

			Url m_mainUrl;
			String m_filename;
			String m_appIndexStr;

			String m_startupScript;

			Path m_installDir;

			std::mutex m_mutex;
		};
	}
}

#endif

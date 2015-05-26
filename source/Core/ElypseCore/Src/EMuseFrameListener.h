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

namespace EMuse
{
	namespace Main
	{
		class d_dll_export EMuseFrameListener : noncopyable, owned_by <EMuseInstance>
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
			Overlay * m_debugOverlay;
			RenderWindow * m_window;

			SceneManager * m_sceneManager;
			Camera * m_camera;

			EMGui * m_gui;
			PhysicsEngine * m_physics;
			ScriptEngine * m_scriptEngine;
			SceneFileParser * m_parser;
			MirrorManager * m_mirrorManager;
			UniverseManager * m_universeManager;
			CamTexManager * m_camTexManager;
			PostEffectManager * m_postEffectsManager;
			AnimationManager * m_animationManager;
			ObjectMaterialManager * m_objectMaterialManager;
			SequenceManager * m_sequenceManager;
			NetworkManager * m_network;

			MuseFile * m_museFile;

			Context * m_context;

			ScriptNode * m_scriptVars[NUM_SCRIPT_VAR];
			ScriptNode * m_scriptBinds[NUM_SCRIPT_BINDS];

			ScriptNode * m_startUpScriptNode;

			ThreadedQueue <ScriptNode *> m_scriptQueue;
			ThreadedQueue <ScriptNode *> m_deletableScriptQueue;



			bool * m_keysDown;

			bool m_main;
			bool m_showFPS;
			bool m_ready;

			bool m_mouseMove;
			bool m_mouseRDown;
			bool m_mouseLDown;
			bool m_mouseMDown;

			Url m_mainUrl;
			String m_filename;
			String m_appIndexStr;

			String m_startupScript;

			Path m_installDir;

			Mutex m_mutex;

		protected:
			void _updateStats();
			void _updateMedia( Real p_time );

			virtual void _initialiseFromExisting( EMuseFrameListener * p_other );

			virtual void _setupEffectsManager();
			virtual void _setupUniverse();
			virtual void _setupInterfaces();
			virtual void _setupScript();
			virtual void _setupMainScript();
			virtual void _setupBinds();
			void _clearAndSetupBind( ScriptNode *& p_bind, ScriptNode * p_node );
			void _destroyBinds();

			char _keycodeToChar( unsigned int p_keycode );

			inline void _tryPush( ScriptNode * p_node )
			{
				if ( p_node != NULL )
				{
					m_scriptQueue.Push( p_node );
				}
			}

		public:
			EMuseFrameListener( EMuseInstance * p_oa,
								RenderWindow * m_window, const Url & p_baseURL,
								const Path & p_installPath, const String & p_appStringIndex );
			EMuseFrameListener( EMuseInstance * p_oa, EMuseFrameListener * p_ofl,
								RenderWindow * p_window, const String & p_instanceNum );
			virtual ~EMuseFrameListener();

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

			void KeyDown( unsigned int p_keyCode );
			void KeyUp( unsigned int p_keyCode );
			void KeyRepeat( unsigned int p_keyCode, unsigned int p_numRepeat );
			void OnChar( unsigned int p_keyCode );
			void OnCharRepeat( unsigned int p_keyCode, unsigned int p_numRepeat );

			void AddToScriptQueue( ScriptNode * p_node );
			void AddToScriptQueue( const String & p_script );

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
			inline const String &	GetFileName()const
			{
				return m_filename;
			}
			inline const Path 	&	GetInstallDir()const
			{
				return m_installDir;
			}

			inline void	SetStartupScript( const String & p_script )
			{
				m_startupScript = p_script;
			}
		};
	}
}

#endif

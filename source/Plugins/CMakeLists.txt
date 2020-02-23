option( BUILD_PLUGINS_ELYPSE_OCX "Build ElypseOcx" TRUE )
option( BUILD_PLUGINS_ELYPSE_FFX "Build ElypseFirefox" TRUE )

function( PluginsInit )
	set( ElpsOcx "no (Not wanted)" PARENT_SCOPE )
	set( ElpsFfx "no (Not wanted)" PARENT_SCOPE )
endfunction( PluginsInit )

function( PluginsBuild )
	if ( WIN32 AND BUILD_PLUGINS_ELYPSE_OCX )
		set( Build ${ElpsOcx} )
		add_subdirectory( Plugins/ElypseOcx )
		set( ElpsOcx ${Build} PARENT_SCOPE )
	endif ()
	if ( BUILD_PLUGINS_ELYPSE_FFX )
		set( Build ${ElpsFfx} )
		add_subdirectory( Plugins/ElypseFirefox )
		set( ElpsFfx ${Build} PARENT_SCOPE )
	endif ()
	set( OPTIONAL_PROJECTS
		${OPTIONAL_PROJECTS}
		PARENT_SCOPE
	)
endfunction( PluginsBuild )

function( PluginsSummary msg )
	set( msg_tmp "\nPlugins :" )
	set( msg_tmp "${msg_tmp}\n    ElypseOcx          ${ElpsOcx}" )
	set( msg_tmp "${msg_tmp}\n    ElypseFirefox      ${ElpsFfx}" )
	set( msg "${msg}${msg_tmp}" PARENT_SCOPE )
endfunction( PluginsSummary )

function( PluginsSetup )
	cpack_add_component_group( Plugins
		EXPANDED
		DISPLAY_NAME "${MAIN_PROJECT_NAME} Plugins"
		DESCRIPTION "${MAIN_PROJECT_NAME} browser plugins."
	)

	if ( WIN32 AND BUILD_PLUGINS_ELYPSE_OCX )
		cpack_add_component( ElypseOcx
			DISPLAY_NAME "ElypseOcx Plugin"
			DESCRIPTION "OCX plugin for ${MAIN_PROJECT_NAME}"
			REQUIRED
			GROUP Plugins
			DEPENDS ElypseCore
			INSTALL_TYPES Minimal Developer Full
		)
	endif ()
	if ( BUILD_PLUGINS_ELYPSE_FFX )
		cpack_add_component( ElypseFirefox
			DISPLAY_NAME "ElypseFirefox Plugin"
			DESCRIPTION "Firefox plugin for ${MAIN_PROJECT_NAME}"
			REQUIRED
			GROUP Plugins
			DEPENDS ElypseCore
			INSTALL_TYPES Minimal Developer Full
		)
	endif ()
endfunction( PluginsSetup )

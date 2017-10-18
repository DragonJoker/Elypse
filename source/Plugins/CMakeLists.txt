option( BUILD_PLUGINS_ELYPSE_OCX "Build ElypseOcx" TRUE )

function( PluginsInit )
	set( ElpsOcx "no (Not wanted)" PARENT_SCOPE )
endfunction( PluginsInit )

function( PluginsBuild )
	if ( WIN32 AND BUILD_PLUGINS_ELYPSE_OCX )
		set( Build ${ElpsOcx} )
		add_subdirectory( Plugins/ElypseOcx )
		set( ElpsOcx ${Build} PARENT_SCOPE )
	endif ()
endfunction( PluginsBuild )

function( PluginsSummary msg )
	set( msg_tmp "\nPlugins :" )
	set( msg_tmp "${msg_tmp}\n    ElypseOcx          ${ElpsOcx}" )
	set( msg "${msg}${msg_tmp}" PARENT_SCOPE )
endfunction( PluginsSummary )

function( PluginsSetup )
	cpack_add_component_group( Plugins
		EXPANDED
		DISPLAY_NAME "Elypse plugins"
		DESCRIPTION "Borwser plugins using ${MAIN_PROJECT_NAME}"
	)

	cpack_add_component( ElypseOcx
		DISPLAY_NAME "ElypseOcx Plugin"
		DESCRIPTION "OCS to be used in InternetExplorer"
		REQUIRED
		GROUP Tools
		DEPENDS ElypseCore
		INSTALL_TYPES Minimal Developer Full
	)
endfunction( PluginsSetup )

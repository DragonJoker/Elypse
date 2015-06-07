option( BUILD_TOOLS_ELYPSE_BINARY "Build ElypseBinary" TRUE )
option( BUILD_TOOLS_MUSE_READER "Build MuseReader" TRUE )
option( BUILD_TOOLS_MUSE_WRITER "Build MuseWriter" TRUE )
option( BUILD_TOOLS_TROLL_EDITOR "Build TrollEditor" TRUE )

function( ToolsInit )
	set( ElpsBinry "no (Not wanted)" PARENT_SCOPE )
	set( MuseReadr "no (Not wanted)" PARENT_SCOPE )
	set( MuseWritr "no (Not wanted)" PARENT_SCOPE )
	set( TrollEdtr "no (Not wanted)" PARENT_SCOPE )
endfunction( ToolsInit )

function( ToolsBuild )
	set( wxWidgets_USE_STATIC OFF)

	if (WIN32)
		find_package( wxWidgets REQUIRED core base aui net stc scintilla )
	else ()
		find_package( wxWidgets REQUIRED core base aui net stc )
	endif()

	find_package( OpenGL )

	if ( wxWidgets_FOUND AND GTK2_FOUND AND OPENGL_FOUND )
		message( STATUS "+ Found wxWidgets" )
		include_directories( ${wxWidgets_INCLUDE_DIRS} )

		set( wxWidgetsLibraries "" )
		FOREACH( Lib ${wxWidgets_LIBRARIES} )
			if ( wxWidgetsLibraries )
				set( wxWidgetsLibraries "${wxWidgetsLibraries}|${Lib}" )
			else()
				set( wxWidgetsLibraries "${Lib}" )
			endif()
		ENDFOREACH()
		
		set( OpenGlLibraries "" )
		foreach( Lib ${OPENGL_gl_LIBRARY} )
			if( OpenGlLibraries )
				set( OpenGlLibraries "${OpenGlLibraries}|${Lib}" )
			else()
				set( OpenGlLibraries "${Lib}" )
			endif()
		endforeach()

		#Few configuration for wxWidgets
		string( COMPARE EQUAL "${wxWidgets_USE_DEBUG}" "ON" IsWxDebug )
		add_definitions( -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES )
		if ( NOT WIN32 )
			add_definitions( -D__WXGTK__ )
		endif ()
		if ( IsWxDebug )
			add_definitions( -D__WXDEBUG__ )
		endif ()
		
		set( BinariesLibraries
			${wxWidgetsLibraries}
			${OpenGlLibraries}
		)
		
		if ( BUILD_TOOLS_ELYPSE_BINARY )
			set( Build ${ElpsBinry} )
			add_subdirectory( Tools/ElypseBinary )
			set( ElpsBinry ${Build} PARENT_SCOPE )
		endif ()
		
		if ( BUILD_TOOLS_MUSE_READER )
			set( Build ${MuseReadr} )
			add_subdirectory( Tools/MuseReader )
			set( MuseReadr ${Build} PARENT_SCOPE )
		endif ()
		
		if ( BUILD_TOOLS_MUSE_WRITER )
			set( Build ${MuseWritr} )
			add_subdirectory( Tools/MuseWriter )
			set( MuseWritr ${Build} PARENT_SCOPE )
		endif ()
		
		if ( BUILD_TOOLS_TROLL_EDITOR )
			set( Build ${TrollEdtr} )
			add_subdirectory( Tools/TrollEditor )
			set( TrollEdtr ${Build} PARENT_SCOPE )
		endif ()
	else()
		set( ElpsBinry "No (missing: " )
		set( MuseReadr "No (missing: " )
		set( TrollEdtr "No (missing: " )
		set( SEP "" )
		if ( NOT wxWidgets_FOUND )
			set( ElpsBinry "${SEP}${ElpsBinry}wxWidgets library " )
			set( MuseReadr "${SEP}${MuseReadr}wxWidgets library " )
			set( TrollEdtr "${SEP}${MuseWritr}wxWidgets library " )
			set( TrollEdtr "${SEP}${TrollEdtr}wxWidgets library " )
			set( SEP ", " )
		endif()
		if ( NOT GTK_FOUND )
			set( ElpsBinry "${SEP}${ElpsBinry}GTK library" )
			set( MuseReadr "${SEP}${MuseReadr}GTK library" )
			set( MuseReadr "${SEP}${MuseWritr}GTK library" )
			set( TrollEdtr "${SEP}${TrollEdtr}GTK library" )
			set( SEP ", " )
		endif()
		if ( NOT OPENGL_FOUND )
			set( ElpsBinry "${SEP}${ElpsBinry}OpenGL library" )
			set( MuseReadr "${SEP}${MuseReadr}OpenGL library" )
			set( MuseReadr "${SEP}${MuseWritr}OpenGL library" )
			set( TrollEdtr "${SEP}${TrollEdtr}OpenGL library" )
			set( SEP ", " )
		endif()
		set( ElpsBinry "${ElpsBinry})" )
		set( MuseReadr "${MuseReadr})" )
		set( MuseReadr "${MuseWritr})" )
		set( TrollEdtr "${TrollEdtr})" )
	endif ()
endfunction( ToolsBuild )

function( ToolsSummary msg )
	set( msg_tmp "\nTools :" )
	set( msg_tmp "${msg_tmp}\n    ElypseBinary       ${ElpsBinry}" )
	set( msg_tmp "${msg_tmp}\n    MuseReader         ${MuseReadr}" )
	set( msg_tmp "${msg_tmp}\n    MuseWriter         ${MuseWritr}" )
	set( msg_tmp "${msg_tmp}\n    TrollEditor        ${TrollEdtr}" )
	set( msg "${msg}${msg_tmp}" PARENT_SCOPE )
endfunction( ToolsSummary )

function( ToolsSetup )
	cpack_add_component_group( Tools
		EXPANDED
		DISPLAY_NAME "Binary tools"
		DESCRIPTION "Tools used to develop new scenes using ${MAIN_PROJECT_NAME}"
	)

	cpack_add_component( ElypseBinary
		DISPLAY_NAME "ElypseBinary Application"
		DESCRIPTION "Simple executable that can run scenes in a single muse file"
		REQUIRED
		GROUP Tools
		DEPENDS ElypseCore
		INSTALL_TYPES Minimal Developer Full
	)
	cpack_add_component( MuseReader
		DISPLAY_NAME "MuseReader Application"
		DESCRIPTION "Application used to read the content of a muse file"
		GROUP Tools
		DEPENDS ElypseCore
		INSTALL_TYPES Developer Full
	)
	cpack_add_component( MuseWriter
		DISPLAY_NAME "MuseWriter Application"
		DESCRIPTION "Application used to builf muse files from scene files, script files and folders"
		GROUP Tools
		DEPENDS ElypseCore
		INSTALL_TYPES Developer Full
	)
	cpack_add_component( TrollEditor
		DISPLAY_NAME "TrollEditor Application"
		DESCRIPTION "Application used to create new scenes"
		GROUP Tools
		DEPENDS ElypseCore
		INSTALL_TYPES Developer Full
	)
endfunction( ToolsSetup )

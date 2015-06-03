function( CoreInit )
	set( GenLib "no (Not wanted)" PARENT_SCOPE )
	set( ElpsCore "no (Not wanted)" PARENT_SCOPE )
endfunction( CoreInit )

function( CoreBuild )
	set( Build ${GenLib} )
	add_subdirectory( Core/GeneralLib )
	set( GenLib ${Build} PARENT_SCOPE )
	
	set( Build ${ElpsCore} )
	add_subdirectory( Core/ElypseCore )
	set( ElpsCore ${Build} PARENT_SCOPE )
endfunction( CoreBuild )

function( CoreSummary msg )
	set( msg_tmp "\nCore :" )
	set( msg_tmp "${msg_tmp}\n    GeneralLib         ${Cutils}" )
	set( msg_tmp "${msg_tmp}\n    ElypseCore         ${C3D}" )
	SET( msg "${msg}${msg_tmp}" PARENT_SCOPE )
endfunction( CoreSummary )

function( CoreSetup )
	cpack_add_component_group( Core
		EXPANDED
		DISPLAY_NAME "Core libraries"
		DESCRIPTION "Base libraries needed to use ${MAIN_PROJECT_NAME}"
	)
	cpack_add_component_group( Development
		DISPLAY_NAME "Development libraries and headers"
		DESCRIPTION "All the tools needed to develop a project using ${MAIN_PROJECT_NAME}"
	)
	cpack_add_component_group( Documentation
		DISPLAY_NAME "Documentation"
		DESCRIPTION "Projects documentations"
	)
	cpack_add_component_group( Samples
		DISPLAY_NAME "${MAIN_PROJECT_NAME} samples and sample applications"
		DESCRIPTION "Sample applications and scenes using ${MAIN_PROJECT_NAME}"
	)

	cpack_add_component( GeneralLib
		DISPLAY_NAME "GeneralLib library"
		DESCRIPTION "Base utilities library used in ${MAIN_PROJECT_NAME}"
		REQUIRED
		GROUP Core
		INSTALL_TYPES Minimal Full
	)
	cpack_add_component( GeneralLib_dev
		DISPLAY_NAME "GeneralLib SDK"
		DESCRIPTION "Development files for GeneralLib"
		GROUP Development
		DEPENDS GeneralLib
		INSTALL_TYPES Developer Full
	)
	cpack_add_component( GeneralLib_doc
		DISPLAY_NAME "GeneralLib help"
		DESCRIPTION "GeneralLib HTML documentation"
		GROUP Documentation
		INSTALL_TYPES Developer Full
	)

	cpack_add_component( ElypseCore
		DISPLAY_NAME "ElypseCore Engine"
		DESCRIPTION "The main ElypseCore shared library, including the necessary files to be able to build projects using ElypseCore"
		REQUIRED
		GROUP Core
		DEPENDS GeneralLib
		INSTALL_TYPES Minimal Full
	)
	cpack_add_component( ElypseCore_dev
		DISPLAY_NAME "ElypseCore SDK"
		DESCRIPTION "Development files for ElypseCore"
		GROUP Development
		DEPENDS GeneralLib GeneralLib_dev ElypseCore
		INSTALL_TYPES Developer Full
	)
	cpack_add_component( ElypseCore_doc
		DISPLAY_NAME "ElypseCore help"
		DESCRIPTION "ElypseCore HTML documentation"
		GROUP Documentation
		INSTALL_TYPES Developer Full
	)
endfunction( CoreSetup )

include_directories( ${CMAKE_SOURCE_DIR}/Core/GeneralLib/Src )

add_subdirectory( Core )
add_subdirectory( Tools )
add_subdirectory( Miniline )
add_subdirectory( Plugins )

CoreInit()
ToolsInit()
MinilineInit()
PluginsInit()

CoreBuild()
ToolsBuild()
MinilineBuild()
PluginsBuild()

set( msg "\nElypse following projects will be built" )
CoreSummary( ${msg} )
ToolsSummary( ${msg} )
MinilineSummary( ${msg} )
PluginsSummary( ${msg} )
message( STATUS ${msg} )

install(
	TARGETS GeneralLib ElypseCore ${OPTIONAL_PROJECTS}
	EXPORT Elypse
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
install( EXPORT Elypse
	FILE ElypseConfig.cmake
	NAMESPACE elypse::
	DESTINATION share/Elypse/cmake
)
include(CMakePackageConfigHelpers)
write_basic_package_version_file( ElypseConfigVersion.cmake
	VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_BUILD}
	COMPATIBILITY AnyNewerVersion
)

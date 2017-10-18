project( GeneralLib )

set( ${PROJECT_NAME}_VERSION_MAJOR	${VERSION_MAJOR} )
set( ${PROJECT_NAME}_VERSION_MINOR	${VERSION_MINOR} )
set( ${PROJECT_NAME}_VERSION_BUILD	${VERSION_BUILD} )

include_directories( ${CMAKE_SOURCE_DIR}/Core/GeneralLib/Src )

add_target(
	${PROJECT_NAME}
	lib
	""
	"${BoostLibraries};${LibZipLibraries}"
)

file( GLOB ${PROJECT_NAME}_Theory_HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Singleton*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Singleton*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Named*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Named*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*NonCopyable*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*NonCopyable*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*OwnedBy*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*OwnedBy*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Reference*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Reference*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Pool*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Pool*.inl
)
file( GLOB ${PROJECT_NAME}_Theory_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Singleton*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Named*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*NonCopyable*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*OwnedBy*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Reference*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Pool*.cpp
)
source_group( "Header Files\\Theory" FILES ${${PROJECT_NAME}_Theory_HEADER_FILES} )
source_group( "Source Files\\Theory" FILES ${${PROJECT_NAME}_Theory_SOURCE_FILES} )

file( GLOB ${PROJECT_NAME}_Math_HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Bezier*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Bezier*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Factorial*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Factorial*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Math*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Math*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Pascal*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Pascal*.inl
)
file( GLOB ${PROJECT_NAME}_Math_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Bezier*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Factorial*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Math*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Pascal*.cpp
)
source_group( "Header Files\\Math" FILES ${${PROJECT_NAME}_Math_HEADER_FILES} )
source_group( "Source Files\\Math" FILES ${${PROJECT_NAME}_Math_SOURCE_FILES} )

file( GLOB ${PROJECT_NAME}_MultiThreading_HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Condition*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Condition*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Thread*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Thread*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Mutex*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Mutex*.inl
)
file( GLOB ${PROJECT_NAME}_MultiThreading_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Condition*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Thread*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Mutex*.cpp
)
source_group( "Header Files\\MultiThreading" FILES ${${PROJECT_NAME}_MultiThreading_HEADER_FILES} )
source_group( "Source Files\\MultiThreading" FILES ${${PROJECT_NAME}_MultiThreading_SOURCE_FILES} )

file( GLOB ${PROJECT_NAME}_Computer_HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Info*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Info*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*FileVersion*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*FileVersion*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Registry*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Registry*.inl
)
file( GLOB ${PROJECT_NAME}_Computer_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Info*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*FileVersion*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Registry*.cpp
)
source_group( "Header Files\\Computer" FILES ${${PROJECT_NAME}_Computer_HEADER_FILES} )
source_group( "Source Files\\Computer" FILES ${${PROJECT_NAME}_Computer_SOURCE_FILES} )

file( GLOB ${PROJECT_NAME}_Templates_HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Buffer*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Buffer*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*LocklessQueue*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*LocklessQueue*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Manager*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Manager*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Memory*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Memory*.inl
)
file( GLOB ${PROJECT_NAME}_Templates_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Buffer*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*LocklessQueue*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Manager*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Memory*.cpp
)
source_group( "Header Files\\Templates" FILES ${${PROJECT_NAME}_Templates_HEADER_FILES} )
source_group( "Source Files\\Templates" FILES ${${PROJECT_NAME}_Templates_SOURCE_FILES} )

file( GLOB ${PROJECT_NAME}_Files_HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*File*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*File*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*NeoCurl*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*NeoCurl*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Zip*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Zip*.inl
)
file( GLOB ${PROJECT_NAME}_Files_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*File*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*NeoCurl*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Zip*.cpp
)
source_group( "Header Files\\Files" FILES ${${PROJECT_NAME}_Files_HEADER_FILES} )
source_group( "Source Files\\Files" FILES ${${PROJECT_NAME}_Files_SOURCE_FILES} )

file( GLOB ${PROJECT_NAME}_Network_HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Tcp*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Tcp*.inl
)
file( GLOB ${PROJECT_NAME}_Network_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Tcp*.cpp
)
source_group( "Header Files\\Network" FILES ${${PROJECT_NAME}_Network_HEADER_FILES} )
source_group( "Source Files\\Network" FILES ${${PROJECT_NAME}_Network_SOURCE_FILES} )

file( GLOB ${PROJECT_NAME}_Utils_HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Assertion*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Assertion*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Library*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Library*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Exception*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Exception*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Execute*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Execute*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Tracker*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Tracker*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*MinMax*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*MinMax*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Path*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Path*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*PreciseTimer*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*PreciseTimer*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/STL*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/STL*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/String*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/String*.inl
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Url*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Url*.inl
)
file( GLOB ${PROJECT_NAME}_Utils_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Assertion*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Library*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Exception*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Execute*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Tracker*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*MinMax*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Path*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*PreciseTimer*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/STL*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/String*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Src/*Url*.cpp
)
source_group( "Header Files\\Utils" FILES ${${PROJECT_NAME}_Utils_HEADER_FILES} )
source_group( "Source Files\\Utils" FILES ${${PROJECT_NAME}_Utils_SOURCE_FILES} )

set_property( TARGET ${PROJECT_NAME} PROPERTY FOLDER "Core" )
set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )
add_target_astyle( ${PROJECT_NAME} ".h;.hpp;.inl;.cpp" )

copy_dll( ${PROJECT_NAME} ${ZLIB_LIBRARY} Debug )
copy_dll( ${PROJECT_NAME} ${ZLIB_LIBRARY} Release )
copy_dll( ${PROJECT_NAME} ${LIBZIP_LIBRARY} Debug )
copy_dll( ${PROJECT_NAME} ${LIBZIP_LIBRARY} Release )
copy_dll( ${PROJECT_NAME} ${CURL_LIBRARY} Debug )
copy_dll( ${PROJECT_NAME} ${CURL_LIBRARY} Release )
if ( WIN32 )
	get_filename_component( _Path ${FREETYPE_LIBRARY} PATH )
	get_filename_component( _Path ${_Path} PATH )
	copy_dll( ${PROJECT_NAME} ${_Path}/bin/freetype6.dll Debug )
	copy_dll( ${PROJECT_NAME} ${_Path}/bin/freetype6.dll Release )
endif ()

set( Build "yes (version ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_BUILD})" PARENT_SCOPE )

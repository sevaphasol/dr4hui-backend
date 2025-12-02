function( setup_constants )
	set( BACKEND_INCLUDE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/backend/include   PARENT_SCOPE )
	set( BACKEND_SOURCE_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/backend/source    PARENT_SCOPE )
	set( EXAMPLES_DIR         ${CMAKE_CURRENT_SOURCE_DIR}/examples          PARENT_SCOPE )
endfunction()

#==============================================================================

function( setup_interface )
	add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/interface )
endfunction()

#==============================================================================

function( setup_plugin_dr4 )
	file( GLOB_RECURSE DR4_BACKEND_SOURCES CONFIGURE_DEPENDS
		${BACKEND_SOURCE_DIR}/dr4/*.cpp
	)
	add_library( plugin_dr4 SHARED ${DR4_BACKEND_SOURCES} )
	target_include_directories( plugin_dr4 PRIVATE ${BACKEND_INCLUDE_DIR} )

	find_package( PkgConfig REQUIRED )
	pkg_check_modules( SFML REQUIRED IMPORTED_TARGET
		sfml-graphics sfml-window sfml-system
	)
	target_link_libraries( plugin_dr4 PRIVATE dr4 PkgConfig::SFML )
endfunction()

#==============================================================================

function( setup_plugin_pp )
	file( GLOB_RECURSE PP_BACKEND_SOURCES CONFIGURE_DEPENDS
		${BACKEND_SOURCE_DIR}/pp/**/*.cpp
		${BACKEND_SOURCE_DIR}/pp/*.cpp
	)
	add_library( plugin_pp SHARED ${PP_BACKEND_SOURCES} )
	target_include_directories( plugin_pp PRIVATE ${BACKEND_INCLUDE_DIR} )
	target_include_directories( plugin_pp PRIVATE ${PROJECT_SOURCE_DIR}/interface/include )

	target_link_libraries(plugin_pp PRIVATE dr4 )
endfunction()

#==============================================================================

function( setup_plugins )
	setup_plugin_dr4()
	setup_plugin_pp()
endfunction()

#==============================================================================

function( install_plugins )
	install( TARGETS plugin_dr4 DESTINATION plugins )
	install( TARGETS plugin_pp  DESTINATION plugins )
endfunction()

#==============================================================================

function( install_core )
	install( TARGETS cum DESTINATION core )
	install( TARGETS hui DESTINATION core )
endfunction()

#==============================================================================

function( build_examples )
	file( GLOB_RECURSE EXAMPLE_SOURCES CONFIGURE_DEPENDS
		${EXAMPLES_DIR}/*.cpp
	)
	add_executable( example ${EXAMPLE_SOURCES} )
	target_link_libraries( example dr4 )
	install( TARGETS example DESTINATION examples )
endfunction()

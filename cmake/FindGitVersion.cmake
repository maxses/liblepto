#-----------------------------------------------------------------------------
#
# \brief		CMake module to create an git version information header file
#
#				Example of usage:
#
#					[...]
#
#					find_package(GitVersion REQUIRED)
#
#					add_executable( ${PROJECT_NAME}
#								src/main.cpp
#								${GIT_VERSION_HEADER}
#					)
#
#					[...]
#           If the cmake-cariable "BUILD_ID" is set, it is set as C/C++ 
#           definition/macro. 
#           This can be used by build/servers/release-scripts.
#           Applications can send the build-id it via UART/CAN.
#           It is needed to find the matching elf file for debugging an
#           flashed firmware. Just the same firmware-version is not sufficient.
#
# \author	Maximilian Seesslen <mes@seesslen.net>
#
#-----------------------------------------------------------------------------


set ( GIT_VERSION_HEADER  "${CMAKE_CURRENT_BINARY_DIR}/git_version/git_version.h" )
set ( GIT_VERSION_DIRECTORY  "${CMAKE_CURRENT_BINARY_DIR}/git_version" )

get_filename_component(
   GV_PROJECT_NAME_UNIQUE
   ${CMAKE_CURRENT_BINARY_DIR}
   NAME
)

set ( GV_PROJECT_NAME_UNIQUE ${PROJECT_NAME}_${GV_PROJECT_NAME_UNIQUE} )

set ( GV_PROJECT_NAME ${PROJECT_NAME} )

file( MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/git_version" )

# to verify this: 
#    make git_creater_canbuz_canbuz
#    stat -c "%Y" canbuz/git_version/git_version.h
#    sleep 2
#    make git_creater_canbuz_canbuz
#    stat -c "%Y" canbuz/git_version/git_version.h
add_custom_target( git_creater_${GV_PROJECT_NAME_UNIQUE}
    COMMAND
      SOURCE_DIR=${CMAKE_SOURCE_DIR} 
      OUTPUT_DIR=${CMAKE_CURRENT_BINARY_DIR}/git_version 
      PROJECT=${GV_PROJECT_NAME} 
         cmake -P ${CMAKE_CURRENT_LIST_DIR}/git_version_script.cmake
)

add_custom_command(
    OUTPUT
         ${GIT_VERSION_HEADER}
    DEPENDS
         DEPENDS git_creater_${GV_PROJECT_NAME_UNIQUE}
)

set_source_files_properties(
   ${GIT_VERSION_HEADER}
   PROPERTIES GENERATED TRUE
)

install(
   FILES
      ${CMAKE_CURRENT_BINARY_DIR}/git_version/versions_${GV_PROJECT_NAME}.txt
   DESTINATION
      share/doc
   OPTIONAL
)


# BUILD_ID might be set by build scripts
if ( BUILD_ID )
   add_definitions( -DBUILD_ID=${BUILD_ID} )
endif ( BUILD_ID )

include_directories ( "${CMAKE_CURRENT_BINARY_DIR}/git_version" )


#---fin.----------------------------------------------------------------------

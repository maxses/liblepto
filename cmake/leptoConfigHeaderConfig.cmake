#------------------------------------------------------------------------------
#
# @brief	CMake package for handling configuration headers in campo projects
#
#    An configuration header can be specified via cmake variable 
#    COMMON_CONFIG_HEADER. 
#    This header will not be included directly but another configuration header
#    Will be generated with project specific defines only.
#    This generated header could then be installed into a sysroot.
#
#    There is also a checksum included in the header as define which can be 
#    used verified that used headers match used library binary.
#
#    In the CMakeLists.txt, "add_generated_config( <target> )" has to be added 
#    after declaring the target.
#
#    In source code, the define <PROJECT>_GENERATED_CONFIG has to be used to 
#    check weather the generated header has to be included or the one from the 
#    sysroot.
#
# @author     Maximilian Seesslen <src@seesslen.net>
# @copyright  SPDX-License-Identifier: Apache-2.0
#
#------------------------------------------------------------------------------


function( add_generated_config project )
   
   # It's not guaranteed that the target is equal to ${PROJECT_NAME}
   # set( project ${PROJECT_NAME} )
   
   string(TOUPPER ${project} PROJECT )
   
   get_property(
      TARGET_DEFS
      TARGET ${project}
      PROPERTY COMPILE_DEFINITIONS
   )

   get_target_property(
      INTERFACE_DEFS
      ${project}
      INTERFACE_COMPILE_DEFINITIONS
   )

   get_directory_property(
      DIRECTORY_DEFS
      COMPILE_DEFINITIONS
   )

   list( APPEND DEFS  ${DIRECTORY_DEFS} ${TARGET_DEFS} ${INTERFACE_DEFS} )
   list( TRANSFORM DEFS PREPEND "-D" )
   
   if ( COMMON_CONFIG_HEADER )
      set ( ${PROJECT}_CONFIG_HEADER "${COMMON_CONFIG_HEADER${VARIANT_POSTFIX_UPPER_CASE}}" PARENT_SCOPE)
      set ( ${PROJECT}_CONFIG_HEADER "${COMMON_CONFIG_HEADER${VARIANT_POSTFIX_UPPER_CASE}}")
      if( NOT COMMON_CONFIG_HEADER${VARIANT_POSTFIX_UPPER_CASE} )
         message( WARNING "A variant '${VARIANT_POSTFIX}' is used but the common header is not defined for it.")
         message( WARNING " Please set COMMON_CONFIG_HEADER${VARIANT_POSTFIX_UPPER_CASE} in top level cmake file." ) 
         message( WARNING " I will use COMMON_CONFIG_HEADER instead." ) 
         set ( ${PROJECT}_CONFIG_HEADER "${COMMON_CONFIG_HEADER}" PARENT_SCOPE)
         set ( ${PROJECT}_CONFIG_HEADER "${COMMON_CONFIG_HEADER}")
      endif()
   elseif( NOT ${PROJECT}_CONFIG_HEADER )
      set ( ${PROJECT}_CONFIG_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/${project}/presets/config_full.h" PARENT_SCOPE)
      set ( ${PROJECT}_CONFIG_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/${project}/presets/config_full.h")
   endif()
   
   set( header ${${PROJECT}_CONFIG_HEADER} )

   if( NOT ${PROJECT}_CONFIG_HEADER )
      message( FATAL_ERROR "Config header could not be evaluated." )
   endif()
   
   set( GENERATED_HEADER ${CMAKE_CURRENT_BINARY_DIR}/${project}/config_generated_local.h )

   add_custom_command(
      OUTPUT ${GENERATED_HEADER}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      COMMAND
         mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/${project}
      COMMAND
         echo "// Generated configuration header. Do not change!" > ${GENERATED_HEADER}
      COMMAND
         ${CMAKE_CXX_COMPILER} -include ${header} ${DEFS}
            -dM -E - < /dev/null >> ${CMAKE_CURRENT_BINARY_DIR}/config_${project}_all.h
      COMMAND
         ${CMAKE_CXX_COMPILER} -include ${header} ${DEFS}
            -dM -E - < /dev/null | grep CONFIG_${PROJECT} >> ${GENERATED_HEADER}
      COMMAND
         echo -n "\\#define ${PROJECT}_CONFIG_CHECKSUM 0x" >> ${GENERATED_HEADER}
      COMMAND
         sha256sum ${GENERATED_HEADER} | cut -c1-8 >> ${GENERATED_HEADER}
      COMMAND
         echo  "\\#define ${project}_CONFIG_CHECKSUM ${PROJECT}_CONFIG_CHECKSUM" >> ${GENERATED_HEADER}
      COMMAND
         echo -n "\\#define ${PROJECT}_CODE_SHA 0x" >> ${GENERATED_HEADER}
      COMMAND
         git --git-dir=${CMAKE_CURRENT_SOURCE_DIR}/.git rev-parse HEAD | head -c8  >> ${GENERATED_HEADER}
      COMMAND
         echo >> ${GENERATED_HEADER}
      COMMAND
         echo  "\\#define ${project}_CODE_SHA ${PROJECT}_CODE_SHA" >> ${GENERATED_HEADER}
      COMMAND
         echo  "\\#define ${PROJECT}_CONFIGURED 1" >> ${GENERATED_HEADER}
      COMMAND
         echo >> ${GENERATED_HEADER}
      COMMAND
         echo "//--- Fin ---------------------------" >> ${GENERATED_HEADER}
      DEPENDS ${header}
   )

   add_custom_target(
      generate_config_${project}
      DEPENDS ${GENERATED_HEADER}
   )

# endfunction()

# function( use_generated_config project )

   # string(TOUPPER ${project} PROJECT )

   target_compile_definitions(
      ${project}
      PUBLIC
         -D${PROJECT}_GENERATED_CONFIG
   )

   target_include_directories(
      ${project}
      PUBLIC
         ${CMAKE_CURRENT_BINARY_DIR}
   )

   add_dependencies( ${project} generate_config_${project} )

endfunction()


#------------------------------------------------------------------------------

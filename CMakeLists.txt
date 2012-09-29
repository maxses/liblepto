#------------------------------------------------------------------------------
#
# \brief	CMakeLists.txt file for "liblepto"
#
#------------------------------------------------------------------------------


cmake_minimum_required(VERSION 3.11)
project(lepto LANGUAGES CXX)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

if( ( "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64" ) OR HOST )
   set(CMAKE_AUTOUIC ON)
   set(CMAKE_AUTOMOC ON)
   set(CMAKE_AUTORCC ON)
   set( QT_COMPONENTS Core Widgets Network Xml )

   # Check either Qt6 or Qt5
   find_package( QT NAMES Qt5 Qt6 COMPONENTS ${QT_COMPONENTS} REQUIRED )
   find_package( Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QT_COMPONENTS} )

   #add_definitions( "-fPIC" )
   set( MCU_PLATFORM "linux" )
endif()

if( PROJECT_IS_TOP_LEVEL )
   # Let the tests work out of box
   add_definitions(
      -DCONFIG_LEPTO_RING_SUPPORT_VOLATILE=1
      -DLEPTO_CONFIGURED
   )
endif()

set( USE_LEPTO 1 PARENT_SCOPE )

set(
   headers
      include/lepto/ansi.h
      include/lepto/lepto.h
      include/lepto/log.h
      include/lepto/ring.hpp
      include/lepto/list.hpp
      include/lepto/string.hpp
      include/lepto/crc32.h
      include/lepto/crc8.h
      include/lepto/print.h
      include/lepto/bufferRing.hpp
)

set(
   sources
      src/lepto.cpp
      src/log.cpp
      src/ring.cpp
      src/string.cpp
      src/signal.cpp
      src/crc32.cpp
      src/crc8.cpp
      src/base64.cpp
      src/print.cpp
)

add_library(
   ${PROJECT_NAME}
   STATIC
      ${sources}
      ${headers}
      ${lepto_HEADERS_MOC}
)

target_compile_definitions(
   ${PROJECT_NAME}
   PUBLIC
      -DUSE_LEPTO
)

if ( COMMON_CONFIG_HEADER )
   set ( LEPTO_CONFIG_HEADER "${COMMON_CONFIG_HEADER}" )
elseif( EXISTS "${CMAKE_SYSROOT}/usr/include/lepto/config.h" )
   set ( LEPTO_CONFIG_HEADER "${CMAKE_SYSROOT}/usr/include/lepto/config.h" )
endif()

if( NOT ${COMMON_CONFIG_HEADER_INCLUDED} )
   if( LEPTO_CONFIG_HEADER )
      get_filename_component( LEPTO_CONFIG_PATH ${LEPTO_CONFIG_HEADER} DIRECTORY )
      get_filename_component( LEPTO_CONFIG_FILE ${LEPTO_CONFIG_HEADER} NAME )
   
      target_compile_options(
         ${PROJECT_NAME}
         PRIVATE
            "SHELL:-include ${LEPTO_CONFIG_HEADER}"
         PUBLIC
            "SHELL:-include ${LEPTO_CONFIG_HEADER}"
      )

      set_source_files_properties(
         ${sources}
         ${headers}
         PROPERTIES
         OBJECT_DEPENDS
         ${LEPTO_CONFIG_HEADER}
      )

      set( LEPTO_CONFIGURED Yes )
      # message( FATAL_ERROR "LEPTO_CONFIG_FILE: ${LEPTO_CONFIG_FILE}" )
   else()
      message( FATAL_ERROR "No config header for lepto defined. Either use "
               "common_config_header(), set"
               "'COMMON_CONFIG_HEADER' or 'LEPTO_CONFIG_HEADER' in your projects "
               "root cmake file. If you are sure that you don't need it, set "
               "'LEPTO_CONFIGURED' cmake variable")
   endif()
endif()


if( "${MCU_PLATFORM}" MATCHES "linux" )
   # There is an threaded list test that uses QThread
   target_link_libraries(
      ${PROJECT_NAME}
      Qt${QT_VERSION_MAJOR}::Core
   )
endif()

target_include_directories(
   ${PROJECT_NAME}
   PRIVATE
      include/
   INTERFACE
      include/
)

if( "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64" OR HOST )
    add_subdirectory( tests )
endif()


#------------------------------------------------------------------------------

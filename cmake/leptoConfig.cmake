#------------------------------------------------------------------------------
#
# \brief CMake variables for "liblepto"
#
#        It will provide a makro '__FILE_NAME__' when compiler does not 
#        already provide it.
#
#------------------------------------------------------------------------------


cmake_minimum_required(VERSION 3.11)

include(CheckCCompilerFlag)

if( CMAKE_CXX_COMPILER_ID MATCHES GNU )
   if ( ${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 12.1.0 )
      # Provide __FILE_NAME__ when not available. The magic here can only provide
      # the name of the compile unit. This could be confusing when using template
      # classes. Disable the feature but provide __FILE_NAME__ as 'unknown'.
      if( 0 )
         # Unfortunately, __FILE_NAME__ will contain the compile unit.
         if ( CMAKE_GENERATOR STREQUAL "Ninja" )
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__FILE_NAME__='\"$$(basename $$(notdir $$(abspath $$@)))\"'")
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__FILE_NAME__='\"$$(basename $$(notdir $$(abspath $$@)))\"'")
         else ( )
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__FILE_NAME__='\"$(basename $(notdir $(abspath $@)))\"'")
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__FILE_NAME__='\"$(basename $(notdir $(abspath $@)))\"'")
         endif ( )
      else( 0 )
         set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__FILE_NAME__='\"unknown\"'")
         set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__FILE_NAME__='\"unknown\"'")
      endif( 0 )
   endif ( ${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 12.1.0 )
endif( CMAKE_CXX_COMPILER_ID MATCHES GNU )

# Dont screw on the configurations.
if ( 0 )
   if( NOT HOST )
      if ( NOT MCU_FLASH_SIZE )
         message( WARNING "MCU_FLASH_SIZE not specified in generated cmake file. Check your arena board files" )
      endif ( NOT MCU_FLASH_SIZE )
   endif( NOT HOST )

   if( MCU_FLASH_SIZE GREATER_EQUAL 32768 ) # 32768; 65536
      add_definitions ( -DLEPTO_LOG_USE_PRETTY_PRINT )
   endif ( )

   if( HOST )
      add_definitions ( -DLEPTO_LOG_USE_PRETTY_PRINT )
   endif ( )
endif()


#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
#
# \brief CMake variables for "liblepto"
#
#        It will provide a makro '__FILE_NAME__' when compiler does not 
#        already provide it.
#
#------------------------------------------------------------------------------


file( READ "${COMMON_CONFIG_HEADER}" CONFIG_STRING )
string( SHA256 HASH "${CONFIG_STRING}" )
string( SUBSTRING "${HASH}" 0 16 CONFIG_HASH )

add_definitions( -DCONFIG_HASH=${CONFIG_HASH} )

#------------------------------------------------------------------------------

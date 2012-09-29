
# Compilation step for static resources
FUNCTION( ADD_RESOURCES out_var )

  SET( out_files )

  FOREACH( src_file ${ARGN} )
    SET( out_file "${PROJECT_BINARY_DIR}/${src_file}.o" )
	 get_filename_component(out_dir ${out_file} DIRECTORY)
	file(MAKE_DIRECTORY ${out_dir})
    ADD_CUSTOM_COMMAND(OUTPUT ${out_file}
		COMMAND ${CMAKE_LINKER} -r -b binary -o ${out_file} ${src_file}
      DEPENDS ${src_file}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMENT "Building resource object ${out_file}"
      VERBATIM
      )
    LIST(APPEND out_files ${out_file})
  ENDFOREACH()

  SET(${out_var} "${out_files}" PARENT_SCOPE)

ENDFUNCTION()


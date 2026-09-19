
# Not tested at all; only a example how Kconf could work

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/autoconf.h
    COMMAND ${CMAKE_SOURCE_DIR}/scripts/config2h.sh
            < ${CMAKE_SOURCE_DIR}/.config
            > ${CMAKE_BINARY_DIR}/autoconf.h
    DEPENDS ${CMAKE_SOURCE_DIR}/.config
)

add_custom_target(
    autoconf DEPENDS ${CMAKE_BINARY_DIR}/autoconf.h
)

add_dependencies( ${PROJECT_NAME} autoconf )

# target_include_directories( ${PROJECT_NAME} PRIVATE
#    ${CMAKE_BINARY_DIR}
# )


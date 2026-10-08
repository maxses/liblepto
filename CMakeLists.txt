#------------------------------------------------------------------------------
#
# \brief    CMakeLists.txt file for "liblepto"
#
#------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.10.2)

project(lepto LANGUAGES CXX VERSION 1.0.0)

include(GNUInstallDirs)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GenerateExportHeader)

option(LEPTO_BUILD_TESTS "Build liblepto test suite" ON)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64|amd64" OR HOST)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(QT_COMPONENTS Core Network Xml)

    find_package(QT NAMES Qt5 Qt6 COMPONENTS ${QT_COMPONENTS} REQUIRED)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${QT_COMPONENTS})
    set(MCU_PLATFORM "linux")
endif()

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    add_definitions(
        -DCONFIG_LEPTO_RING_SUPPORT_VOLATILE=1
        -DLEPTO_CONFIGURED
    )
endif()

if(NOT CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(USE_LEPTO 1 PARENT_SCOPE)
endif()

find_package(leptoConfigHeader REQUIRED)

set(headers
    include/lepto/ansi.h
    include/lepto/base64.h
    include/lepto/lepto.h
    include/lepto/log.h
    include/lepto/logPrinter.h
    include/lepto/ring.hpp
    include/lepto/list.hpp
    include/lepto/string.hpp
    include/lepto/crc32.h
    include/lepto/crc8.h
    include/lepto/print.h
    include/lepto/bufferRing.hpp
    include/lepto/eventLoop.hpp
    include/lepto/tuple.hpp
    include/lepto/signal.hpp
    include/lepto/signalDeferred.hpp
    include/lepto/signalPool.hpp
    include/lepto/signalPoolStatic.hpp
    ${COMMON_CONFIG_HEADER}
    config_generated_lepto.h
)

set(sources
    src/lepto.cpp
    src/log.cpp
    src/logPrinter.cpp
    src/ring.cpp
    src/string.cpp
    src/signal.cpp
    src/crc32.cpp
    src/crc8.cpp
    src/base64.cpp
    src/print.cpp
    src/eventLoop.cpp
)

add_library(
    ${PROJECT_NAME}
    STATIC
        ${sources}
        ${headers}
        ${lepto_HEADERS_MOC}
)

add_generated_config(lepto)

target_compile_definitions(
    ${PROJECT_NAME}
    PUBLIC
        -DUSE_LEPTO
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_property(TARGET ${PROJECT_NAME} PROPERTY VERSION 1)
set_property(TARGET ${PROJECT_NAME} PROPERTY SOVERSION 1)
set_property(TARGET ${PROJECT_NAME} PROPERTY INTERFACE_${PROJECT_NAME}_MAJOR_VERSION 3)
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY COMPATIBLE_INTERFACE_STRING ${PROJECT_NAME}_MAJOR_VERSION)

if("${MCU_PLATFORM}" MATCHES "linux")
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core)
endif()

generate_export_header(${PROJECT_NAME})

install(
    TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    DIRECTORY
        include/lepto
    DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT Devel
)

install(
    FILES
        LICENSE
    DESTINATION
        ${CMAKE_INSTALL_DOCDIR}
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/config_generated_lepto.h
    DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}/lepto
    RENAME
        config.h
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

export(EXPORT ${PROJECT_NAME}Targets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Targets.cmake"
    NAMESPACE Upstream::
)

configure_file(
    cmake/${PROJECT_NAME}Config.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Config.cmake"
    COPYONLY
)

install(
    FILES
        cmake/${PROJECT_NAME}Config.cmake
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    COMPONENT Devel
)

install(
    FILES
        cmake/leptoGitVersionConfig.cmake
        cmake/git_version_script.cmake
        cmake/git_version_script.sh
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/leptoGitVersion
    COMPONENT Devel
)

install(
    FILES
        cmake/git_version_script.sh
    PERMISSIONS
        OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
        OWNER_READ GROUP_READ WORLD_READ
        OWNER_WRITE
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/leptoGitVersion
    COMPONENT Devel
)

install(
    FILES
        cmake/leptoConfigHeaderConfig.cmake
    DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/leptoConfigHeader
    COMPONENT Devel
)

if(LEPTO_BUILD_TESTS AND (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64|amd64" OR HOST))
    add_subdirectory(tests)
endif()

#------------------------------------------------------------------------------

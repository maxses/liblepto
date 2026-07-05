#------------------------------------------------------------------------------
#
# \brief	CMakeLists.txt file for "libbiwak"
#
#------------------------------------------------------------------------------


cmake_minimum_required(VERSION 3.10.2)
project(lepto_tests)

find_package(Catch2 3 QUIET)
if (Catch2_FOUND)
    add_definitions( "-DCATCH_V3" )
    set(CATCH2_TARGET Catch2::Catch2WithMain)
else()
    find_package(Catch2 2 REQUIRED)
    add_definitions( "-DCATCH_V2" )
    set(CATCH2_TARGET Catch2::Catch2)
endif()

set(
   headers

)

set(
   sources
      test_main.cpp
      test_ring.cpp
      test_crc8.cpp
      test_crc32.cpp
      test_list.cpp
      test_ring_threaded.cpp
      test_ring_threaded.hpp
      test_signal.cpp
      test_string.cpp
      test_base64.cpp
      test_log.cpp
      test_regConfig.cpp
)

add_executable(
   lepto_tests
      ${sources}
      ${headers}
)


target_link_libraries(
   lepto_tests
   PRIVATE
      lepto
      ${CATCH2_TARGET}
)


add_test(
   NAME lepto_tests
   COMMAND lepto_tests
   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)


#------------------------------------------------------------------------------

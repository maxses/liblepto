#-----------------------------------------------------------------------------
#
# \brief		helper cmake file to create git version information header
#
#				Use helper bash script to direct git version header to cmake
#				environment. The actual git version info header file is only created
#				if the content differs to the current content. So there are no
#				unneccessary rebuilds.
#
# \author	Maximilian Seesslen <mes@seesslen.net>
#
#-----------------------------------------------------------------------------


execute_process(
                      COMMAND ${CMAKE_CURRENT_LIST_DIR}/git_version_script.sh
                      WORKING_DIRECTORY $ENV{SOURCE_DIR}
                      OUTPUT_VARIABLE GIT_ALL
                 )

execute_process(
                     COMMAND ${CMAKE_CURRENT_LIST_DIR}/git_version_script.sh -t
                     WORKING_DIRECTORY $ENV{SOURCE_DIR}
                     OUTPUT_VARIABLE GIT_ALL_TEXT
                )

if(EXISTS $ENV{OUTPUT_DIR}/git_version.h)
	 file(READ $ENV{OUTPUT_DIR}/git_version.h VERSION_)
else()
    set(VERSION_ "")
endif()

if (NOT "${GIT_ALL}" STREQUAL "${VERSION_}")
    file(WRITE $ENV{OUTPUT_DIR}/git_version.h "${GIT_ALL}")
endif()

file(WRITE $ENV{OUTPUT_DIR}/versions_$ENV{PROJECT}.txt "${GIT_ALL_TEXT}")


#---fin-----------------------------------------------------------------------

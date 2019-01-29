# define the minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(houdini)

# 
SET(CMAKE_VERBOSE_MAKEFILE ON)

# use folder
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDER ON)

# messages
MESSAGE(STATUS "plugin build begin")

# set run time path
SET(PLUGIN_ROOT_PATH "${PROJECT_SOURCE_DIR}")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_ROOT_PATH}/lib")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PLUGIN_ROOT_PATH}/lib")

SET(ECHO_ROOT_PATH CACHE PATH "${ECHO_ROOT_PATH}" FORCE)
IF(${ECHO_ROOT_PATH})
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${ECHO_ROOT_PATH}/bin/editor/win64")
ELSE()
	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PLUGIN_ROOT_PATH}/bin")
ENDIF()

SET(CMAKE_CXX_STANDARD 11)

IF(WIN32)
	
ELSE()
	MESSAGE(STATUS "For now, I only test on windows platform, Don't have to time test for mac linux ios android")
ENDIF()

# auto group files
MACRO(GROUP_FILES src_files root_path)
    SET(cur_dir ${root_path})
    FOREACH(group_files ${${src_files}})
        STRING(REGEX REPLACE ${cur_dir}/\(.*\) \\1 sgbd_fpath ${group_files})
        STRING(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
        STRING(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
        STRING(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
        IF(sgbd_nogroup)
			SOURCE_GROUP("" FILES ${group_files})   
		ELSE()
			INCLUDE_DIRECTORIES(${sgbd_group_name})
			SOURCE_GROUP(${sgbd_group_name} FILES ${group_files})   
           ENDIF(sgbd_nogroup) 
    ENDFOREACH(group_files)
ENDMACRO(GROUP_FILES)

ADD_SUBDIRECTORY("src")


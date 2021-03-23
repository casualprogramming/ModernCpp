cmake_minimum_required(VERSION 3.13.2)
set(CMAKE_CXX_STANDARD 20)
project(Programmers)

#function of get sub directory list
MACRO(SUBDIRLIST result curdir)
    file(GLOB childs "${curdir}/*")
    SET(dirlist "")
    FOREACH(child ${childs})
        IF(IS_DIRECTORY "${child}")
        get_filename_component(curdir_abs ${curdir} ABSOLUTE)
        file(RELATIVE_PATH subdirName ${curdir_abs} "${child}")
        LIST(APPEND dirlist ${subdirName})
        ENDIF()
    ENDFOREACH()
    SET(${result} ${dirlist})
ENDMACRO()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/mybin/)
set(MY_SOURCE "src")
SUBDIRLIST(SUBDIRS ${MY_SOURCE})

FOREACH(subdir ${SUBDIRS})
    file(GLOB_RECURSE SOURCE_FILE_LIST "${MY_SOURCE}/${subdir}/*.c" "${MY_SOURCE}/${subdir}/*.cpp" "${MY_SOURCE}/${subdir}/*.h")
    add_executable("${subdir}" ${SOURCE_FILE_LIST})   
ENDFOREACH()


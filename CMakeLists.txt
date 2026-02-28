cmake_minimum_required(VERSION 3.16)
project(CLearning C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB CHILDREN RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)
foreach(child ${CHILDREN})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${child}
       AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${child}/CMakeLists.txt)
    )
    add_subdirectory(${child})
    endif()
endforeach()

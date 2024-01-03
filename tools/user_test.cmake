#!/usr/bin/env -S cmake -P

cmake_path(SET CURSES_CPP_ROOT NORMALIZE ${CMAKE_CURRENT_LIST_DIR}/..)
cmake_path(SET USER_TEST_DIR ${CURSES_CPP_ROOT}/user_test)

if(EXISTS ${USER_TEST_DIR})
    message(FATAL_ERROR "${USER_TEST_DIR} already exists.")
endif()

function(execute_command ARG_WORKING_DIRECTORY) # Put the command in ARGN
    execute_process(
        WORKING_DIRECTORY ${ARG_WORKING_DIRECTORY}
        COMMAND ${ARGN}
        RESULT_VARIABLE result
    )
    if(result)
        message(FATAL_ERROR "Failed to run ${ARGN}")
    endif()
endfunction()

execute_command(
    ${CURSES_CPP_ROOT}
    ${CMAKE_COMMAND} -E make_directory ${USER_TEST_DIR}
)
execute_command(
    ${USER_TEST_DIR}
    ${CMAKE_COMMAND} -S .. -B build -D CMAKE_BUILD_TYPE=Release
)
execute_command(
    ${USER_TEST_DIR}
    ${CMAKE_COMMAND} --build build --config Release -j 8
)
execute_command(
    ${USER_TEST_DIR}
    ${CMAKE_COMMAND} --install build --prefix install
)
execute_command(
    ${USER_TEST_DIR}
    ${CMAKE_COMMAND} -S ../examples -B examples -D CMAKE_BUILD_TYPE=Release -D CMAKE_PREFIX_PATH=${USER_TEST_DIR}/install
)
execute_command(
    ${USER_TEST_DIR}
    ${CMAKE_COMMAND} --build examples --config Release -j 8
)

message(STATUS "Success!")
message(STATUS "The examples are built in ${USER_TEST_DIR}/examples")

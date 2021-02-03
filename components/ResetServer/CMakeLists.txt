#
# Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
#
# SPDX-License-Identifier: BSD-2-Clause
#

cmake_minimum_required(VERSION 3.8.2)

project(ResetServer C)

CAmkESAddImportPath(camkes-include)
CAmkESAddCPPInclude(camkes-include)
CAmkESAddTemplatesPath(templates)

DeclareCAmkESConnector(
    ResetServerInit
    FROM
    ResetServer.template.c
    FROM_LIBS
    ResetServer-server
    TO
    empty.c
)

DeclareCAmkESConnector(
    ResetServerClientInit
    FROM
    ResetServer-client.template.c
    FROM_LIBS
    ResetServer-client
    TO
    empty.c
)

if("${KernelPlatform}" STREQUAL "tx2")
    set(ResetServerExtraLibs platsupportports)
endif()

CAmkESAddCPPInclude("${CMAKE_CURRENT_LIST_DIR}/include/plat/${KernelPlatform}/")

add_subdirectory(
    "${CMAKE_CURRENT_LIST_DIR}/libResetServer-client"
    ${CMAKE_CURRENT_BINARY_DIR}/libResetServer-client
)

add_library(ResetServer-server EXCLUDE_FROM_ALL src/reset.c)
target_link_libraries(ResetServer-server sel4camkes ${ResetServerExtraLibs})

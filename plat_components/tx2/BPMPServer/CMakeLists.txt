#
# Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
#
# SPDX-License-Identifier: BSD-2-Clause
#

cmake_minimum_required(VERSION 3.8.2)

project(BPMPServer C)

CAmkESAddImportPath(camkes-include)
CAmkESAddCPPInclude(camkes-include)
CAmkESAddTemplatesPath(templates)

set(CAmkESDTS ON CACHE BOOL "" FORCE)

DeclareCAmkESConnector(
    BPMPServerInit
    FROM
    BPMPServer.template.c
    FROM_LIBS
    BPMPServer-server
    TO
    empty.c
)

DeclareCAmkESConnector(
    BPMPServerClientInit
    FROM
    BPMPServer-client.template.c
    FROM_LIBS
    BPMPServer-client
    TO
    empty.c
)

add_subdirectory(
    "${CMAKE_CURRENT_LIST_DIR}/libBPMPServer-client"
    ${CMAKE_CURRENT_BINARY_DIR}/libBPMPServer-client
)

add_library(BPMPServer-server EXCLUDE_FROM_ALL src/server.c)
target_link_libraries(BPMPServer-server sel4camkes platsupportports)

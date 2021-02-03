#
# Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
#
# SPDX-License-Identifier: BSD-2-Clause
#

cmake_minimum_required(VERSION 3.8.2)

project(picotcp-socket-sync-package C)

CAmkESAddImportPath(camkes-include)
CAmkESAddCPPInclude(camkes-include)
CAmkESAddTemplatesPath(templates)

DeclareCAmkESConnector(
    PicotcpSocketSyncServerInit
    FROM
    picotcp-socket-sync-server-init.template.c
    FROM_LIBS
    picotcp-socket-sync-server
    TO
    empty.c
)

DeclareCAmkESConnector(
    PicotcpSocketSyncClientInit
    FROM
    empty.c
    FROM_LIBS
    PicoServer-client
    TO
    empty.c
)

add_library(picotcp-socket-sync INTERFACE)

target_include_directories(picotcp-socket-sync INTERFACE include)

add_library(PicoServer-client INTERFACE)
target_include_directories(PicoServer-client INTERFACE client_include)
target_link_libraries(PicoServer-client INTERFACE picotcp-socket-sync)

add_library(picotcp-socket-sync-server EXCLUDE_FROM_ALL server/server.c server/picoserver_client.c)
target_link_libraries(
    picotcp-socket-sync-server
    sel4camkes
    picotcp
    picotcp-socket-sync
    PicoServer-client
)

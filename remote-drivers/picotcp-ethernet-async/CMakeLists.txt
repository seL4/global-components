#
# Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
#
# SPDX-License-Identifier: BSD-2-Clause
#

cmake_minimum_required(VERSION 3.8.2)

project(picotcp-ethernet-async-package C)

CAmkESAddImportPath(camkes-include)
CAmkESAddCPPInclude(camkes-include)
CAmkESAddTemplatesPath(templates)

DeclareCAmkESConnector(
    PicotcpEthernetAsyncServerInit
    FROM
    picotcp-ethernet-async-server-init.template.c
    FROM_LIBS
    picotcp-ethernet-async-server
    TO
    empty.c
)

DeclareCAmkESConnector(
    PicotcpEthernetAsyncClientInit
    FROM
    picotcp-ethernet-async-client-init.template.c
    FROM_LIBS
    picotcp-ethernet-async-client
    TO
    empty.c
)

add_library(picotcp-ethernet-async INTERFACE)

target_include_directories(picotcp-ethernet-async INTERFACE include)

add_library(picotcp-ethernet-async-server EXCLUDE_FROM_ALL server.c)
target_link_libraries(picotcp-ethernet-async-server sel4camkes picotcp-ethernet-async ethdrivers)

add_library(picotcp-ethernet-async-client EXCLUDE_FROM_ALL client.c)
target_link_libraries(picotcp-ethernet-async-client sel4camkes picotcp-ethernet-async picotcp)

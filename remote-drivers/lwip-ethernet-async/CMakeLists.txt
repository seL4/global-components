#
# Copyright 2020, Data61
# Commonwealth Scientific and Industrial Research Organisation (CSIRO)
# ABN 41 687 119 230.
#
# This software may be distributed and modified according to the terms of
# the BSD 2-Clause license. Note that NO WARRANTY is provided.
# See "LICENSE_BSD2.txt" for details.
#
# @TAG(DATA61_BSD)
#

cmake_minimum_required(VERSION 3.8.2)

project(lwip-ethernet-async-package C)

CAmkESAddImportPath(camkes-include)
CAmkESAddCPPInclude(camkes-include)
CAmkESAddTemplatesPath(templates)

DeclareCAmkESConnector(
    LwipEthernetAsyncServerInit
    FROM
    lwip-ethernet-async-server-init.template.c
    FROM_LIBS
    lwip-ethernet-async-server
    TO
    empty.c
)

DeclareCAmkESConnector(
    LwipEthernetAsyncClientInit
    FROM
    lwip-ethernet-async-client-init.template.c
    FROM_LIBS
    lwip-ethernet-async-client
    TO
    empty.c
)

add_library(lwip-ethernet-async INTERFACE)
target_include_directories(lwip-ethernet-async INTERFACE include)

add_library(lwip-ethernet-async-server EXCLUDE_FROM_ALL server.c)
target_link_libraries(
    lwip-ethernet-async-server
    sel4camkes
    lwip-ethernet-async
    ethdrivers
    shared_ringbuffer
    LWIPServer-client
)

add_library(lwip-ethernet-async-client EXCLUDE_FROM_ALL client.c)
target_link_libraries(
    lwip-ethernet-async-client
    sel4camkes
    lwip-ethernet-async
    lwip
    shared_ringbuffer
    LWIPServer-client
)

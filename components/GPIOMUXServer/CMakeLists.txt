#
# Copyright 2019, Data61
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

project(GPIOMUXServer C)

CAmkESAddImportPath(camkes-include)
CAmkESAddCPPInclude(camkes-include)
CAmkESAddTemplatesPath(templates)

DeclareCAmkESConnector(
    GPIOServerInit
    FROM
    GPIOServer.template.c
    FROM_LIBS
    GPIOServer-server
    TO
    empty.c
)

DeclareCAmkESConnector(
    GPIOServerClientInit
    FROM
    GPIOServer-client.template.c
    FROM_LIBS
    GPIOMUXServer-client
    TO
    empty.c
)

DeclareCAmkESConnector(
    MUXServerInit
    FROM
    MUXServer.template.c
    FROM_LIBS
    MUXServer-server
    TO
    empty.c
)

DeclareCAmkESConnector(
    MUXServerClientInit
    FROM
    MUXServer-client.template.c
    FROM_LIBS
    GPIOMUXServer-client
    TO
    empty.c
)

if("${KernelPlatform}" STREQUAL "tx2")
    DeclareCAmkESComponent(GPIOMUXServer LIBS platsupportports)
endif()

CAmkESAddCPPInclude("${CMAKE_CURRENT_LIST_DIR}/include/plat/${KernelPlatform}/")

add_subdirectory(
    "${CMAKE_CURRENT_LIST_DIR}/libGPIOMUXServer-client"
    ${CMAKE_CURRENT_BINARY_DIR}/libGPIOMUXServer-client
)

add_library(GPIOServer-server EXCLUDE_FROM_ALL src/gpio.c)
target_link_libraries(GPIOServer-server sel4camkes GPIOMUXServer-client)

add_library(MUXServer-server EXCLUDE_FROM_ALL src/mux.c)
target_link_libraries(MUXServer-server sel4camkes)

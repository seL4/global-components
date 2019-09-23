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

set(CAmkESDTS ON CACHE BOOL "" FORCE)
set(CAmkESCPP ON CACHE BOOL "" FORCE)

if("${KernelPlatform}" STREQUAL "tx2")
    DeclareCAmkESComponent(
        GPIOMUXServer
        SOURCES
        src/gpio.c
        src/mux.c
        src/common.c
        LIBS
        GPIOMUXServer-client
        platsupportports
    )
else()
    DeclareCAmkESComponent(
        GPIOMUXServer
        SOURCES
        src/gpio.c
        src/mux.c
        src/common.c
        LIBS
        GPIOMUXServer-client
    )
endif()

CAmkESAddCPPInclude("${CMAKE_CURRENT_LIST_DIR}/include/plat/${KernelPlatform}/")

add_subdirectory(
    "${CMAKE_CURRENT_LIST_DIR}/libGPIOMUXServer-client"
    ${CMAKE_CURRENT_BINARY_DIR}/libGPIOMUXServer-client
)

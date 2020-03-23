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

project(ResetServer C)

set(CAmkESDTS ON CACHE BOOL "" FORCE)
set(CAmkESCPP ON CACHE BOOL "" FORCE)

# Check if platform specific source files exists
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/src/plat/${KernelPlatform}/plat.c")
    set(ResetServerPlatSources "src/plat/${KernelPlatform}/plat.c")
endif()

if("${KernelPlatform}" STREQUAL "tx2")
    set(ResetServerExtraLibs platsupportports BPMPServer-client)
endif()

DeclareCAmkESComponent(
    ResetServer
    SOURCES
    src/reset.c
    ${ResetServerPlatSources}
    LIBS
    ${ResetServerExtraLibs}
)

CAmkESAddCPPInclude("${CMAKE_CURRENT_LIST_DIR}/include/plat/${KernelPlatform}/")

add_subdirectory(
    "${CMAKE_CURRENT_LIST_DIR}/libResetServer-client"
    ${CMAKE_CURRENT_BINARY_DIR}/libResetServer-client
)

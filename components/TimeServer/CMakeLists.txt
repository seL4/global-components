#
# Copyright 2018, Data61
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

project(TimeServer C)

if(KernelArchARM)
    set(CAmkESDTS ON CACHE BOOL "" FORCE)
endif()
set(CAmkESCPP ON CACHE BOOL "" FORCE)

if("${KernelPlatform}" STREQUAL "pc99")
    list(APPEND TimeServerPlatSources src/plat/pc99/plat.c)
endif()

# The imx8 boards (imx8mm-evk, imx8mq-evk) share the same timer device configuration
if("${KernelPlatform}" MATCHES "imx8.*")
    set(PlatPrefix "imx8")
else()
    set(PlatPrefix "${KernelPlatform}")
endif()

DeclareCAmkESComponent(TimeServer SOURCES src/time_server.c ${TimeServerPlatSources})

CAmkESAddCPPInclude("${CMAKE_CURRENT_LIST_DIR}/include/plat/${PlatPrefix}/")

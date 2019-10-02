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

project(ClockServer C)

if(NOT KernelArchARM)
    message(FATAL_ERROR "The ClockServer component can only be used on ARM platforms!")
endif()

set(CAmkESDTS ON CACHE BOOL "" FORCE)
set(CAmkESCPP ON CACHE BOOL "" FORCE)

DeclareCAmkESComponent(ClockServer SOURCES src/clock_server.c)

CAmkESAddCPPInclude("${CMAKE_CURRENT_LIST_DIR}/include/plat/${KernelPlatform}/")

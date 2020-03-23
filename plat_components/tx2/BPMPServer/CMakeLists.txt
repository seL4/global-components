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

project(BPMPServer C)

set(CAmkESDTS ON CACHE BOOL "" FORCE)

DeclareCAmkESComponent(BPMPServer SOURCES src/bpmp.c LIBS platsupportports)

add_subdirectory(
    "${CMAKE_CURRENT_LIST_DIR}/libBPMPServer-client"
    ${CMAKE_CURRENT_BINARY_DIR}/libBPMPServer-client
)

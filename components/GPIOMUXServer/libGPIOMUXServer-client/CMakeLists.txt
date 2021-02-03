#
# Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
#
# SPDX-License-Identifier: BSD-2-Clause
#

cmake_minimum_required(VERSION 3.8.2)

project(libGPIOMUXServer-client C)

add_library(GPIOMUXServer-client EXCLUDE_FROM_ALL gpio.c mux.c)

target_include_directories(GPIOMUXServer-client PUBLIC include plat_include/${KernelPlatform})
if("${KernelPlatform}" STREQUAL "tx2")
    target_link_libraries(GPIOMUXServer-client platsupport platsupportports)
else()
    target_link_libraries(GPIOMUXServer-client platsupport)
endif()

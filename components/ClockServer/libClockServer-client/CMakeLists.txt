#
# Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
#
# SPDX-License-Identifier: BSD-2-Clause
#

cmake_minimum_required(VERSION 3.8.2)

project(libClockServer-client C)

add_library(ClockServer-client EXCLUDE_FROM_ALL clockserver_client.c)

target_include_directories(ClockServer-client PUBLIC include)
target_link_libraries(ClockServer-client muslc utils platsupport)

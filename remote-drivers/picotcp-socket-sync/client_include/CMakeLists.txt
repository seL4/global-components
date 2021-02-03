#
# Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
#
# SPDX-License-Identifier: BSD-2-Clause
#

cmake_minimum_required(VERSION 3.8.2)

project(libPicoServer-client C)

add_library(PicoServer-client INTERFACE)

target_include_directories(PicoServer-client INTERFACE ./)
target_link_libraries(PicoServer-client)

#
# Copyright 2018, Data61, CSIRO (ABN 41 687 119 230)
#
# SPDX-License-Identifier: BSD-2-Clause
#

cmake_minimum_required(VERSION 3.8.2)

project(libFileServer-client C)

add_library(FileServer-client EXCLUDE_FROM_ALL fsclient.c)

target_include_directories(FileServer-client PUBLIC .)
target_link_libraries(FileServer-client muslc utils sel4muslcsys)

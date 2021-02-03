#
# Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
#
# SPDX-License-Identifier: BSD-2-Clause
#

cmake_minimum_required(VERSION 3.8.2)

project(libBPMPServer-client C)

add_library(BPMPServer-client EXCLUDE_FROM_ALL bpmpserver_client.c)

target_include_directories(BPMPServer-client PUBLIC include)
target_link_libraries(BPMPServer-client muslc utils tx2bpmp)

#
# Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
#
# SPDX-License-Identifier: BSD-2-Clause
#

cmake_minimum_required(VERSION 3.8.2)

project(picotcp-base-package C)

CAmkESAddImportPath(camkes-include)
CAmkESAddCPPInclude(camkes-include)
CAmkESAddTemplatesPath(templates)

DeclareCAmkESConnector(PicotcpBaseInit FROM pico-init.template.c FROM_LIBS picotcp-init TO empty.c)

add_library(picotcp-init EXCLUDE_FROM_ALL picotcp-init.c)
target_link_libraries(picotcp-init sel4camkes picotcp)

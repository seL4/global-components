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

project(picotcp-base-package C)

CAmkESAddImportPath(camkes-include)
CAmkESAddCPPInclude(camkes-include)
CAmkESAddTemplatesPath(templates)

DeclareCAmkESConnector(PicotcpBaseInit FROM pico-init.template.c FROM_LIBS picotcp-init TO empty.c)

add_library(picotcp-init EXCLUDE_FROM_ALL picotcp-init.c)
target_link_libraries(picotcp-init sel4camkes picotcp)

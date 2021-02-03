#
# Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
#
# SPDX-License-Identifier: BSD-2-Clause
#

cmake_minimum_required(VERSION 3.8.2)

project(dynamic-untyped-allocators-package C)

CAmkESAddImportPath(camkes-include)
CAmkESAddCPPInclude(camkes-include)
CAmkESAddTemplatesPath(templates)

DeclareCAmkESConnector(
    DynamicUntypedAllocatorEnvironment
    FROM
    dynamic-untyped-allocators.template.c
    FROM_HEADER
    dynamic-untyped-allocators.template.h
    FROM_LIBS
    sel4allocman
    TO
    empty.c
)

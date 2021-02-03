#
# Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
#
# SPDX-License-Identifier: BSD-2-Clause
#

cmake_minimum_required(VERSION 3.8.2)

CAmkESAddImportPath(camkes-include)
CAmkESAddCPPInclude(camkes-include)

if(KernelArchARM)
    if(NOT KernelArmExportPMUUser)
        message("BenchUtiliz not available, as KernelArmExportPMUUser is OFF")
        return()
    endif()
elseif(KernelArchX86)
    if(NOT KernelExportPMCUser)
        message("BenchUtiliz not available, as KernelExportPMCUser is OFF")
        return()
    endif()
else()
    message("BenchUtiliz not available, as platform is unsupported")
    return()
endif()

project(BenchUtiliz C)

DeclareCAmkESComponent(BenchUtiliz SOURCES src/benchUtiliz.c LIBS sel4bench)

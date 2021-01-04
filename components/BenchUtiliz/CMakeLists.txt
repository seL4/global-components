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

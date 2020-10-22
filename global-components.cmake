#
# Copyright 2018, Data61
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

include("${CMAKE_CURRENT_LIST_DIR}/global-connectors.cmake")

CAmkESAddImportPath(components plat_components/${KernelPlatform})

foreach(
    comp
    IN
    ITEMS
    remote-drivers/picotcp-ethernet-async
    remote-drivers/picotcp-socket-sync
    components/modules/fdt-bind-driver
    components/modules/dynamic-untyped-allocators
    components/modules/single-threaded
    components/modules/x86-iospace-dma
    components/modules/picotcp-base
    components/ClockServer
    components/GPIOMUXServer
    components/ResetServer
    plat_components/tx2/BPMPServer
)
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/${comp}" ${comp})
endforeach()

foreach(
    comp
    IN
    ITEMS
    PCIConfigIO
    RTC
    SerialServer
    TimeServer
    FileServer
    VirtQueue
    BenchUtiliz
    Ethdriver
    PicoServer
)
    include("${CMAKE_CURRENT_LIST_DIR}/components/${comp}/CMakeLists.txt")
endforeach()

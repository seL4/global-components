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

CAmkESAddImportPath(components)
CAmkESAddImportPath(plat_components/${KernelPlatform})
CAmkESAddImportPath(interfaces)
CAmkESAddImportPath(plat_interfaces/${KernelPlatform})
CAmkESAddTemplatesPath(templates)

add_subdirectory(
    ${CMAKE_CURRENT_LIST_DIR}/remote-drivers/picotcp-ethernet-async/
    remote-drivers/picotcp-ethernet-async
)
add_subdirectory(
    ${CMAKE_CURRENT_LIST_DIR}/components/modules/fdt-bind-driver/
    components/modules/fdt-bind-driver
)
add_subdirectory(
    ${CMAKE_CURRENT_LIST_DIR}/components/modules/dynamic-untyped-allocators/
    components/modules/dynamic-untyped-allocators
)
add_subdirectory(
    ${CMAKE_CURRENT_LIST_DIR}/components/modules/single-threaded/
    components/modules/single-threaded
)
add_subdirectory(
    ${CMAKE_CURRENT_LIST_DIR}/components/modules/x86-iospace-dma/
    components/modules/x86-iospace-dma
)
add_subdirectory(
    ${CMAKE_CURRENT_LIST_DIR}/remote-drivers/picotcp-socket-sync/
    remote-drivers/picotcp-socket-sync
)
add_subdirectory(
    ${CMAKE_CURRENT_LIST_DIR}/components/modules/picotcp-base/ components/modules/picotcp-base
)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/components/ClockServer/ components/ClockServer)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/components/GPIOMUXServer/ components/GPIOMUXServer)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/components/ResetServer/ components/ResetServer)
add_subdirectory(
    ${CMAKE_CURRENT_LIST_DIR}/plat_components/tx2/BPMPServer/ plat_components/tx2/BPMPServer
)

include(${CMAKE_CURRENT_LIST_DIR}/components/PCIConfigIO/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/components/RTC/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/components/SerialServer/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/components/TimeServer/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/components/FileServer/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/components/VirtQueue/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/components/BenchUtiliz/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/components/Ethdriver/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/components/PicoServer/CMakeLists.txt)

foreach(
    connector
    IN
    ITEMS
    seL4RPCDataportSignal
    seL4GlobalAsynch
    seL4GlobalAsynchCallback
    seL4Ethdriver
    seL4MessageQueue
    seL4RPCOverMultiSharedData
)
    DeclareCAmkESConnector(
        ${connector}
        FROM
        ${connector}-from.template.c
        TO
        ${connector}-to.template.c
    )
endforeach()
DeclareCAmkESConnector(
    seL4SharedDataWithCaps
    FROM
    seL4SharedDataWithCaps.template.c
    TO
    seL4SharedDataWithCaps.template.c
)
DeclareCAmkESConnector(
    seL4TimeServer
    FROM
    seL4RPCCallSignal-from.template.c
    FROM_HEADER
    get-notification.template.h
    TO
    seL4RPCCallSignal-to.template.c
)
DeclareCAmkESConnector(
    seL4SerialServer
    FROM
    seL4RPCDataportSignal-from.template.c
    FROM_HEADER
    get-notification.template.h
    TO
    seL4RPCDataportSignal-to.template.c
)
DeclareCAmkESConnector(
    seL4RPCCallSignal
    FROM
    seL4RPCCallSignal-from.template.c
    TO
    seL4RPCCallSignal-to.template.c
    TO_HEADER
    seL4RPCCallSignal-to.template.h
)

DeclareCAmkESConnector(
    seL4RPCCallSignalNoThreads
    FROM
    seL4RPCCallSignal-from.template.c
    TO
    seL4RPCCallSignal-to.template.c
    TO_HEADER
    seL4RPCCallSignal-to.template.h
)

DeclareCAmkESConnector(
    seL4PicoServerSignal
    FROM
    seL4RPCCallSignal-from.template.c
    FROM_HEADER
    get-notification.template.h
    TO
    seL4RPCCallSignal-to.template.c
    TO_HEADER
    seL4RPCCallSignal-to.template.h
)
DeclareCAmkESConnector(
    seL4RPCDataport
    FROM
    seL4RPCDataport-from.template.c
    TO
    seL4RPCDataport-to.template.c
    TO_HEADER
    seL4RPCDataport-to.template.h
)

DeclareCAmkESConnector(
    seL4RPCDataportNoThreads
    FROM
    seL4RPCDataport-from.template.c
    TO
    seL4RPCDataport-to.template.c
    TO_HEADER
    seL4RPCDataport-to.template.h
)

DeclareCAmkESConnector(
    seL4PicoServer
    FROM
    seL4RPCDataport-from.template.c
    TO
    seL4RPCDataport-to.template.c
    TO_HEADER
    seL4RPCDataport-to.template.h
)

DeclareCAmkESConnector(
    seL4GlobalAsynchHardwareInterrupt TO seL4GlobalAsynchHardwareInterrupt.template.c
)

DeclareCAmkESConnector(
    seL4RPCNoThreads
    FROM
    seL4RPCNoThreads-from.template.c
    TO
    seL4RPCNoThreads-to.template.c
    TO_HEADER
    seL4RPCNoThreads-to.template.h
)

DeclareCAmkESConnector(
    seL4DTBHardwareThreadless
    FROM
    empty.c
    TO
    seL4DTBHardwareThreadless.template.c
)

DeclareCAmkESConnector(seL4DTBHWThreadless TO seL4DTBHardwareThreadless.template.c)

DeclareCAmkESConnector(
    seL4VirtQueues
    FROM
    seL4VirtQueues-from.template.c
    TO
    seL4VirtQueues-to.template.c
    FROM_HEADER
    seL4VirtQueues-from.template.h
)

DeclareCAmkESConnector(
    seL4GPIOServer
    FROM
    seL4GPIOServer-from.template.c
    TO
    seL4GPIOServer-to.template.c
    FROM_HEADER
    seL4GPIOServer-from.template.h
    TO_HEADER
    seL4GPIOServer-to.template.h
)

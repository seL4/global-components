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
CAmkESAddImportPath(interfaces)
CAmkESAddTemplatesPath(templates)

include(${CMAKE_CURRENT_LIST_DIR}/components/PCIConfigIO/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/components/RTC/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/components/SerialServer/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/components/TimeServer/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/components/FileServer/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/components/VirtQueue/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/components/Ethdriver/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/components/PicoServer/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/components/ClockServer/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/components/GPIOMUXServer/CMakeLists.txt)

foreach(
    connector
    IN
    ITEMS
    seL4RPCCallSignal
    seL4RPCCallDataport
    seL4RPCDataport
    seL4RPCDataportSignal
    seL4GlobalAsynch
    seL4GlobalAsynchCallback
    seL4Ethdriver
    seL4VirtQueues
    seL4MessageQueue
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
    seL4TimeServer
    FROM
    seL4RPCCallSignal-from.template.c
    TO
    seL4RPCCallSignal-to.template.c
)
DeclareCAmkESConnector(
    seL4SerialServer
    FROM
    seL4RPCDataportSignal-from.template.c
    TO
    seL4RPCDataportSignal-to.template.c
)
DeclareCAmkESConnector(
    seL4PicoServerSignal
    FROM
    seL4RPCCallSignal-from.template.c
    TO
    seL4RPCCallSignal-to.template.c
)
DeclareCAmkESConnector(
    seL4PicoServer
    FROM
    seL4RPCDataport-from.template.c
    TO
    seL4RPCDataport-to.template.c
)

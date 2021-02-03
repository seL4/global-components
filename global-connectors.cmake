#
# Copyright 2018, Data61, CSIRO (ABN 41 687 119 230)
#
# SPDX-License-Identifier: BSD-2-Clause
#

cmake_minimum_required(VERSION 3.8.2)

CAmkESAddImportPath(interfaces plat_interfaces/${KernelPlatform})

CAmkESAddTemplatesPath(templates)

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

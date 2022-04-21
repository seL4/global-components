#
# Copyright 2018, Data61, CSIRO (ABN 41 687 119 230)
# Copyright 2021, HENSOLDT Cyber GmbH
#
# SPDX-License-Identifier: BSD-2-Clause
#

cmake_minimum_required(VERSION 3.8.2)

CAmkESAddImportPath(interfaces plat_interfaces/${KernelPlatform})

CAmkESAddTemplatesPath(templates)

# Connector templates with FROM and TO only
foreach(
    connector
    IN
    ITEMS
    seL4GlobalAsynch
    seL4GlobalAsynchCallback
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

# Connector templates with FROM, FROM_HEADER, TO and TO_HEADER
foreach(
    connector
    IN
    ITEMS
    seL4RPCCallSignal
    seL4RPCDataport
    seL4RPCDataportSignal
    seL4RPCNoThreads
    seL4GPIOServer
    seL4Ethdriver
)
    DeclareCAmkESConnector(
        ${connector}
        FROM
        ${connector}-from.template.c
        FROM_HEADER
        ${connector}-from.template.h
        TO
        ${connector}-to.template.c
        TO_HEADER
        ${connector}-to.template.h
    )
endforeach()

# Specific connector templates not fitting with the schemes above
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
    seL4RPCCallSignal-from.template.h
    TO
    seL4RPCCallSignal-to.template.c
    TO_HEADER
    seL4RPCCallSignal-to.template.h
)

DeclareCAmkESConnector(
    seL4SerialServer
    FROM
    seL4RPCDataportSignal-from.template.c
    FROM_HEADER
    seL4RPCDataportSignal-from.template.h
    TO
    seL4RPCDataportSignal-to.template.c
)

DeclareCAmkESConnector(
    seL4RPCCallSignalNoThreads
    FROM
    seL4RPCCallSignal-from.template.c
    FROM_HEADER
    seL4RPCCallSignal-from.template.h
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
    seL4RPCCallSignal-from.template.h
    TO
    seL4RPCCallSignal-to.template.c
    TO_HEADER
    seL4RPCCallSignal-to.template.h
)

DeclareCAmkESConnector(
    seL4RPCDataportNoThreads
    FROM
    seL4RPCDataport-from.template.c
    FROM_HEADER
    seL4RPCDataport-from.template.h
    TO
    seL4RPCDataport-to.template.c
    TO_HEADER
    seL4RPCDataport-to.template.h
)

DeclareCAmkESConnector(
    seL4PicoServer
    FROM
    seL4RPCDataport-from.template.c
    FROM_HEADER
    seL4RPCDataport-from.template.h
    TO
    seL4RPCDataport-to.template.c
    TO_HEADER
    seL4RPCDataport-to.template.h
)

DeclareCAmkESConnector(
    seL4GlobalAsynchHardwareInterrupt TO seL4GlobalAsynchHardwareInterrupt.template.c
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
    FROM_HEADER
    seL4VirtQueues-from.template.h
    TO
    empty.c
)

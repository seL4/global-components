#
# Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
#
# SPDX-License-Identifier: BSD-2-Clause
#

cmake_minimum_required(VERSION 3.8.2)

project(Ethdriver C)

CAmkESAddCPPInclude("${CMAKE_CURRENT_LIST_DIR}/include/plat/${KernelPlatform}/")

set(ETHDRIVER_LIBS sel4utils sel4vka sel4allocman sel4vspace sel4simple sel4platsupport ethdrivers)

DeclareCAmkESComponent(
    Ethdriver82574
    SOURCES
    src/ethdriver.c
    src/plat/${KernelPlatform}/82574.c
    INCLUDES
    include
    LIBS
    "${ETHDRIVER_LIBS}"
)

DeclareCAmkESComponent(
    Ethdriver82580
    SOURCES
    src/ethdriver.c
    src/plat/${KernelPlatform}/82580.c
    INCLUDES
    include
    LIBS
    "${ETHDRIVER_LIBS}"
)

DeclareCAmkESComponent(
    EthdriverARMPlat
    SOURCES
    src/ethdriver.c
    INCLUDES
    include
    LIBS
    "${ETHDRIVER_LIBS}"
)

if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/src/plat/${KernelPlatform}/plat.c)
    DeclareCAmkESComponent(EthdriverARMPlat SOURCES src/plat/${KernelPlatform}/plat.c)
endif()

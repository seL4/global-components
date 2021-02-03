#
# Copyright 2019, Data61, CSIRO (ABN 41 687 119 230)
#
# SPDX-License-Identifier: BSD-2-Clause
#

set(GLOBAL_COMPONENTS_DIR "${CMAKE_CURRENT_LIST_DIR}" CACHE STRING "")
set(GLOBAL_COMPONENTS_PATH "${CMAKE_CURRENT_LIST_DIR}/global-components.cmake" CACHE STRING "")
mark_as_advanced(GLOBAL_COMPONENTS_DIR GLOBAL_COMPONENTS_PATH)

macro(global_components_import_project)
    include(${GLOBAL_COMPONENTS_PATH})
endmacro()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    global-components
    DEFAULT_MSG
    GLOBAL_COMPONENTS_DIR
    GLOBAL_COMPONENTS_PATH
)

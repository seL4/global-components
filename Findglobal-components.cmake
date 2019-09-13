#
# Copyright 2019, Data61
# Commonwealth Scientific and Industrial Research Organisation (CSIRO)
# ABN 41 687 119 230.
#
# This software may be distributed and modified according to the terms of
# the BSD 2-Clause license. Note that NO WARRANTY is provided.
# See "LICENSE_BSD2.txt" for details.
#
# @TAG(DATA61_BSD)
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

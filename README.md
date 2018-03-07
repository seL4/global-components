<!--
     Copyright 2017, Data61
     Commonwealth Scientific and Industrial Research Organisation (CSIRO)
     ABN 41 687 119 230.

     This software may be distributed and modified according to the terms of
     the BSD 2-Clause license. Note that NO WARRANTY is provided.
     See "LICENSE_BSD2.txt" for details.

     @TAG(DATA61_BSD)
-->
Global Components
=================

This repository contains a collection of reusable CAmkES components and 
interfaces.

cmake
-----

To include components and interfaces from this repository ensure this directory is
added using `add_subdirectory` in your cmake scripts. In a normal project where
this repository is checked out into the projects/ directory and all subdirectories
of projects/ automatically have `add_subdirectory` performed on them nothing additional
needs to be done.

make
----

To include components and interfaces from this repository
in an application, set the option "Search path for components and interfaces" 
in the "CAmkES Options" section of menuconfig to the directory where this
repository is cloned.

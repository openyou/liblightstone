liblightstone
=============

by Kyle Machulis <kyle@nonpolynomial.com>

Nonpolynomial Labs - http://www.nonpolynomial.com

Description
===========

This is a C-based development library for the Lightstone, a Biometric USB device distributed with the Journey to Wild Divine Relaxation Software (http://www.wilddivine.com)

liblightstone is released under the BSD License.

A Tale of Two Libraries
=======================

liblightstone comes in two flavors: Win32 and libusb-1.0. The Win32 version uses direct Win32 DDK calls to access the windows HID system, and reads raw reports from there. libusb-1.0 works for all platforms currently supporting the library. As of this writing (late December 2009), the libusb-1.0 version of liblightstone has been tested on OS X and linux.

Library Requirements (For Compilation)
======================================

* [CMake (Required on all platforms)](http://www.cmake.org)
* [WDK (Windows Only)](http://www.microsoft.com/whdc/devtools/WDK/default.mspx)
* [libusb-1.0 (All non-windows platforms)](http://www.libusb.org)

Build Notes
===========

To build liblightstone, make a subdirectory in the source dir (called whatever you want), then go into that directory and run "cmake ..". This will generate the project files for your platform (makefiles for OS X and linux, visual studio for windows). For more information on project generators, look at the cmake documentation.

Notes on Compilation and Usage
==============================

Mac OS X
--------

Assuming you have not installed the Journey to Wild Divine software and are not installing the dmg version of liblightstone, you will need to install the "Lightstone OS X Extension" in order to use the lightstone on OS X. The file and installation instructions are available on the liblightstone sourceforge site. Note that there are two different version of the kext, one for 10.4 and earlier, and the other for 10.5 and later.

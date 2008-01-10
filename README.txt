=============
liblightstone
=============

by Kyle Machulis <kyle@nonpolynomial.com>
Nonpolynomial Labs - http://www.nonpolynomial.com

===========
Description
===========

This is a C-based development library for the Lightstone, a Biometric USB device distributed with the Journey to Wild Divine Relaxation Software (http://www.wilddivine.com)

===========================
Lightstone Information
===========================

More information about the Lightstone internals and protocol can be found at 

http://wiki.nonpolynomial.com/Lightstone

=======================
A Tale of Two Libraries
=======================

liblightstone comes in two flavors: Win32 and libhid. The Win32 version uses direct Win32 DDK calls to access the windows HID system, and reads raw reports from there. libhid is completely cross platform (though building on windows requires a patch, which is distributed with the source distribution of liblightstone.

Licensing between these two libraries is also different. As the Win32 version was completely written by the liblightstone team, and is licensed as MIT. The libhid version is GPLv2 due to the fact that libhid itself is GPLv2. Once the HID parser is replaced, the libhid version will most likely become MIT also.

======================================
Library Requirements (For Compilation)
======================================

-----
Win32
-----

(Visual Studio 2005 solution files provided)

Windows DDK: http://www.microsoft.com/whdc/devtools/ddk/default.msxp

------
libhid
------

(cmake cross-platform build files provided)

libusb:
Linux (or optionally OS X, requires modification for Universal Building) - http://libusb.sourceforge.net
OS X (Prebuilt Universal Package) - http://www.ellert.se/twain-sane/
Windows - http://libusb-win32.sourceforge.net/

libhid - http://libhid.alioth.debian.org/

cmake - http://www.cmake.org

==============================
Notes on Compilation and Usage
==============================

-------
Windows
-------

For the moment, it is recommended that you use the Win32 library on Windows. If you decide to use the libhid version on windows, there's a couple of caveats. libhid seems to be much slower than direct win32 calls, and will sometimes send back bogus messages (am currently working on ironing this out). Also, the libhid patch requires that all the files be compiled as C++, as the c99 compatibility seems to be different in VS2005. 

--------
Mac OS X
--------

Assuming you have not installed the Journey to Wild Divine software and are not installing the dmg version of liblightstone, you will need to install the "Lightstone OS X Extension" in order to use the lightstone on OS X. The file and installation instructions are available on the liblightstone sourceforge site.

-----
Linux
-----

The libhid lightstone library does not yet seem to work on Linux. For some reason, pulling the descriptor either doesn't work or just completely locks up the system. There might be a need to make a full kernel level exclusion for the lightstone so it's not picked up by hid.

=============
liblightstone
=============

by Kyle Machulis <kyle@nonpolynomial.com>
Nonpolynomial Labs - http://www.nonpolynomial.com

===========
Description
===========

This is a C-based development library for the Lightstone, a Biometric USB device distributed with the Journey to Wild Divine Relaxation Software (http://www.wilddivine.com)

liblightstone is released under the BSD License.

======================
Lightstone Information
======================

More information about the Lightstone internals and protocol can be found at 

http://wiki.nonpolynomial.com/Lightstone

====================
Library Requirements
====================

For C Driver:

- libusb-1.0: http://www.libusb.org
- cmake (2.6 or higher): http://www.cmake.org

For Python Driver:

- pyusb 1.0+ (Only available through sourceforge svn trunk at the moment): http://pyusb.berlios.de/ 

==============================
Notes on Compilation and Usage
==============================

-------
Windows
-------

As of version 0.9.0 of liblightstone, I can no longer get the Windows DDK version to compile. I don't know if there was a change from the DDK to WDK that caused this or what, but I don't really feel like debugging windows DDK code when the winusb libusb-1.0 branch is progressing. So, for the 0.9.0 version only, consider windows support dropped. The only thing holding liblightstone back from 1.0 is the winusb calls in libusb-1.0, so once that's out, I'll put in the win32 fixes for cmake and roll the version number to 1.0. Until then, use the older version.

--------
Mac OS X
--------

Assuming you have not installed the Journey to Wild Divine software and are not installing the dmg version of liblightstone, you will need to install the "Lightstone OS X Extension" in order to use the lightstone on OS X. The file and installation instructions are available on the liblightstone sourceforge site:

http://is.gd/5GjW7

Note that there are two different version of the kext, one for 10.4 and earlier, and the other for 10.5 and later.



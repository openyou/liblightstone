/*
 * Declaration file for Lightstone User Space Driver
 *
 * Copyright (c) 2007-2008 Kyle Machulis/Nonpolynomial Labs <kyle@nonpolynomial.com>
 *
 * More info on Nonpolynomial Labs @ http://www.nonpolynomial.com
 *
 * Sourceforge project @ http://www.sourceforge.net/projects/liblightstone/
 *
 * This library is covered by the MIT License (Win32 Version) or GPLv2 (LibHID Version)
 * Read LICENSE_MIT.txt or LICENSE_GPL.txt for details.
 */

#ifndef LIBLIGHTSTONE_H
#define LIBLIGHTSTONE_H

#define LIGHTSTONE_VID 0x0483
#define LIGHTSTONE_PID 0x0035

#ifdef USE_LIBHID
#include <hid.h>
typedef HIDInterface* lightstone;
#else
#include <windows.h>
typedef HANDLE lightstone;
#endif

typedef struct
{
	float hrv;
	float scl;
} lightstone_info;


int lightstone_get_count();
int lightstone_open(lightstone* dev, unsigned int device_index);
int lightstone_close(lightstone dev);
lightstone_info lightstone_get_info(lightstone dev);

#endif //LIBTRANCEVIBE_H

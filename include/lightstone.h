/***
 * @file lightstone.h
 * @brief Implementation of lightstone communication
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://liblightstone.nonpolynomial.com/
 *
 */

#ifndef LIBLIGHTSTONE_H
#define LIBLIGHTSTONE_H

#define LIGHTSTONE_VID 0x0483
#define LIGHTSTONE_PID 0x0035

//VID/PID for different Lightstone
#define LIGHTSTONE_VID_2 0x14FA
#define LIGHTSTONE_PID_2 0x0001

#if USE_WIN32
#include <windows.h>
typedef HANDLE lightstone;
#else
#include "nputil_libusb1.h"
typedef nputil_libusb1_struct lightstone;
#endif

typedef struct
{
	float hrv;
	float scl;
} lightstone_info;

#ifdef __cplusplus
extern "C" {
#endif

	lightstone* lightstone_create();
	void lightstone_delete();
	int lightstone_get_count(lightstone* dev);
	int lightstone_open(lightstone* dev, unsigned int device_index);
	void lightstone_close(lightstone* dev);
	lightstone_info lightstone_get_info(lightstone* dev);
	
#ifdef __cplusplus
}
#endif


#endif //LIBLIGHTSTONE_H

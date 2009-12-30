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

#if !defined(NPUTIL_LIBUSB1_STRUCT)
#define NPUTIL_LIBUSB1_STRUCT
#include "libusb-1.0/libusb.h"
typedef struct {
	struct libusb_context* _context;
	struct libusb_device_handle* _device;
	struct libusb_transfer* _in_transfer;
	struct libusb_transfer* _out_transfer;
	int _is_open;
	int _is_inited;
} nputil_libusb1_struct;
#endif
typedef nputil_libusb1_struct lightstone;

typedef struct
{
	float hrv;
	float scl;
} lightstone_info;

#ifdef __cplusplus
extern "C" {
#endif

	lightstone* lightstone_create();
	void lightstone_delete(lightstone* dev);
	int lightstone_get_count(lightstone* dev);
	int lightstone_open(lightstone* dev, unsigned int device_index);
	void lightstone_close(lightstone* dev);
	lightstone_info lightstone_get_info(lightstone* dev);
	
#ifdef __cplusplus
}
#endif

#endif //LIBLIGHTSTONE_H

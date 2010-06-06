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

#define LIGHTSTONE_VID_PID_PAIRS_COUNT 2
unsigned int lightstone_vid_pid_pairs[LIGHTSTONE_VID_PID_PAIRS_COUNT][2] = { {0x0483, 0x0035}, {0x14FA, 0x0001} };

#if defined(WIN32)
#if !defined(NPUTIL_WIN32_HID_STRUCT)
#define NPUTIL_WIN32_HID_STRUCT
typedef struct {
	HANDLE* _device;
	int _is_open;
	int _is_inited;
} nputil_win32hid_struct;
#endif
typedef nputil_win32_struct lightstone;

#else //Non-Win32 platforms

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
	void lightstone_delete(lightstone* dev);
	
	int lightstone_get_count(lightstone* dev);
	int lightstone_get_count_vid_pid(lightstone* dev, unsigned int vendor_id, unsigned int product_id);
	
	int lightstone_open(lightstone* dev, unsigned int device_index);
	int lightstone_open_vid_pid(lightstone* dev, unsigned int device_index, unsigned int vendor_id, unsigned int product_id);

	void lightstone_close(lightstone* dev);
	int lightstone_valid(lightstone* dev);

	int lightstone_read(lightstone* dev, unsigned char *report, unsigned int report_length);
	lightstone_info lightstone_get_info(lightstone* dev);
	
#ifdef __cplusplus
}
#endif

#endif //LIBLIGHTSTONE_H

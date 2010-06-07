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


#if defined(WIN32)
#if !defined(LIGHTSTONE_STATIC)
#define LIGHTSTONE_DECLSPEC __declspec(dllexport)
#else
#define LIGHTSTONE_DECLSPEC
#endif
#if !defined(NPUTIL_WIN32HID_STRUCT)
#define NPUTIL_WIN32HID_STRUCT
#include <windows.h>
typedef struct {
	HANDLE* _dev;
	int _is_open;
	int _is_inited;
} nputil_win32hid_struct;
#endif
typedef nputil_win32hid_struct lightstone;

#else //Non-Win32 platforms
#define LIGHTSTONE_DECLSPEC

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

	LIGHTSTONE_DECLSPEC lightstone* lightstone_create();
	LIGHTSTONE_DECLSPEC void lightstone_delete(lightstone* dev);
	
	LIGHTSTONE_DECLSPEC int lightstone_get_count(lightstone* dev);
	LIGHTSTONE_DECLSPEC int lightstone_get_count_vid_pid(lightstone* dev, unsigned int vendor_id, unsigned int product_id);
	
	LIGHTSTONE_DECLSPEC int lightstone_open(lightstone* dev, unsigned int device_index);
	LIGHTSTONE_DECLSPEC int lightstone_open_vid_pid(lightstone* dev, unsigned int device_index, unsigned int vendor_id, unsigned int product_id);

	LIGHTSTONE_DECLSPEC void lightstone_close(lightstone* dev);
	LIGHTSTONE_DECLSPEC int lightstone_valid(lightstone* dev);

	int lightstone_read(lightstone* dev, unsigned char *report, unsigned int report_length);
	LIGHTSTONE_DECLSPEC lightstone_info lightstone_get_info(lightstone* dev);
	
#ifdef __cplusplus
}
#endif

#endif //LIBLIGHTSTONE_H

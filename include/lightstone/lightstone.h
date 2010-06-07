/***
 * @file lightstone.h
 * @brief Implementation of lightstone communication
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2007-2010 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://liblightstone.nonpolynomial.com/
 *
 */

#ifndef LIBLIGHTSTONE_H
#define LIBLIGHTSTONE_H

#if defined(WIN32)
#if defined(LIGHTSTONE_DYNAMIC)
/// definition for when building DLLs. Doesn't need to be defined otherwise.
#define LIGHTSTONE_DECLSPEC __declspec(dllexport)
#else
#define LIGHTSTONE_DECLSPEC
#endif

#if !defined(NPUTIL_WIN32HID_STRUCT)
#define NPUTIL_WIN32HID_STRUCT
#include <windows.h>

/**
 * Structure to hold information about Windows HID devices.
 *
 * Structure taken from libnputil. 
 */
typedef struct {
	/// Windows device handle
	HANDLE* _dev;
	/// 0 if device is closed, > 0 otherwise
	int _is_open;
	/// 0 if device is initialized, > 0 otherwise
	int _is_inited;
} nputil_win32hid_struct;
#endif
/// Typedef to keep lightstone type consistent across platforms
typedef nputil_win32hid_struct lightstone;

#else //Non-Win32 platforms
#define LIGHTSTONE_DECLSPEC

#if !defined(NPUTIL_LIBUSB1_STRUCT)
#define NPUTIL_LIBUSB1_STRUCT
#include "libusb-1.0/libusb.h"
/**
 * Structure to hold information about libusb-1.0 devices.
 *
 * Structure taken from libnputil. 
 */
typedef struct {
	/// Library context for libusb-1.0
	struct libusb_context* _context;
	/// Specific device context for libusb-1.0
	struct libusb_device_handle* _device;
	/// In transfer object for asynchronous libusb-1.0 transfers
	struct libusb_transfer* _in_transfer;
	/// Out transfer object for asynchronous libusb-1.0 transfers
	struct libusb_transfer* _out_transfer;
	/// 0 if device is closed, > 0 otherwise
	int _is_open;
	/// 0 if device is not initialized, > 0 otherwise
	int _is_inited;
} nputil_libusb1_struct;
#endif
/// Typedef to keep lightstone type consistent across platforms
typedef nputil_libusb1_struct lightstone;
#endif

/// Lightstone information structure
typedef struct
{
	/**
	 * Heart Rate Variability measurement
	 * 
	 */
	float hrv;
	/**
	 * Skin Conductance Level measurement
	 * 
	 */
	float scl;
} lightstone_info;

#ifdef __cplusplus
extern "C" {
#endif

	/** 
	 * Create a lightstone device structure and return it. Must be
	 * called for each lightstone device to be used.
	 * 
	 * 
	 * @return Initialized device structure
	 */
	LIGHTSTONE_DECLSPEC lightstone* lightstone_create();

	/** 
	 * Destroy a lightstone device. Will also close the device if it
	 * is currently open.
	 * 
	 * @param dev Device structure to destroy
	 */
	LIGHTSTONE_DECLSPEC void lightstone_delete(lightstone* dev);

	/** 
	 * Count the number of lightstones on the system, of all known
	 * VIDs and PIDs for the version of liblightstone
	 * 
	 * @param dev Initialized device structure (does not have to be
	 * opened)
	 * 
	 * @return Number of lightstones connected to the computer 
	 */
	LIGHTSTONE_DECLSPEC int lightstone_get_count(lightstone* dev);

	/** 
	 * Return the count of a certain device VID/PID pair on the system
	 * 
	 * @param dev Initialized device stucture (does not have to be
	 * opened)
	 * @param vendor_id USB Vendor ID for device
	 * @param product_id USB Product ID for device
	 * 
	 * @return Count of VID/PID pair matches connected to the computer
	 */
	LIGHTSTONE_DECLSPEC int lightstone_get_count_vid_pid(lightstone* dev, unsigned int vendor_id, unsigned int product_id);

	/** 
	 * Open a lightstone device of an VID/PID known to liblightstone
	 * 
	 * @param dev Initialized device structure
	 * @param device_index Index of device to open on bus, starts at 0
	 * 
	 * @return 0 if device opened successfully, non-zero otherwise
	 */
	LIGHTSTONE_DECLSPEC int lightstone_open(lightstone* dev, unsigned int device_index);

	/** 
	 * Open a lightstone device of a certain VID/PID 
	 * 
	 * @param dev Initialized device structure
	 * @param device_index Index of device to open on bus, starts at 0
	 * @param vendor_id USB Vendor ID for device
	 * @param product_id USB Product ID for device
	 * 
	 * @return 0 if device opened successfully, non-zero otherwise
	 */
	LIGHTSTONE_DECLSPEC int lightstone_open_vid_pid(lightstone* dev, unsigned int device_index, unsigned int vendor_id, unsigned int product_id);

	/** 
	 * Close an opened device
	 * 
	 * @param dev Initialized and opened device to close.
	 */
	LIGHTSTONE_DECLSPEC void lightstone_close(lightstone* dev);

	/** 
	 * Test whether the current device is initialized
	 * 
	 * @param dev Device to check
	 * 
	 * @return > 0 if device is initialized, 0 otherwise
	 */
	LIGHTSTONE_DECLSPEC int lightstone_valid(lightstone* dev);

	/** 
	 * Internal function used to read via platform specific
	 * functions. Not meant for use outside of library.
	 * 
	 * @param dev Initialized and opened device
	 * @param report Report buffer to read into
	 * @param report_length Lenght of report to read
	 * 
	 * @return 
	 */
	int lightstone_read(lightstone* dev, unsigned char *report, unsigned int report_length);

	/** 
	 * Retreive a single HRV/SCL pair from the device. Blocks until
	 * pair is retrived.
	 * 
	 * @param dev Initialized and opened device
	 * 
	 * @return Structure with latest HRV/SCL reading
	 */
	LIGHTSTONE_DECLSPEC lightstone_info lightstone_get_info(lightstone* dev);
	
#ifdef __cplusplus
}
#endif

#endif //LIBLIGHTSTONE_H

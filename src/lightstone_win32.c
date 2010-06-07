/*
 * Declaration file for Lightstone User Space Driver
 *
 * Copyright (c) 2007-2008 Kyle Machulis/Nonpolynomial Labs <kyle@nonpolynomial.com>
 *
 * More info on Nonpolynomial Labs @ http://www.nonpolynomial.com
 *
 * Sourceforge project @ http://www.sourceforge.net/projects/liblightstone/
 *
 * Win32 code adapted from Lightstone Monitor (http://sourceforge.net/projects/lsm/)
 *
 * This library is covered by the MIT License
 * Read LICENSE_MIT.txt for details.
 */

#include "lightstone/lightstone.h"
#include "nputil/nputil_win32hid.h"

#include "lightstone.c"

LIGHTSTONE_DECLSPEC int lightstone_get_count_vid_pid(lightstone* dev, unsigned int vendor_id, unsigned int product_id)
{
	return nputil_win32hid_count(dev, vendor_id, product_id);
}

LIGHTSTONE_DECLSPEC lightstone* lightstone_create()
{
	return nputil_win32hid_create_struct();   
}

LIGHTSTONE_DECLSPEC void lightstone_delete(lightstone* dev)
{
	nputil_win32hid_delete_struct(dev);
}

LIGHTSTONE_DECLSPEC int lightstone_open_vid_pid(lightstone* dev, unsigned int device_index, unsigned int vendor_id, unsigned int product_id)
{
	return nputil_win32hid_open(dev, vendor_id, product_id, device_index);
}

LIGHTSTONE_DECLSPEC void lightstone_close(lightstone* dev)
{
	nputil_win32hid_close(dev);
}

int lightstone_read(lightstone* dev, unsigned char* report, unsigned int report_length)
{
	return nputil_win32hid_read(dev, report, report_length);
}

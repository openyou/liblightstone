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

#include "lightstone.h"
#include "nputil_win32hid.h"

int lightstone_get_count(lightstone* dev, unsigned int vendor_id, unsigned int product_id)
{
	return nputil_win32hid_count(dev->_dev, vendor_id, product_id);
}

lightstone* lightstone_create()
{
	return nputil_win32hid_create_struct();   
}

void lightstone_delete(lightstone* dev)
{
	nputil_win32hid_delete_struct(dev);
}

int lightstone_open(lightstone* dev, unsigned int device_index, unsigned int vendor_id, unsigned int product_id)
{
	return nputil_win32hid_open(dev->_dev, vendor_id, product_id, device_index);
}

int lightstone_close(lightstone dev)
{
	return nputil_win32hid_close(dev->_dev);
}

int lightstone_read(lightstone* dev, char* report, unsigned int report_length)
{
	int transferred;
	int t;
	t = ReadFile 
		(device->_dev, 
		 report,
		 Capabilities.InputReportByteLength,
		 &transferred,
		 NULL);
	return transferred;
}

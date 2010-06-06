/***
 * @file lightstone_libusb1.c
 * @brief LibUSB based implementation of lightstone communication
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2006-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://liblightstone.nonpolynomial.com/
 *
 */

#include "lightstone/lightstone.h"
#include "nputil/nputil_libusb1.h"
#include <stdio.h>
#include <string.h>

#include <lightstone.c>

lightstone* lightstone_create()
{
	lightstone* s = nputil_libusb1_create_struct();
	nputil_libusb1_init(s);
	return s;
}

void lightstone_delete(lightstone* d)
{
	nputil_libusb1_delete_struct(d);
}

int lightstone_get_count_vid_pid(lightstone* d, unsigned int vendor_id, unsigned int product_id)
{
	return nputil_libusb1_count(d, vendor_id, product_id);
}

int lightstone_open_vid_pid(lightstone* dev, unsigned int device_index, unsigned int vendor_id, unsigned int product_id)
{
	char buffer[9];
	int ret;
	if((ret = nputil_libusb1_open(dev, vendor_id, product_id, device_index)) < 0)
	{
		return ret;
	}
	if(libusb_kernel_driver_active(dev->_device, 0))
	{
		libusb_detach_kernel_driver(dev->_device, 0);
	}
	libusb_claim_interface(dev->_device, 0);
	return 0;
}

void lightstone_close(lightstone* dev)
{
	nputil_libusb1_close(dev);
}

int lightstone_read(lightstone* dev, unsigned char* report, unsigned int report_length)
{
	int transferred;
	int t;
	t = libusb_interrupt_transfer(dev->_device, 0x81, report, report_length, &transferred, 0x10);	
	return transferred;
}


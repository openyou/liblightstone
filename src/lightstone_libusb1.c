/***
 * @file lightstone_libusb1.c
 * @brief LibUSB based implementation of lightstone communication
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2006-2011 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://liblightstone.nonpolynomial.com/
 *
 */


#include <stdlib.h>
#include "lightstone/lightstone.h"


LIGHTSTONE_DECLSPEC lightstone* lightstone_create()
{
	lightstone* s = (lightstone*)malloc(sizeof(lightstone));
	s->_is_open = 0;
	s->_is_inited = 0;
	if(libusb_init(&s->_context) < 0)
	{
		return NULL;
	}
	s->_is_inited = 1;	
	return s;
}

LIGHTSTONE_DECLSPEC int lightstone_get_count(lightstone* s)
{
	struct libusb_device **devs;
	struct libusb_device *found = NULL;
	struct libusb_device *dev;
	size_t i = 0;
	int j = 0;
	int count = 0;

	if (!s->_is_inited)
	{
		return E_LIGHTSTONE_NOT_INITED;
	}
	
	if (libusb_get_device_list(s->_context, &devs) < 0)
	{
		return E_LIGHTSTONE_DRIVER_ERROR;
	}

	while ((dev = devs[i++]) != NULL)
	{
		struct libusb_device_descriptor desc;
		int dev_error_code;
		dev_error_code = libusb_get_device_descriptor(dev, &desc);
		if (dev_error_code < 0)
		{
			break;
		}
		for(j = 0; j < LIGHTSTONE_VID_PID_PAIRS_COUNT; ++j)
		{
			if (desc.idVendor == lightstone_vid_pid_pairs[j][0] && desc.idProduct == lightstone_vid_pid_pairs[j][1])
			{
				++count;
				break;
			}
		}
	}

	libusb_free_device_list(devs, 1);
	return count;
}

LIGHTSTONE_DECLSPEC int lightstone_open(lightstone* s, unsigned int device_index)
{
	int ret;
	struct libusb_device **devs;
	struct libusb_device *found = NULL;
	struct libusb_device *dev;
	size_t i = 0;
	int j = 0;
	int count = 0;
	int device_error_code = 0;

	if (!s->_is_inited)
	{
		return E_LIGHTSTONE_NOT_INITED;
	}

	if ((device_error_code = libusb_get_device_list(s->_context, &devs)) < 0)
	{
		return E_LIGHTSTONE_DRIVER_ERROR;
	}

	while ((dev = devs[i++]) != NULL)
	{
		struct libusb_device_descriptor desc;
		device_error_code = libusb_get_device_descriptor(dev, &desc);
		if (device_error_code < 0)
		{
			libusb_free_device_list(devs, 1);
			return E_LIGHTSTONE_NOT_INITED;
		}
		for(j = 0; j < LIGHTSTONE_VID_PID_PAIRS_COUNT; ++j)
		{
			if (desc.idVendor == lightstone_vid_pid_pairs[j][0] && desc.idProduct == lightstone_vid_pid_pairs[j][1])
			{
				if(count == device_index)
				{
					found = dev;
					break;
				}
				++count;
				break;
			}
			if(found) break;
		}
		if(found) break;
	}

	if (found)
	{
		device_error_code = libusb_open(found, &s->_device);
		if (device_error_code < 0)
		{
			libusb_free_device_list(devs, 1);
			return E_LIGHTSTONE_NOT_INITED;
		}
	}
	else
	{
		return E_LIGHTSTONE_NOT_INITED;		
	}
	s->_is_open = 1;

	if(libusb_kernel_driver_active(s->_device, 0))
	{
		libusb_detach_kernel_driver(s->_device, 0);
	}
	ret = libusb_claim_interface(s->_device, 0);

	return ret;
}

LIGHTSTONE_DECLSPEC int lightstone_close(lightstone* s)
{
	if(!s->_is_open)
	{
		return E_LIGHTSTONE_NOT_OPENED;
	}
	if (libusb_release_interface(s->_device, 0) < 0)
	{
		return E_LIGHTSTONE_NOT_INITED;				
	}
	libusb_close(s->_device);
	s->_is_open = 0;
	return 0;
}

LIGHTSTONE_DECLSPEC void lightstone_delete(lightstone* dev)
{
	free(dev);
}

int lightstone_read(lightstone* dev, unsigned char* input_report)
{
	int trans;
	int ret = libusb_bulk_transfer(dev->_device, LIGHTSTONE_IN_ENDPT, input_report, 8, &trans, 0x10);
	return trans;
}

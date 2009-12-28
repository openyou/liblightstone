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

#include "lightstone.h"
#include <stdio.h>
#include <string.h>

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

int lightstone_get_count(lightstone* d)
{
	int count = 0;
	count += nputil_libusb1_count(d, LIGHTSTONE_VID, LIGHTSTONE_PID);
	//count += nputil_libusb1_count(d, LIGHTSTONE_VID_2, LIGHTSTONE_PID_2);
	return count;
}

int lightstone_open(lightstone* dev, unsigned int device_index)
{
	char buffer[9];
	int ret;
	if((ret = nputil_libusb1_open(dev, LIGHTSTONE_VID, LIGHTSTONE_PID, device_index)) < 0)
	{
		return ret;
	}
	libusb_claim_interface(dev->_device, 0);
	return 0;
}

void lightstone_close(lightstone* dev)
{
	nputil_libusb1_close(dev);
}

unsigned int hex2dec(char *data, unsigned int len)
{
	unsigned int i;
	unsigned int value = 0;

	for (i = 0; i < len; i++) {
		value = value*0x10;
		if (data[i] >= '0' && data[i] <= '9')
			value += data[i]-'0';
		else if (data[i] >= 'A' && data[i] <= 'F')
			value += data[i]-'A' + 10;
		else
			return 0;
	}
	return value;
}

lightstone_info lightstone_get_info(lightstone* dev)
{
	lightstone_info ret;
	//hid_return t;
	ret.hrv = -1;
	ret.scl = -1;
	//	if (DeviceHandle != INVALID_HANDLE_VALUE)
	{
		int NumberOfBytesRead;
		char rawAscii[300];
		unsigned char InputReport[256];
		char message_started = 0;
		int transferred = 0;
		int char_count = 0;
		int ii;
		int t;
		while(1)
		{
			t = libusb_interrupt_transfer(dev->_device, 0x81, InputReport, 8, &transferred, 0x10);
			if(transferred == 0x8)
			{
				for(ii = 1; ii < InputReport[0]+1; ++ii)
				{
					if(!message_started && InputReport[ii] != '<') continue;
					message_started = 1;
					if (InputReport[ii] != 10 && InputReport[ii] != 13)
					{
						rawAscii[char_count] = InputReport[ii];
						++char_count;
					}
					else if ( InputReport[ii] == 13 ) 
					{
						rawAscii[char_count] = 0;
						if ( strlen(rawAscii) > 18) 
						{
							ret.scl = ((float)(((hex2dec(rawAscii+5,2)) << 8) | (hex2dec(rawAscii+7,2)))) * .01;
							ret.hrv = ((float)(((hex2dec(rawAscii+10,2)) << 8) | (hex2dec(rawAscii+12,2)))) * .001;
							return ret;
						}
						message_started = 0;
						char_count = 0;
					}
				}
			}
		}
	}
	return ret;
}

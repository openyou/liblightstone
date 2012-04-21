/***
 * @file lightstone.c
 * @brief Common functions for lightstone communication
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2006-2012 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://liblightstone.nonpolynomial.com/
 *
 */

#include "lightstone/lightstone.h"
#include <hidapi.h>
#include <stdlib.h>
#include <string.h>

/**
 * Opaque structure to hold information about HID devices.
 *
 * @ingroup CoreFunctions
 */
struct lightstone {
	/// hidapi device handle
	hid_device* _dev;
	/// 0 if device is closed, > 0 otherwise
	int _is_open;
	/// 0 if device is initialized, > 0 otherwise
	int _is_inited;
};

LIGHTSTONE_DECLSPEC struct lightstone* lightstone_create()
{
	struct lightstone* s = (struct lightstone*)malloc(sizeof(struct lightstone));
	s->_is_open = 0;
	s->_is_inited = 0;
	hid_init();
	s->_is_inited = 1;	
	return s;
}

LIGHTSTONE_DECLSPEC int lightstone_get_count(struct lightstone* s)
{
	if (!s->_is_inited)
	{
		return E_LIGHTSTONE_NOT_INITED;
	}
	int count = 0;
	struct hid_device_info* devices_old = hid_enumerate(lightstone_vid_pid_pairs[0][0],lightstone_vid_pid_pairs[0][1]);
	struct hid_device_info* devices_new = hid_enumerate(lightstone_vid_pid_pairs[1][0],lightstone_vid_pid_pairs[1][1]);

	struct hid_device_info* device_cur = devices_old;
	while(device_cur) {
		++count;
		device_cur = device_cur->next;
	}
	device_cur = devices_new;
	while(device_cur) {
		++count;
		device_cur = device_cur->next;
	}
	
	hid_free_enumeration(devices_old);
	hid_free_enumeration(devices_new);	
	return count;
}

LIGHTSTONE_DECLSPEC int lightstone_open(struct lightstone* s, unsigned int device_index)
{
	if (!s->_is_inited)
	{
		return E_LIGHTSTONE_NOT_INITED;
	}

	struct hid_device_info* devices_old = hid_enumerate(lightstone_vid_pid_pairs[0][0],lightstone_vid_pid_pairs[0][1]);
	struct hid_device_info* devices_new = hid_enumerate(lightstone_vid_pid_pairs[1][0],lightstone_vid_pid_pairs[1][1]);

	int count = 0;
	
	struct hid_device_info* device_cur = devices_old;
	while(device_cur) {
		if(count == device_index) {
			printf("opening %s!\n");
			s->_dev = hid_open_path(device_cur->path);
			break;
		}
		++count;
		device_cur = device_cur->next;
	}
	if(!s->_dev) {
		device_cur = devices_new;
		while(device_cur) {
			if(count == device_index) {
				s->_dev = hid_open_path(device_cur->path);
				break;
			}
			++count;
			device_cur = device_cur->next;
		}
	}
	hid_free_enumeration(devices_old);
	hid_free_enumeration(devices_new);
	if(!s->_dev) {
		return E_LIGHTSTONE_NOT_OPENED;
	}
	s->_is_open = 1;
	return 0;
}

LIGHTSTONE_DECLSPEC int lightstone_close(struct lightstone* s)
{
	if(!s->_is_open)
	{
		return E_LIGHTSTONE_NOT_OPENED;
	}
	hid_close(s->_dev);
	s->_is_open = 0;
	return 0;
}

LIGHTSTONE_DECLSPEC void lightstone_delete(struct lightstone* dev)
{
	free(dev);
	dev = NULL;
	hid_exit();
}

int lightstone_read(struct lightstone* dev, unsigned char* input_report)
{
	return hid_read(dev->_dev, input_report, 8);;
}

LIGHTSTONE_DECLSPEC unsigned int hex2dec(char *data, unsigned int len)
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

LIGHTSTONE_DECLSPEC int lightstone_valid(struct lightstone* d)
{	
	return d->_is_open;
}

LIGHTSTONE_DECLSPEC lightstone_info lightstone_get_info(struct lightstone* dev)
{
	lightstone_info ret;
	//hid_return t;
	ret.hrv = -1;
	ret.scl = -1;
	if (lightstone_valid(dev))
	{
		char rawAscii[300];
		unsigned char InputReport[256];
		char message_started = 0;
		int transferred = 0;
		int char_count = 0;
		int ii;

		while(1)
		{
			transferred = lightstone_read(dev, InputReport);
			if(transferred == 0x8 || transferred == 0x9)
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

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
	int len;
	nputil_libusb1_open(dev, LIGHTSTONE_VID, LIGHTSTONE_PID, device_index);	

	len = libusb_get_descriptor(dev->_device,
								 LIBUSB_DT_HID,
								 0,
								 (char*)buffer,
								 9);
	
	if (len < 0) {
		printf("Cannot get descriptor!");
		return -1;
	}
	
	len = libusb_get_descriptor(dev->_device,
								 LIBUSB_DT_REPORT,
								 0,
								 (char*)buffer,
								 9);

	if (len < 0) {
		printf("Cannot get report!");
		return -1;
	}

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
		char InputReport[256];
		char message_started = 0;
		int transferred = 0;
		int char_count = 0;
		int ii;
		int t;
		while(1)
		{
			//t = hid_interrupt_read(dev,0x81,InputReport,0x8,10);
			//t = libusb_interrupt_transfer(dev->_device, 0x81, InputReport, 8, &transferred, 0x10);
			t = libusb_control_transfer(dev->_device,
										0x81 + LIBUSB_REQUEST_TYPE_CLASS + LIBUSB_RECIPIENT_INTERFACE,
										0x01, //HID_REPORT_GET
										0 + (0x03 << 8), //HID_RT_FEATURE
										1,
										InputReport,
										0x8,
										0x10);

			//if( t == HID_RET_SUCCESS )
			if(t == 0x8)
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
				printf("Transferred! %d %d\n", transferred, t);
			}
			else
			{
				printf("Not enough transferred! %d %d\n", transferred, t);
			}
		}
	}
	return ret;
}

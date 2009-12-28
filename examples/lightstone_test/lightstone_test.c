#include "lightstone.h"
#include <stdio.h>

int main(int argc, char** argv)
{
	lightstone test = NULL;
	lightstone_info r;
	int ret;

    //Uncomment for libhid debug messages
//#ifdef USE_LIBHID
	hid_set_debug(HID_DEBUG_ALL);
	hid_set_debug_stream(stderr);
	hid_set_usb_debug(0);
//#endif USE_LIBHID
	
	ret = lightstone_get_count(&test, 0);

	if(!ret)
	{
		printf("No lightstones connected!\n");
		return 1;
	}
	printf("Found %d Lightstones\n", ret);
	ret = lightstone_open(&test, 0);
	if(ret < 0)
	{
		printf("Cannot open lightstone!\n");
		return 1;
	}
	while(1)
	{
		r = lightstone_get_info(test);
		if(r.hrv < 0) 
		{
			printf("Error reading lightstone, shutting down!\n");
		}
		printf ("%f %f\n", r.hrv, r.scl);
	}
	return 0;
}

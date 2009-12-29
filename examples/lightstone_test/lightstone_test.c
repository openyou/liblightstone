/***
 * @file lightstone_test.c
 * @brief Tests lightstone connection and communication
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2006-2009 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://liblightstone.nonpolynomial.com/
 *
 */

#include "lightstone/lightstone.h"
#include <stdio.h>

int main(int argc, char** argv)
{
	lightstone* test = lightstone_create();
	lightstone_info r;
	int ret;

    //Uncomment for libhid debug messages
	//#ifdef USE_LIBHID
	//hid_set_debug(HID_DEBUG_ALL);
	//hid_set_debug_stream(stderr);
	//hid_set_usb_debug(0);
	//#endif USE_LIBHID
	
	ret = lightstone_get_count(test);

	if(!ret)
	{
		printf("No lightstones connected!\n");
		return 1;
	}
	printf("Found %d Lightstones\n", ret);
	ret = lightstone_open(test, 0);
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
	lightstone_delete(test);
	return 0;
}

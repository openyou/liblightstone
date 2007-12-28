#include "lightstone.h"
#include <stdio.h>

int main(int argc, char** argv)
{
	lightstone test = NULL;
	lightstone_info r;
	int ret;
	ret = lightstone_get_count(&test, 0);
	if(!ret)
	{
		printf("No lightstones connected!");
		return 1;
	}
	ret = lightstone_open(&test, 0);
	if(!ret)
	{
		printf("Cannot open lightstone!");
		return 1;
	}
	while(1)
	{
		r = lightstone_get_info(test);
		printf ("%f %f\n", r.hrv, r.scl);
	}
	return 0;
}
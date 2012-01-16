/***
 * @file lightstone_test.c
 * @brief Tests lightstone connection and communication
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2006-2011 Nonpolynomial Labs/Kyle Machulis
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
	int ret, count, i, j;

	count = lightstone_get_count(test);

	if(!count)
	{
		printf("No lightstones connected!\n");
		return 1;
	}
	printf("Found %d Lightstones\n", count);
	for(i = 0; i < count; ++i)
	{
		ret = lightstone_open(test, i);
		if(ret < 0)
		{
			printf("Cannot open lightstone!\n");
			return 1;
		}
		printf("Opening lightstone %d\n", i + 1);
		while(1)//for(j = 0; j < 10; ++j)
		{
			r = lightstone_get_info(test);
			if(r.hrv < 0) 
			{
				printf("Error reading lightstone, shutting down!\n");
				break;
			}
			printf ("%f %f\n", r.hrv, r.scl);
		}
		printf("Closed lightstone %d\n", i + 1);
	}
	lightstone_delete(test);
	return 0;
}

#include "lightstone.h"
#include <stdio.h>

int main(int argc, char** argv)
{
	lightstone test = NULL;
	lightstone_info r;
	lightstone_open(&test, 0);
	while(1)
	{
		r = lightstone_get_info(test);
		printf ("%f %f\n", r.hrv, r.scl);
	}
	return 0;
}
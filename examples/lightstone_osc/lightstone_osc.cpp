/***
 * @file lightstone_osc.c
 * @brief Transfers lightstone data via OSC
 * @author Kyle Machulis (kyle@nonpolynomial.com)
 * @copyright (c) 2006-2011 Nonpolynomial Labs/Kyle Machulis
 * @license BSD License
 *
 * Project info at http://liblightstone.nonpolynomial.com/
 *
 */


#include "lightstone/lightstone.h"
#include <stdio.h>
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"

#define ADDRESS "127.0.0.1"
#define PORT 7000

#define OUTPUT_BUFFER_SIZE 1024

int main(int argc, char** argv)
{
	struct lightstone* test = lightstone_create();
	lightstone_info r;
	int ret, count, i, j;

	count = lightstone_get_count(test);

	if(!count)
	{
		printf("No lightstones connected!\n");
		return 1;
	}
	printf("Found %d Lightstones\n", count);
	UdpTransmitSocket transmitSocket( IpEndpointName( ADDRESS, PORT ) );
    
	char buffer[OUTPUT_BUFFER_SIZE];
	osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );

	for(i = 0; i < count; ++i)
	{
		ret = lightstone_open(test, i);
		if(ret < 0)
		{
			printf("Cannot open lightstone!\n");
			return 1;
		}
		printf("Opening lightstone %d\n", i + 1);
		while(1)
		{

			r = lightstone_get_info(test);
			if(r.hrv < 0) 
			{
				printf("Error reading lightstone, shutting down!\n");
				break;
			}
    
			p << osc::BeginBundleImmediate
        << osc::BeginMessage( "/lightstone" ) 
				<< true << (float)r.hrv << (float)r.scl << osc::EndMessage 
        << osc::EndBundle;
    
			transmitSocket.Send( p.Data(), p.Size() );
			p.Clear();
			printf ("%f %f\n", r.hrv, r.scl);
		}
		printf("Closed lightstone %d\n", i + 1);
	}
	lightstone_delete(test);
	return 0;
}

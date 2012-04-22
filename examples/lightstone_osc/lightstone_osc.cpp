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

#include <argtable2.h>
#include <stdio.h>
#include "lightstone/lightstone.h"
#include "oscpack/osc/OscOutboundPacketStream.h"
#include "oscpack/ip/UdpSocket.h"

#define OUTPUT_BUFFER_SIZE 1024

int main(int argc, char** argv)
{

	struct arg_str *host = arg_str0(NULL,"host", NULL, "host to broadcast on (default: 127.0.0.1)");
	struct arg_int *port = arg_int0(NULL,"port", NULL, "port to broadcast on (default: 7000)");
	struct arg_int *index = arg_int0(NULL,"index", NULL, "index of device to use (zero based, default: 0)");
	struct arg_lit *verbose = arg_lit0(NULL,"verbose", "print values to stdout as well as sending to network");
	struct arg_lit *help = arg_lit0(NULL,"help", "print this help and exit");
	struct arg_end *end = arg_end(20);
	void* argtable[] = {host,port,index,help,end};
	const char* progname = "lightstone_osc";
	int exitcode=0;
	int nerrors;
	
	if (arg_nullcheck(argtable) != 0) {
		printf("%s: insufficient memory\n", progname);
		exitcode=1;
		arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
		return exitcode;
	}

	host->sval[0] = "127.0.0.1";
	port->ival[0] = 7000;
	index->ival[0] = 0;
	nerrors = arg_parse(argc,argv,argtable);
	
	printf("lightstone-osc: OSC Utility for Lightstone Data. lightstone-osc --help for options.\n");
	/* special case: '--help' takes precedence over error reporting */
	if (help->count > 0) {
		printf("Usage: %s", progname);
		arg_print_syntax(stdout,argtable,"\n");
		printf("\n");
		printf("OpenSoundControl output for the Lightstone/IOM Device. Connects\n");
		printf("to the device given by [index] and sends OSC messages out\n");
		printf("on [host]:[port]. Messages are of the format:\n");
		printf("\n");
		printf("/lightstone/[hrv:float]/[scl:float]\n");
		printf("\n");
		printf("HRV is heart rate variable, SCL is skin conductance level. Messages\n");
		printf("should be produced at around 30hz.\n");
		arg_print_glossary(stdout,argtable,"  %-25s %s\n");
		exitcode=0;
		arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
		return exitcode;
	}

	/* If the parser returned any errors then display them and exit */
	if (nerrors > 0) {
		/* Display the error details contained in the arg_end struct.*/
		arg_print_errors(stdout,end,progname);
		printf("Try '%s --help' for more information.\n",progname);
		exitcode=1;
		arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
		return exitcode;
	}

	struct lightstone* test;
	lightstone_info r;
	int ret, count;
	count = 0;
	test = lightstone_create();
	count = lightstone_get_count(test);

	if(!count)
	{
		printf("No lightstones connected!\n");
		return 1;
	}
	printf("Found %d Lightstones\n", count);
	if(count - 1 < index->ival[0])
	{
		printf("Index %d higher than number of connected lightstones (%d)!\n", index->ival[0], count);
		return 1;
	}

	char buffer[OUTPUT_BUFFER_SIZE];
	osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );
	UdpTransmitSocket transmitSocket( IpEndpointName( host->sval[0], port->ival[0] ) );
	printf("Establishing network connection on %s:%d\n", host->sval[0], port->ival[0]);
	printf("Opening lightstone %d\n", index->ival[0]);
	ret = lightstone_open(test, index->ival[0] );
	arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
	if(ret < 0)
	{
		printf("Cannot open lightstone!\n");
		return 1;
	}
	printf("starting!\n");
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
			<< 1.0f << 1.0f << osc::EndMessage 
			<< osc::EndBundle;
    
		transmitSocket.Send( p.Data(), p.Size() );
		p.Clear();
		printf ("%f %f\n", r.hrv, r.scl);
	}
	printf("Closed lightstone\n");
	lightstone_delete(test);
	return exitcode;
}

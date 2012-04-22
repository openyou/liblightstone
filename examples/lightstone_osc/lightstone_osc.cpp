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
#include <iostream>
#include "lightstone/lightstone.h"
#include "oscpkt/oscpkt.hh"
#include "oscpkt/udp.hh"

#define OUTPUT_BUFFER_SIZE 1024

using std::cout;
using std::cerr;
using namespace oscpkt;

int main(int argc, char** argv)
{

	struct arg_str *arg_host = arg_str0(NULL,"host", NULL, "host to broadcast on (default: 127.0.0.1)");
	struct arg_int *arg_port = arg_int0(NULL,"port", NULL, "port to broadcast on (default: 7000)");
	struct arg_int *arg_index = arg_int0(NULL,"index", NULL, "index of device to use (zero based, default: 0)");
	struct arg_lit *arg_verbose = arg_lit0(NULL,"verbose", "print values to stdout as well as sending to network");
	struct arg_lit *arg_quiet = arg_lit0(NULL,"quiet", "don't print beginning messages (still prints updates if --verbose)");
	struct arg_lit *arg_help = arg_lit0(NULL,"help", "print this help and exit");
	struct arg_end *arg_final = arg_end(20);
	void* argtable[] = {arg_host,arg_port,arg_index,arg_verbose,arg_quiet,arg_help,arg_final};
	const char* progname = "lightstone_osc";
	int exitcode=0;
	int nerrors;
	bool exitearly = false;
	
	if (arg_nullcheck(argtable) != 0) {
		printf("%s: insufficient memory\n", progname);
		exitcode=1;
		arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
		return exitcode;
	}

	arg_host->sval[0] = "127.0.0.1";
	arg_port->ival[0] = 7000;
	arg_index->ival[0] = 0;
	nerrors = arg_parse(argc,argv,argtable);
	const char* host = arg_host->sval[0];
	int port = arg_port->ival[0];
	int quiet = arg_quiet->count;
	int verbose = arg_verbose->count;
	int index = arg_index->ival[0];
		
	if(!quiet) printf("lightstone-osc: OSC Utility for Lightstone Data. lightstone-osc --help for options.\n");
	/* special case: '--help' takes precedence over error reporting */
	if (arg_help->count > 0) {
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
		exitearly = true;
	}

	/* If the parser returned any errors then display them and exit */
	if (nerrors > 0) {
		/* Display the error details contained in the arg_end struct.*/
		arg_print_errors(stdout,arg_final,progname);
		printf("Try '%s --help' for more information.\n",progname);
		exitcode=1;
		exitearly = true;
	}

	arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
	if(exitearly) {
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
	if(!quiet) printf("Found %d Lightstones\n", count);
	if(count - 1 < index)
	{
		printf("Index %d higher than number of connected lightstones (%d)!\n", index, count);
		return 1;
	}

	UdpSocket sock;
	sock.connectTo(host, port);
	if (!sock.isOk()) {
		printf("Error establishing network connection on %s:%d\n", host, port);
		return 1;
	}
	if(!quiet) printf("Establishing network connection on %s:%d\n", host, port);
	if(!quiet) printf("Opening lightstone %d\n", index);
	ret = lightstone_open(test, index);
	
	if(ret < 0)
	{
		printf("Cannot open lightstone!\n");
		return 1;
	}
	Message m("/lightstone");
	PacketWriter pw;
	while(1)
	{		
		r = lightstone_get_info(test);
		if(r.hrv < 0) 
		{
			printf("Error reading lightstone, shutting down!\n");
			break;
		}
		m.clear();
		m = Message("/lightstone");
		m.pushFloat(r.hrv);
		m.pushFloat(r.scl);
		pw.startBundle().startBundle().addMessage(m).endBundle().endBundle();
		bool ok = sock.sendPacket(pw.packetData(), pw.packetSize());
		if(verbose) printf ("%f %f\n", r.hrv, r.scl);
	}
	printf("Closed lightstone\n");
	lightstone_delete(test);
	return exitcode;
}

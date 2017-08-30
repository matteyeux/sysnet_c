#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <assert.h>

#include <include/system.h>
#include <include/network.h>
#include <include/cpu.h>

/*Easier to understand*/
#define macosx	__APPLE__ && __MACH__

#ifdef DEBUG
	#define VERSION "1.2.0-DEBUG"
#else 
	#define VERSION "1.2.0"
#endif
#define TOOLNAME "Sysnet"

static struct option longopts[] = {
	{ "all", 		no_argument,	NULL, 'a'},
	{ "system",		no_argument,	NULL, 's'},
	{ "network", 	no_argument,	NULL, 'n'},
	{ "disk", 		required_argument,	NULL, 'd'},
	{ "cpu", 		no_argument, 	NULL, 'c'},
	{ "version", 	no_argument, 	NULL, 'v'},
	{ "help", 		no_argument, 	NULL, 'h'},
	{ NULL, 0, NULL, 0 }
};

void usage(int argc, char *argv[])
{
	char *name = NULL;
	name = strrchr(argv[0], '/');
	fprintf(stdout, "Usage : %s [OPTIONS]\n",(name ? name + 1: argv[0]));
	fprintf(stdout, " -s, --system\t\t\tsystem information\n");
	fprintf(stdout, " -n, --network <interface>\tnetwork information\n");
	#if defined (__x86_64__) || defined (__i386__) || defined (__i366__)
	fprintf(stdout, " -c, --cpu\t\t\tcpu information\n");
	#endif
	fprintf(stdout, " -d, --disk\t\t\tdisk information\n");
	fprintf(stdout, " -a, --all\t\t\tall information\n");
	fprintf(stdout, " -v, --version\t\t\tversion\n");
	#ifdef DEBUG
		printf("DEBUG: ON\n");
	#endif
}

int main(int argc, char *argv[])
{	
	int opt;
	int optindex=0;

	if (argc < 2)
	{	
		usage(argc, argv);
		return 0;
	}
	while((opt = getopt_long(argc, (char* const *)argv, "asnchvd", longopts, &optindex)) != -1)
	{
		switch (opt)
		{
			case 'h' :
				usage(argc, argv);
				return 0;

			case 'a' :
				fprintf(stdout, "=== System ===\n");
				username();
				infosys();
				#ifdef linux
				raminfo();
				#endif
				fprintf(stdout, "\n=== Disk ===\n");
				disk_info("/");
				fprintf(stdout, "\n=== Network ===\n");
				hostname();
				network_info();
				#ifdef linux
					print_gateway();
				#endif
				#if defined (__x86_64__) || defined (__i386__) || defined (__i366__)
				fprintf(stdout, "\n=== CPU ===\n");
				cpu_info();
				#endif
				break;

			case 's' : 
				username();
				infosys();
				#ifdef linux
				raminfo();
				#endif 
				break;

			case 'n' :
				hostname();
				if (argv[optind])
				{
					network_info(argv[optind]);
				} else {
					network_info(NULL);
				}
				#ifdef linux
				print_gateway();
				#endif
				break;

			case 'd' :
				if (!argv[optind])
				{	
					argv[optind] = "/";
				}
				disk_info(argv[optind]);
				break;
			case 'c':
				cpu_info();
				break;

			case 'v' :
				fprintf(stdout, "%s, version %s\nCompiled on %s\nCopyright 2016-2017 - Mathieu Hautebas\n", TOOLNAME, VERSION, __DATE__);
				break;

			default:
				usage(argc, argv);
				return -1;
		}
	}
	return 0;
}

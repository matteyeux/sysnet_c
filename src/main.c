#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <assert.h>
#include "functions.h"

/*Easier to understand*/
#define macosx	__APPLE__ && __MACH__

#define VERSION "1.1.1"
#define TOOLNAME "Sysnet"
#define FLAG_EXTRACT    1 << 0

//extern char *optarg;
static struct option longopts[] = {
	{ "all", 		no_argument,	NULL, 'a'},
	{ "system",		no_argument,	NULL, 's'},
	{ "network", 	no_argument,	NULL, 'n'},
	{ "disk", 		required_argument,	NULL, 'd'}, // I have an idea for this arg, need to investigate more
	{ "cpu", 		no_argument, 	NULL, 'c'},
	{ "version", 	no_argument, 	NULL, 'v'},
	{ "help", 		no_argument, 	NULL, 'h'},
	{ NULL, 0, NULL, 0 }
};

void usage(int argc, char *argv[])
{
	char *name = NULL;
    name = strrchr(argv[0], '/');
	printf("Usage : %s [OPTIONS]\n",(name ? name + 1: argv[0]));
	printf(" -s, --system\tsystem information\n");
	printf(" -n, --network\tnetwork information\n");
	printf(" -c, --cpu\tcpu information\n");
	printf(" -d, --disk\tdisk information\n");
	printf(" -a, --all\tall information\n");
	printf(" -v, --version\tversion\n");
	//int test = typename(optarg);
	//printf("%d",test);
}

int test (char* testvar)
{
    printf("%s\n",testvar);
    return 0;
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
	while((opt = getopt_long(argc, (char* const *)argv, "asnchv:d", longopts, &optindex)) != -1)
	{
		switch (opt)
		{
			case 'h' :
				usage(argc, argv);
				return 0;

			case 'a' :
				printf("=== System ===\n");
				username();
				infosys();
				#ifdef linux
				raminfo();
				#endif
				//disk_info();
				printf("\n=== Network ===\n");
				hostname();
				network_info();
				#ifdef linux
					print_gateway();
				#endif
				printf("\n=== CPU ===\n");
				cpu_info();
				return 0;

			case 's' : 
				username();
				infosys();
				#ifdef linux
				raminfo();
				#endif 
				//disk_info();
				break;

			case 'n' :
				hostname();
				network_info();
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
				printf("%s, version %s\nCopyright 2016 - Mathieu Hautebas\n", TOOLNAME, VERSION);
				break;

			default:
				usage(argc, argv);
				return -1;
		}
	}
	return 0;
}

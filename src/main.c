#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include "functions.h"

/*Easier to understand*/
#define macosx	__APPLE__ && __MACH__

#define VERSION "1.1"
#define TOOLNAME "Sysnet"

static struct option longopts[] = {
	{ "system",		no_argument,	NULL, 's'},
	{ "network", 	no_argument,	NULL, 'n'},
	{ "disk", 		no_argument,	NULL, 'd'},
	{ "log", 		no_argument,	NULL, 'l'}, //Maybe required_argument
	{ "all", 		no_argument,	NULL, 'a'},
	{ "version", 	no_argument, 	NULL, 'v'},
	{ "help", 		no_argument, 	NULL, 'h'},
	{ NULL, 0, NULL, 0 }
};

void usage(int argc, char *argv[])
{
	char *name = NULL;
    name = strrchr(argv[0], '/');
	printf("Usage : %s [OPTIONS]\n",(name ? name + 1: argv[0]));
	printf(" -s, --system\tsystem informations\n");
	printf(" -n, --network\tnetwork informations\n");
	printf(" -a, --all\tsystem & network infos\n");
	printf(" -v, --version\tversion\n");
}

int main(int argc, char *argv[])
{	
	int opt = 0;
	int optindex=0;
	
	if (argc < 2)
	{	
		usage(argc, argv);
		return 0;
	}
	while((opt = getopt_long(argc, argv, "alnhsdv", longopts, &optindex)) > 0)
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
				raminfo();
				disk_infos();
				printf("\n=== Network ===\n");
				hostname();
				network_infos();
				#ifdef linux
					print_gateway();
				#endif
				return 0;

			case 's' : 
				username();
				infosys();
				raminfo();
				disk_infos();
				break;

			case 'n' :
				hostname();
				network_infos();
				#ifdef linux
				print_gateway();
				#endif
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

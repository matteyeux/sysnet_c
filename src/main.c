#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>

#include <include/system.h>
#include <include/network.h>
#include <include/cpu.h>

/*Easier to understand*/

#ifdef DEBUG
	#define VERSION "1.4.0-DEBUG"
#else 
	#define VERSION "1.4.0"
#endif
#define TOOLNAME "sysnet"

static struct option longopts[] = {
	{ "all", 		no_argument,	NULL, 'a'},
	{ "system",		no_argument,	NULL, 's'},
	{ "network",	no_argument,	NULL, 'n'},
	{ "wireless",	required_argument, NULL, 'w'},
	{ "disk", 		required_argument,	NULL, 'd'},
	{ "cpu", 		no_argument, 	NULL, 'c'},
	{ "file", 		required_argument, NULL, 'f'},
	{ "version", 	no_argument, 	NULL, 'v'},
	{ "help", 		no_argument, 	NULL, 'h'},
	{ NULL, 0, NULL, 0 }
};

void usage(char *argv[])
{
	char *name = NULL;
	name = strrchr(argv[0], '/');
	fprintf(stdout, "Usage : %s [OPTIONS]\n",(name ? name + 1: argv[0]));
	fprintf(stdout, " -s, --system\t\t\tsystem information\n");
	fprintf(stdout, " -n, --network <list|interface>\tnetwork information\n");
	#ifdef linux
	fprintf(stdout, " -w, --wireless <interface>\tfind wireless network\n");
	#endif
	#if defined (__x86_64__) || defined (__i386__) || defined (__i366__)
	fprintf(stdout, " -c, --cpu\t\t\tcpu information\n");
	#endif
	fprintf(stdout, " -d, --disk\t\t\tdisk information\n");
	fprintf(stdout, " -f, --file <file>\t\tfile information\n");
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
	int all = 0;
	int system = 0;
	int network = 0;
	int wireless = 0;
	int disk = 0;
	int file = 0;

	#ifdef linux
	char *wireless_iface;
	#endif /* linux */

	#ifdef LIBCPUID
	int cpu = 0;
	#endif /* LIBCPUID */
	if (argc < 2) {
		usage(argv);
		return 0;
	}

	while((opt = getopt_long(argc, (char* const *)argv, "asnchvdfw", longopts, &optindex)) != -1) {
		switch (opt) {
			case 'h' :
				usage(argv);
				return 0;
			case 'a' :
				all = 1;
				break;
			case 's' : 
				system = 1;
				break;
			case 'n' :
				network = 1;
				break;
			case 'w' :
				wireless = 1;
				break;
			case 'd' :
				disk = 1;
				break;
			#ifdef LIBCPUID
			case 'c':
				cpu = 1;
				break;
			#endif
			case 'f' :
				file = 1;
				break;
			case 'v' :
				fprintf(stdout, "%s, version %s\nCompiled on %s\nCopyright 2016-2020 - Mathieu Hautebas\n", TOOLNAME, VERSION, __DATE__);
				break;
			default:
				usage(argv);
				return -1;
		}
	}

	if (all) {
		fprintf(stdout, "=== System ===\n");
		username();
		infosys();
		#ifdef linux
		raminfo();
		#endif
		fprintf(stdout, "\n=== Disk ===\n");
		disk_info("/");
		fprintf(stdout, "\n=== Network ===\n");
		print_network_info();
		grab_gateway();
		#ifdef LIBCPUID
		fprintf(stdout, "\n=== CPU ===\n");
		cpu_info();
		#endif
	}

	if (system) {
		username();
		infosys();
		#ifdef linux
		raminfo();
		#endif
	}

	if (network) {
		print_network_info();
		grab_gateway();
	}

	if (wireless) {
		if (argv[optind] != NULL) {
			wireless_iface = argv[optind];
		} else {
			wireless_iface = get_wireless_iface();

			if (wireless_iface == NULL) {
				fprintf(stderr, "[-] no wireless interface found\n");
				return -2;
			}
		}

		/* moved the check here in case you need to enable a wireless interface */
		if (getuid() != 0) {
			fprintf(stderr, "[-] you need higher privileges\n");
			return -1;
		}

		if (is_iface_up(wireless_iface) != true)
			up_iface(wireless_iface);

		find_wifi(wireless_iface);
	}

	if (disk) {
		if (!argv[optind])
			argv[optind] = "/";

		disk_info(argv[optind]);
	}

	#ifdef LIBCPUID
	if (cpu) {
		cpu_info();
	}
	#endif

	if (file) {
		if (!argv[optind])
			argv[optind] = ".";

		fileinfo(argv[optind]);
	}
	return 0;
}

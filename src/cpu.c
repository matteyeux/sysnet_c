#include <stdio.h>
#include <include/cpu.h>

#ifdef LIBCPUID
#include <libcpuid/libcpuid.h>
struct cpu_raw_data_t raw; 
struct cpu_id_t data;    
#endif

int cpu_init()
{
	#ifdef LIBCPUID
	if (!cpuid_present()) {
		fprintf(stderr, "Sorry, your CPU doesn't support CPUID!\n");
		return -1;
	}


	if (cpuid_get_raw_data(&raw) < 0) {
		fprintf(stderr, "Sorry, cannot get the CPUID raw data.\n");
		fprintf(stderr, "Error: %s\n", cpuid_error());
		return -2;
	}

	if (cpu_identify(&raw, &data) < 0) {
		fprintf(stderr, "Sorrry, CPU identification failed.\n");
		fprintf(stderr, "Error: %s\n", cpuid_error());
		return -3;
	}
	#else
	fprintf(stderr, "[ERROR] CPU not supported\n");
	#endif
	return 0;
}

int cpu_info()
{
	#ifdef LIBCPUID
	cpu_init();
	fprintf(stdout, "Vendor :\t\t%s\n", data.vendor_str);
	fprintf(stdout, "Model :\t\t\t%s\n", data.brand_str);
	fprintf(stdout, "Physical cores :\t%d\n", data.num_cores);
	fprintf(stdout, "Codename :\t\t%s\n", data.cpu_codename);
	#endif
	return 0;
}

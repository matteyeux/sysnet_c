#include <stdio.h>
#include <libcpuid/libcpuid.h>

struct cpu_raw_data_t raw; 
struct cpu_id_t data;    

int cpu_init()
{
    if (!cpuid_present()) {
        printf("Sorry, your CPU doesn't support CPUID!\n");
        return -1;
    }


    if (cpuid_get_raw_data(&raw) < 0) { 
        printf("Sorry, cannot get the CPUID raw data.\n");
        printf("Error: %s\n", cpuid_error());
        return -2;
    }

    if (cpu_identify(&raw, &data) < 0) {    
        printf("Sorrry, CPU identification failed.\n");
        printf("Error: %s\n", cpuid_error());
        return -3;
    }
}

int cpu_info()
{
	cpu_init();
    printf("Vendor :\t\t%s\n", data.vendor_str);
    printf("Model :\t\t\t%s\n", data.brand_str);
    printf("Physical cores :\t%d\n", data.num_cores);
    return 0;
}

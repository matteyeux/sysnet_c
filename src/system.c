#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#ifdef linux
#include <sys/sysinfo.h>
#endif

#include "functions.h"
#ifdef linux
struct sysinfo sys_info;
#endif
int username()
{
    char *p=getenv("USER");
    if(p==NULL)
    { 
    	return EXIT_FAILURE;
    }
    printf("User : \t\t\t%s\n",p);
    return 0;   
}

int infosys() 
{
	if (uname(&buf) != 0)
	{	
		abort();
	}
	printf("Operating System :\t%s\nversion :\t\t%s\narchitecture : \t\t%s\n", buf.sysname, buf.release, buf.machine);
	#ifdef linux
	if(sysinfo(&sys_info) != 0)
		perror("sysinfo");
	printf("n° of processes : \t%d\n", sys_info.procs);
	#endif
	printf("shell : \t\t%s\n",getenv("SHELL"));
	return 0;
}

int raminfo() 
{
    struct sysinfo si;
    if (sysinfo(&si) == 0) {
    	double used_ram =  si.totalram/pow(2,30) - si.freeram/pow(2,30);
    	printf("\nUsed RAM : \t\t%.2lf GB\n", used_ram);
		printf("Free RAM : \t\t%.2lf GB\n",si.freeram/pow(2,30));
		printf("Total RAM : \t\t%.2lf GB\n", si.totalram/pow(2,30)); //It's Go for frenchies
    };
    return 0;
}

void disk_infos()
{
	char *filename = "/"; // Gonna add it as an arg *or not*

	struct statvfs buf;
	if (!statvfs(filename, &buf)) {
		unsigned long blksize, blocks, freeblks, disk_size, used, free;
		blksize = buf.f_bsize;
		blocks = buf.f_blocks;
		freeblks = buf.f_bfree;

		disk_size = blocks * blksize;
		free = freeblks * blksize;
		used = disk_size - free;
		printf("\nDisk usage of %s : \t%.2lf GB\nFree space in %s : \t%.2lf GB\nTotal : \t\t%.2lf GB\n", filename, used/pow(2,30), filename, free/pow(2,30), disk_size/pow(2,30));
    } 
    else {
		printf("Couldn't get file system statistics\n");
	}
}

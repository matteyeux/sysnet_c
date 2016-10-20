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
		//#ifdef linux
			printf("Disk usage of %s : \t%lu \nFree space in %s : \t%lu\nTotal : \t\t%lu\n", filename, used, filename, free, disk_size);
		//#else
		//	printf("Disk usage of %s : \t%u \nFree space in %s : \t%u\nTotal : \t%u\n", filename, used, filename, free, disk_size);
		//#endif
    } 
    else {
		printf("Couldn't get file system statistics\n");
	}
}

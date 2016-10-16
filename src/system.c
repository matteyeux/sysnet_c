#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "functions.h"


int username()
{
    char *p=getenv("USER");
    if(p==NULL)
    { 
    	return EXIT_FAILURE;
    }
    printf("User : %s\n",p);
    return 0;   
}

int infosys() 
{
	//struct utsname buf;
	if (uname(&buf) != 0)
	{	
		abort();
	}
	printf("Operating System : %s\nversion : %s\narchitecture : %s\n", buf.sysname, buf.release, buf.machine);
	return 0;
}

void disk_infos()
{
	char *filename = "/"; // Gonna add it as an arg

	struct statvfs buf;
	if (!statvfs(filename, &buf)) {
		unsigned long blksize, blocks, freeblks, disk_size, used, free;
		blksize = buf.f_bsize;
		blocks = buf.f_blocks;
		freeblks = buf.f_bfree;

		disk_size = blocks * blksize;
		free = freeblks * blksize;
		used = disk_size - free;
		#ifdef linux
			printf("Disk usage of %s : %lu \nFree space in %s : %lu\nTotal : %lu\n", filename, used, filename, free, disk_size);
		#else
			printf("Disk usage of %s : %u \nFree space in %s : %u\nTotal : %u\n", filename, used, filename, free, disk_size);
		#endif
    } 
    else {
		printf("Couldn't get file system statistics\n");
	}
}
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#ifdef linux
#include <sys/sysinfo.h>
#endif

#include <include/system.h>
#include <include/common.h>

int username()
{
    char *user=getenv("USER");
	int uid = getuid();
    if(user==NULL)
    { 
    	return EXIT_FAILURE;
    }
    fprintf(stdout, "User : \t\t\t%s\n",user);
    fprintf(stdout, "UID : \t\t\t%d\n", uid);
	return 0;   
}

int infosys() 
{
	if (uname(&buf) != 0)
	{	
		abort();
	}
	fprintf(stdout, "Operating System :\t%s\nversion :\t\t%s\narchitecture : \t\t%s\n", buf.sysname, buf.release, buf.machine);
	#ifdef linux
	if(sysinfo(&sys_info) != 0)
		perror("sysinfo");
	fprintf(stdout, "n° of processes : \t%d\n", sys_info.procs);
	fprintf(stdout, "swap : %ld\n", sys_info.totalswap);
	#endif
	fprintf(stdout, "shell : \t\t%s\n",getenv("SHELL"));
	return 0;
}
#ifdef linux
int raminfo() 
{
    struct sysinfo si;
    if (sysinfo(&si) == 0) {
    	double used_ram =  convert2gb(si.totalram, si.freeram);
    	fprintf(stdout, "\nUsed RAM : \t\t%.2lf GB\n", used_ram);
		fprintf(stdout, "Free RAM : \t\t%.2lf GB\n", convert2gb(0, si.freeram));
		fprintf(stdout, "Total RAM : \t\t%.2lf GB\n", convert2gb(0, si.totalram));
    }
    return 0;
}
#endif
int disk_info(const char *partition)
{
	struct statvfs buf;
	if (!statvfs(partition, &buf)) {
		unsigned long blksize, blocks, freeblks, disk_size, used, free;
		blksize = buf.f_bsize;
		blocks = buf.f_blocks;
		freeblks = buf.f_bfree;

		disk_size = blocks * blksize;
		free = freeblks * blksize;
		used = disk_size - free;
		fprintf(stdout, "Disk usage of %s : \t%.2lf GB\nFree space in %s : \t%.2lf GB\nTotal in %s: \t\t%.2lf GB\n", partition, used/pow(2,30), partition, free/pow(2,30), partition, disk_size/pow(2,30));
    } 
    else {
		fprintf(stderr, "Couldn't get file system statistics\n");
	}
	return 0;
}

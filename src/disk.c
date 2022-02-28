#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <math.h>
#include <include/disk.h>
#include <include/common.h>

struct utsname buf;

int disk_info(const char *partition)
{
	struct statvfs buf;
	int div = 1;
	if (!statvfs(partition, &buf)) {
		unsigned long blksize, blocks, freeblks, disk_size, used, available;
		blksize = buf.f_bsize;
		blocks = buf.f_blocks;
		freeblks = buf.f_bfree;

		disk_size = blocks * blksize;
		available = freeblks * blksize;
		used = disk_size - available;
		#ifdef __APPLE__
			div = 1000;
		#endif
		unsigned long disk_size_gb = convert2gb(0, disk_size) / div;
		unsigned long used_gb = convert2gb(0, used) / div;
		unsigned long free_gb = convert2gb(0, available) / div;

		fprintf(stdout, "Disk usage of %s : \t%.2lu GB\t",	partition, used_gb);
		fprintf(stdout, "%d%%\n", 							convert2percent(used_gb, disk_size_gb));
		fprintf(stdout, "Free space in %s : \t%.2lu GB\t",	partition, free_gb);
		fprintf(stdout, "%d%%\n", 							convert2percent(free_gb, disk_size_gb));
		fprintf(stdout, "Total in %s: \t\t%.2lu GB\n",		partition, disk_size_gb);
	}
	else {
		fprintf(stderr, "Couldn't get file system statistics\n");
	}
	return 0;
}

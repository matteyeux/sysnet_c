#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <errno.h>
#include <sys/sysinfo.h>

#include <include/system.h>
#include <include/disk.h>
#include <include/common.h>

struct sysinfo sys_info;

int username(void)
{
	char *user=getenv("USER");
	int uid = getuid(), gid = getgid();

	if(user == NULL) {
		return EXIT_FAILURE;
	}

	fprintf(stdout, "User : \t\t\t%s\n",user);
	fprintf(stdout, "UID : \t\t\t%d\n", uid);
	fprintf(stdout, "GID : \t\t\t%d\n", gid);
	return 0;
}

void infosys(void)
{
    struct utsname buf;
	if (uname(&buf) != 0)
		abort();

	fprintf(stdout, "Operating System :\t%s\nversion :\t\t%s\narchitecture : \t\t%s\n", buf.sysname, buf.release, buf.machine);

	if(sysinfo(&sys_info) != 0)
		perror("sysinfo");

	fprintf(stdout, "n° of processes : \t%d\n", sys_info.procs);
	fprintf(stdout, "swap : \t\t\t%.2lf GB\n", convert2gb(0, sys_info.totalswap));
	fprintf(stdout, "shell : \t\t%s\n",getenv("SHELL"));

}

int raminfo(void)
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

int fileinfo(char *path2file)
{
	struct stat sb;
	int get_stat = stat(path2file, &sb);
	if (get_stat == -1) {
		perror("stat");
		return get_stat;
	}

	fprintf(stdout, "File type:				  ");

	switch (sb.st_mode & S_IFMT) {
		case S_IFBLK:	fprintf(stdout, "block device\n");			break;
		case S_IFCHR:	fprintf(stdout, "character device\n");		break;
		case S_IFDIR:	fprintf(stdout, "directory\n");				break;
		case S_IFIFO:	fprintf(stdout, "FIFO/pipe\n");				break;
		case S_IFLNK:	fprintf(stdout, "symlink\n");				break;
		case S_IFREG:	fprintf(stdout, "regular file\n");			break;
		case S_IFSOCK:	fprintf(stdout, "socket\n");				break;
		default:		fprintf(stdout, "unknown?\n");				break;
	}

	fprintf(stdout, "I-node number:				%ld\n", (long) sb.st_ino);

	fprintf(stdout, "Mode:						%lo (octal)\n", (unsigned long) sb.st_mode);

	fprintf(stdout, "Link count:				%ld\n", (long) sb.st_nlink);
	fprintf(stdout, "Ownership:					UID=%ld   GID=%ld\n", (long) sb.st_uid, (long) sb.st_gid);

	fprintf(stdout, "Preferred I/O block size:	%ld bytes\n", (long) sb.st_blksize);
	fprintf(stdout, "File size:					%lld bytes\n", (long long) sb.st_size);
	fprintf(stdout, "Blocks allocated:			%lld\n", (long long) sb.st_blocks);

	fprintf(stdout, "Last status change:		%s", ctime(&sb.st_ctime));
	fprintf(stdout, "Last file access:			%s", ctime(&sb.st_atime));
	fprintf(stdout, "Last file modification:	%s", ctime(&sb.st_mtime));

	return 0;
}

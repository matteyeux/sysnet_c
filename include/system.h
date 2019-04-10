#include <sys/sysinfo.h>

struct sysinfo sys_info;

void infosys(void);
int username(void);
int disk_info(const char *partition);
int diskstat(int argc, char *argv[]);
int raminfo(void);
int fileinfo(char *path2file);

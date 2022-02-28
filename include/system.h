#include <sys/sysinfo.h>

void infosys(void);
int username(void);
int disk_info(const char *partition);
int diskstat(int argc, char *argv[]);
int raminfo(void);
int fileinfo(char *path2file);

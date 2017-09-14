/*System functions*/
#include <sys/utsname.h>
#include <sys/statvfs.h>
struct utsname buf;

#ifdef linux
#include <sys/sysinfo.h>
struct sysinfo sys_info;
#endif

int typeOS();
int infosys();
int username();
void use_log();
int disk_info(const char *partition);
int diskstat(int argc, char *argv[]);
int raminfo();
void print_ram();
int fileinfo(char *path2file);

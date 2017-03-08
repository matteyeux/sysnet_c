/*System functions*/
#include <sys/utsname.h>
#include <sys/statvfs.h>
struct utsname buf;
#ifdef linux
struct sysinfo sys_info;
#endif
void print_ram();

int version();
int typeOS();
int infosys();
int username();
void use_log();
int disk_info(const char *partition);
int diskstat(int argc, char *argv[]);
int raminfo();
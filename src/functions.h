#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <math.h>

#ifdef linux
	#include <linux/if_link.h>
#endif

#define BUFSIZE 8192
/*Easier to understand*/
#define macosx	__APPLE__ && __MACH__
//#define linux __linux
#define windows	_WIN32

struct utsname buf;
//#ifdef linux
//struct sysinfo sys_info;
//#endif
void print_ram();
char gateway[255];

struct route_info {
    struct in_addr dstAddr;
    struct in_addr srcAddr;
    struct in_addr gateWay;
    char ifName[IF_NAMESIZE];
};


/*System functions*/
int version();
int typeOS();
int infosys();
int username();
void use_log();
int disk_info(const char *partition);
int diskstat(int argc, char *argv[]);
int raminfo();

/*Network functions*/
int hostname();
int get_broadcast(char *host_ip, char *netmask);
int network_info();
#ifdef linux
int readNlSock(int sockFd, char *bufPtr, int seqNum, int pId);
void printRoute(struct route_info *rtInfo);
void printGateway();
void parseRoutes(struct nlmsghdr *nlHdr, struct route_info *rtInfo);
int print_gateway();
#endif

/*CPU functions*/
int cpu_info(); 

#include <sys/utsname.h>
#include <sys/statvfs.h>

#include <netinet/in.h>
#include <net/if.h>

#ifdef linux
	#include <linux/if_link.h>
#endif

#define BUFSIZE 8192
/*Easier to understand*/
#define macosx	__APPLE__ && __MACH__
//#define linux __linux
#define windows	_WIN32

struct utsname buf;
struct sysinfo sys_info;
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
int Linux_kern();
int infosys();
int username();
void use_log();
void disk_infos();
int diskstat(int argc, char *argv[]);

/*Network functions*/
int hostname();
int network_infos();
#ifdef linux
int readNlSock(int sockFd, char *bufPtr, int seqNum, int pId);
void printRoute(struct route_info *rtInfo);
void printGateway();
void parseRoutes(struct nlmsghdr *nlHdr, struct route_info *rtInfo);
int print_gateway();
#endif
/*int public_IP();
int ssid();
int dns();
*/

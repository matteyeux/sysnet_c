/*Network functions*/
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <math.h>
#include <stdbool.h>

#ifdef linux
	#include <linux/if_link.h>
#endif

#define BUFSIZE 8192

char gateway[255];

struct route_info {
	struct in_addr dstAddr;
	struct in_addr srcAddr;
	struct in_addr gateWay;
	char ifName[IF_NAMESIZE];
};

int hostname();
int get_broadcast(char *host_ip, char *netmask);
char *get_mac_addr(char *interface);
int network_info(char *interface, int iface);
int find_wifi(char* iw_interface);
bool is_iface_up(const char *interface);
int up_iface(const char *interface);
#ifdef linux
int readNlSock(int sockFd, char *bufPtr, int seqNum, int pId);
void printRoute(struct route_info *rtInfo);
void printGateway();
void parseRoutes(struct nlmsghdr *nlHdr, struct route_info *rtInfo);
int print_gateway();
int check_wireless(const char* ifname, char* protocol);
char *get_wireless_iface(void);
#endif

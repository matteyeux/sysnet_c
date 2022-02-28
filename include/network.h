/*Network functions*/
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <math.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netdb.h>
#include <linux/if_link.h>

#define BUFSIZE 8192

typedef struct net_info_s net_info_t;
struct route_info {
	struct in_addr dstAddr;
	struct in_addr srcAddr;
	struct in_addr gateWay;
	char ifName[IF_NAMESIZE];
};


struct net_info_s {
	char *mac_addr;
	char *ip_addr;
	char *netmask;
	char *broadcast;
	int cidr;
};

typedef struct ifaddrs ifaddrs_t;

net_info_t *get_network_info(ifaddrs_t *ifaddr, char *interface, int ip_version);
void print_network_info(void);
char *get_hostname(void);
char *get_broadcast(char *host_ip, char *netmask);
char *get_mac_addr(char *interface);
int find_wifi(char* iw_interface);
bool is_iface_up(const char *interface);
int up_iface(const char *interface);
int readNlSock(int sockFd, char *bufPtr, int seqNum, int pId);
void printRoute(struct route_info *rtInfo);
void printGateway();
void parseRoutes(struct nlmsghdr *nlHdr, struct route_info *rtInfo);
int grab_gateway(void);
int check_wireless(const char* ifname, char* protocol);
char *get_wireless_iface(void);

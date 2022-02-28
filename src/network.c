#define _GNU_SOURCE		/* To get defines of NI_MAXSERV and NI_MAXHOST */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <netinet/in.h>
#include <net/if.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <errno.h>
#include <iwlib.h>
#include <linux/if_link.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#include <include/network.h>

char gateway[255];

char *get_hostname(void)
{
	char *host;

	host = malloc(sizeof(char) * 256);

	if (host == NULL) {
		fprintf(stderr, "[MALLOC] unable to allocate memory\n");
		return NULL;
	}

	host[255] = '\0';
	gethostname(host, 256);
	//fprintf(stdout, "hostname: %s\n", host);
	return (char *)host;
}

char *get_broadcast(char *host_ip, char *netmask)
{
	struct in_addr host, mask, broadcast;
	char broadcast_address[INET_ADDRSTRLEN];
	char *broadcast_ret;

	if (inet_pton(AF_INET, host_ip, &host) == 1 && inet_pton(AF_INET, netmask, &mask) == 1)
		broadcast.s_addr = host.s_addr | ~mask.s_addr;
	else {
		fprintf(stderr, "ERROR : %s\n", strerror(errno));
		return NULL;
	}

	if (inet_ntop(AF_INET, &broadcast, broadcast_address, INET_ADDRSTRLEN) != NULL) {
		broadcast_ret = broadcast_address;
		return broadcast_ret;
	} else {
		return NULL;
	}
}

char *get_mac_addr(char *interface){
	int fd;
	struct ifreq ifr;
	unsigned char *mac_digit = NULL;
	char *mac;

	mac = malloc(sizeof(char) *30);
	if (mac == NULL) {
		fprintf(stderr, "[MALLOC] unable to allocate memory\n");
		return NULL;
	}

	memset(&ifr, 0, sizeof(ifr));
	fd = socket(AF_INET, SOCK_DGRAM, 0);

	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name , interface , IFNAMSIZ-1);
	if (0 == ioctl(fd, SIOCGIFHWADDR, &ifr)) {
		mac_digit = (unsigned char *)ifr.ifr_hwaddr.sa_data;
		// if interface == "lo"; it prints -> mac : 00:00:00:00:00:00
		if (strcmp(interface, "lo") != 0)
			sprintf(mac, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X" , mac_digit[0], mac_digit[1], mac_digit[2], mac_digit[3], mac_digit[4], mac_digit[5]);
	}
	return (char *)mac;
}
/*
* only work for common netmasks
* for the moment
*/
int get_cidr(char *netmask)
{
	int cidr, i = 0;
	inet_pton(AF_INET, netmask, &cidr);

	while (cidr > 0) {
		cidr = cidr >> 1;
		i++;
	}

	return i;
}

net_info_t *get_network_info(ifaddrs_t *ifaddr, char *interface, int ip_version)
{
	struct ifaddrs *ifa;
	struct sockaddr *netmask;

	struct net_info_s *network_info = NULL;

	int family, s, n;
	char ip_address[NI_MAXHOST];
	char mask[NI_MAXHOST];

	// don't forget to free at some point
	network_info = malloc(sizeof(net_info_t));

	if (network_info == NULL) {
		fprintf(stderr, "[MALLOC] unable to allocate memory\n");
		return NULL;
	}

	network_info->mac_addr = get_mac_addr(interface);

	/*
	* Loop for a linked list which
	* consists of ifaddrs structures
	*/
	for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
		if (ifa->ifa_addr == NULL)
			continue;

		family = ifa->ifa_addr->sa_family;
		netmask = ifa->ifa_netmask;

		if (!strcmp(ifa->ifa_name, interface)) {

			if (family == AF_INET && (ip_version == 4 || ip_version == 0)) {
				s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
								ip_address, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
				if (s != 0) {
					fprintf(stderr, "getnameinfo() failed: %s\n", gai_strerror(s));
					exit(EXIT_FAILURE);
				}

				network_info->ip_addr = ip_address;
			} else if (family == AF_INET6 && (ip_version == 0 || ip_version == 6)) {
				s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in6),
								ip_address, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

				if (s != 0) {
					fprintf(stderr, "getnameinfo() failed: %s\n", gai_strerror(s));
					exit(EXIT_FAILURE);
				}

				network_info->ip_addr = ip_address;
			} else {}

			if (family == AF_INET && netmask != NULL) {
				s = getnameinfo(netmask, sizeof(struct sockaddr_in),
								mask, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

				network_info->netmask = mask;
				network_info->cidr = get_cidr(mask);
			}
		}
	}

	return network_info;
}

void print_network_info(void)
{
	int n;
	char *hostname;
	struct ifaddrs *ifaddr, *ifa;
	net_info_t *network;

	hostname = get_hostname();

	fprintf(stdout, "hostname : %s\n", hostname);

	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		exit(EXIT_FAILURE);
	}


	for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
		if (ifa->ifa_addr == NULL)
			continue;

		if (ifa->ifa_addr->sa_family == AF_INET){
			fprintf(stdout, "IPv4 %s\n", ifa->ifa_name);


			network = get_network_info(ifaddr, ifa->ifa_name, 4);

			fprintf(stdout, "\taddress : %s\n", network->ip_addr);
			fprintf(stdout, "\tnetmask : %s\t CIDR : %d\n", network->netmask, network->cidr);

			network->broadcast = get_broadcast(network->ip_addr, network->netmask);

			fprintf(stdout, "\tbroadcast : %s\n", network->broadcast);

		} else if (ifa->ifa_addr->sa_family == AF_INET6) {
			fprintf(stdout, "IPv6 %s\n", ifa->ifa_name);
			network = get_network_info(ifaddr, ifa->ifa_name, 6);
			fprintf(stdout, "\taddress : %s\n", network->ip_addr);
		} else {}

		if (ifa->ifa_addr->sa_family == AF_INET6 || ifa->ifa_addr->sa_family == AF_INET) {
			if (strcmp("lo", ifa->ifa_name))
				fprintf(stdout, "\thwaddr : %s\n", network->mac_addr);

			free(network->mac_addr);
			free(network);
		}

	}

	freeifaddrs(ifaddr);
	free(hostname);
}

bool is_iface_up(const char *interface) {
	struct ifreq ifr;
	int sock = socket(PF_INET6, SOCK_DGRAM, IPPROTO_IP);

	memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_name, interface);

	if (ioctl(sock, SIOCGIFFLAGS, &ifr) < 0) {
		perror("SIOCGIFFLAGS");
	}

	close(sock);
	return !!(ifr.ifr_flags & IFF_UP);
}

int up_iface(const char *interface)
{
	int sockfd;
	struct ifreq ifr;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if (sockfd < 0)
		return -1;

	memset(&ifr, 0, sizeof ifr);

	strncpy(ifr.ifr_name, interface, IFNAMSIZ);

	ifr.ifr_flags |= IFF_UP;
	ioctl(sockfd, SIOCSIFFLAGS, &ifr);
	return 0;
}

int readNlSock(int sockFd, char *bufPtr, int seqNum, int pId)
{
	struct nlmsghdr *nlHdr;
	int readLen = 0, msgLen = 0;

	do {
		/* Recieve response from the kernel */
		if ((readLen = recv(sockFd, bufPtr, BUFSIZE - msgLen, 0)) < 0) {
			perror("SOCK READ: ");
			return -1;
		}

		nlHdr = (struct nlmsghdr *) bufPtr;

		/* Check if the header is valid */
		if ((NLMSG_OK(nlHdr, readLen) == 0) || (nlHdr->nlmsg_type == NLMSG_ERROR)) {
			perror("Error in recieved packet");
			return -1;
		}

		/* Check if the its the last message */
		if (nlHdr->nlmsg_type == NLMSG_DONE) {
			break;
		} else {
			/* Else move the pointer to buffer appropriately */
			bufPtr += readLen;
			msgLen += readLen;
		}

		/* Check if its a multi part message */
		if ((nlHdr->nlmsg_flags & NLM_F_MULTI) == 0) {
			break;
		}
	} while ((nlHdr->nlmsg_seq != (unsigned int)seqNum) || (nlHdr->nlmsg_pid != (unsigned int)pId));

	return msgLen;
}

/* For printing the routes. */
void printRoute(struct route_info *rtInfo)
{
	char tempBuf[512];

	/* Print Destination address */
	if (rtInfo->dstAddr.s_addr != 0)
		strcpy(tempBuf,  inet_ntoa(rtInfo->dstAddr));
	else
		sprintf(tempBuf, "*.*.*.*\t");

	fprintf(stdout, "%s\t", tempBuf);

	/* Print Gateway address */
	if (rtInfo->gateWay.s_addr != 0)
		strcpy(tempBuf, (char *) inet_ntoa(rtInfo->gateWay));
	else
		sprintf(tempBuf, "*.*.*.*\t");

	fprintf(stdout, "%s\t", tempBuf);

	/* Print Interface Name*/
	fprintf(stdout, "%s\t", rtInfo->ifName);

	/* Print Source address */
	if (rtInfo->srcAddr.s_addr != 0)
		strcpy(tempBuf, inet_ntoa(rtInfo->srcAddr));
	else
		sprintf(tempBuf, "*.*.*.*\t");

	fprintf(stdout, "%s\n", tempBuf);
}

void parseRoutes(struct nlmsghdr *nlHdr, struct route_info *rtInfo)
{
	struct rtmsg *rtMsg;
	struct rtattr *rtAttr;
	int rtLen;

	rtMsg = (struct rtmsg *) NLMSG_DATA(nlHdr);

	if ((rtMsg->rtm_family != AF_INET) || (rtMsg->rtm_table != RT_TABLE_MAIN))
		return;

	rtAttr = (struct rtattr *) RTM_RTA(rtMsg);
	rtLen = RTM_PAYLOAD(nlHdr);
	for (; RTA_OK(rtAttr, rtLen); rtAttr = RTA_NEXT(rtAttr, rtLen)) {
		switch (rtAttr->rta_type) {
		case RTA_OIF:
			if_indextoname(*(int *) RTA_DATA(rtAttr), rtInfo->ifName);
			break;
		case RTA_GATEWAY:
			rtInfo->gateWay.s_addr= *(u_int *) RTA_DATA(rtAttr);
			break;
		case RTA_PREFSRC:
			rtInfo->srcAddr.s_addr= *(u_int *) RTA_DATA(rtAttr);
			break;
		case RTA_DST:
			rtInfo->dstAddr .s_addr= *(u_int *) RTA_DATA(rtAttr);
			break;
		}
	}

	if (rtInfo->dstAddr.s_addr == 0)
		sprintf(gateway, "%s", (char *) inet_ntoa(rtInfo->gateWay));

	return;
}

int grab_gateway(void)
{
	struct nlmsghdr *nlMsg;
	struct route_info *rtInfo;

	char msgBuf[BUFSIZE];

	int sock, len, msgSeq = 0;

	if ((sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE)) < 0)
		perror("Socket Creation: ");

	memset(msgBuf, 0, BUFSIZE);

	nlMsg = (struct nlmsghdr *) msgBuf;

	nlMsg->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));
	nlMsg->nlmsg_type = RTM_GETROUTE;

	nlMsg->nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST;
	nlMsg->nlmsg_seq = msgSeq++;
	nlMsg->nlmsg_pid = getpid();

	if (send(sock, nlMsg, nlMsg->nlmsg_len, 0) < 0) {
		fprintf(stderr, "write to socket failed...\n");
		return -1;
	}

	/* Read the response */
	len = readNlSock(sock, msgBuf, msgSeq, getpid());

	if (len < 0) {
		fprintf(stderr, "fead from socket failed...\n");
		return -1;
	}

	rtInfo = malloc(sizeof(struct route_info));

	for (; NLMSG_OK(nlMsg, len); nlMsg = NLMSG_NEXT(nlMsg, len)) {
		memset(rtInfo, 0, sizeof(struct route_info));
		parseRoutes(nlMsg, rtInfo);
	}

	free(rtInfo);
	close(sock);

	if (strcmp(gateway, "")!=0) {
		fprintf(stdout, "\nGateway : %s\n", gateway);
	}

	return 0;
}

int find_wifi(char* iw_interface){
	int sock, i = 0;
	wireless_scan_head head;
	wireless_scan *result;
	iwrange range;

	// open iw_socket
	sock = iw_sockets_open();

	// get metadata to use for scan */
	if (iw_get_range_info(sock, iw_interface, &range) < 0) {
		fprintf(stderr, "ERROR : %s\n", strerror(errno));
		return -1;
	}

	// scan
	if (iw_scan(sock, iw_interface, range.we_version_compiled, &head) < 0) {
		fprintf(stderr, "ERROR : %s\n", strerror(errno));
		return -1;
	}

	result = head.result;
	while (NULL != result) {
		printf("%s\n", result->b.essid);
		result = result->next; i++;
	}

	printf("found %d wifi networks\n", i);
	return 0;
}

int check_wireless(const char* ifname, char* protocol) {
	int sock = -1;
	struct iwreq pwrq;

	memset(&pwrq, 0, sizeof(pwrq));
	strncpy(pwrq.ifr_name, ifname, IFNAMSIZ);

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return 0;
	}

	if (ioctl(sock, SIOCGIWNAME, &pwrq) != -1) {
		if (protocol) strncpy(protocol, pwrq.u.name, IFNAMSIZ);
			close(sock);

		return 1;
	}

	close(sock);
	return 0;
}


char *get_wireless_iface(void) {
	struct ifaddrs *ifaddr, *ifa;
	char *wireless_iface;
	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		return NULL;
	}

	/* walk through linked list, maintaining head pointer so we can free list later */
	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		char protocol[IFNAMSIZ]  = {0};

		if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_PACKET)
			continue;

		if (check_wireless(ifa->ifa_name, protocol)) {
			wireless_iface = ifa->ifa_name;
			freeifaddrs(ifaddr);

			return (char *)wireless_iface;
		}
	}
	freeifaddrs(ifaddr);
	return NULL;
}

#define _GNU_SOURCE     /* To get defines of NI_MAXSERV and NI_MAXHOST */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <netinet/in.h>
#include <net/if.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <errno.h>
#ifdef linux
#include <iwlib.h>
#include <linux/if_link.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#endif

#include <include/network.h>


int hostname()
{
    char hostname[1024];
    hostname[1023] = '\0';
    gethostname(hostname, 1023);
    fprintf(stdout, "hostname: %s\n", hostname);
    return 0;
}

int get_broadcast(char *host_ip, char *netmask)
{   
    struct in_addr host, mask, broadcast;
    char broadcast_address[INET_ADDRSTRLEN];
    
    if (inet_pton(AF_INET, host_ip, &host) == 1 && inet_pton(AF_INET, netmask, &mask) == 1)
        broadcast.s_addr = host.s_addr | ~mask.s_addr;
    else {
        fprintf(stderr, "ERROR : %s\n", strerror(errno));
        return 1;
    }
    
    if (inet_ntop(AF_INET, &broadcast, broadcast_address, INET_ADDRSTRLEN) != NULL)
        fprintf(stdout, "\tbroadcast: %s\n", broadcast_address);
    else {
        fprintf(stderr, "ERROR : %s\n", strerror(errno));
        return 1;
    }
    return 0;
}   

int network_info(char *interface)
{
    struct ifaddrs *ifaddr, *ifa; 
    struct sockaddr *netmask;
    int family, s, n, i, suffix;
    char ip_address[NI_MAXHOST];
    char mask[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
        if (ifa->ifa_addr == NULL)
        continue;
        
        if  (interface != NULL && !strcmp(interface, "list")){
			for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) {
				if (ifa->ifa_addr == NULL)
					continue;
				if (ifa->ifa_addr->sa_family == AF_INET){
					printf("IPv4 %s\n", ifa->ifa_name);
				} else if (ifa->ifa_addr->sa_family == AF_INET6){
					printf("IPv6 %s\n", ifa->ifa_name);
				}else {}
			}
			break;
		}

        family = ifa->ifa_addr->sa_family;
        netmask = ifa->ifa_netmask;

        //printf("%s\n", ifa->ifa_name); //usefull for debug
        if (family == AF_INET) {
            s = getnameinfo(ifa->ifa_addr,
            (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6), ip_address, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            
            if (s != 0) {
                fprintf(stderr, "getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }


            if (interface == NULL)
            {
                fprintf(stdout, "%s\n\taddress: %s\n", ifa->ifa_name,ip_address);
            }
        }

        else if (family == AF_INET6) {
            s = getnameinfo(ifa->ifa_addr,
            (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6), ip_address, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            
            if (s != 0) {
                fprintf(stderr, "getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }
            if (interface != NULL)
            {
                if (!strcmp(interface, ifa->ifa_name))
                {
                    fprintf(stdout, "IPv6 %s\n\taddress: %s\n", ifa->ifa_name,ip_address);
                }
            } else {
                fprintf(stdout, "IPv6 %s\n\taddress: %s\n", ifa->ifa_name,ip_address);
            }
        }
        if(family == AF_INET && netmask != NULL)
        {   
            i = 0;
            s = getnameinfo(netmask, sizeof(struct sockaddr_in), mask, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            
            inet_pton(AF_INET, mask, &suffix);
            while (suffix > 0) {
                suffix = suffix >> 1;
                i++;
            }

            if (interface != NULL && !strcmp(interface, ifa->ifa_name))
            {
                fprintf(stdout, "IPv4 %s\n\taddress: %s\n", ifa->ifa_name,ip_address);
                fprintf(stdout, "\tnetmask: %s\t\tsuffix : %d\n", mask, i);
                get_broadcast(ip_address, mask);
            }
            if (interface == NULL)
            {
                fprintf(stdout, "\tnetmask: %s\t\tsuffix : %d\n", mask, i);
                get_broadcast(ip_address, mask);
            }
        }
    }
    freeifaddrs(ifaddr);
   return 0;
}

#ifdef linux
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
        if ((NLMSG_OK(nlHdr, readLen) == 0)
            || (nlHdr->nlmsg_type == NLMSG_ERROR)) {
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
    } while ((nlHdr->nlmsg_seq != seqNum) || (nlHdr->nlmsg_pid != pId));
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
    //printf("%s\n", inet_ntoa(rtInfo->dstAddr));

    if (rtInfo->dstAddr.s_addr == 0)
        sprintf(gateway, "%s", (char *) inet_ntoa(rtInfo->gateWay));
    //printRoute(rtInfo);

    return;
}

int print_gateway()
{
    struct nlmsghdr *nlMsg;
    struct rtmsg *rtMsg;
    struct route_info *rtInfo;
    char msgBuf[BUFSIZE];

    int sock, len, msgSeq = 0;

    if ((sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE)) < 0)
        perror("Socket Creation: ");

    memset(msgBuf, 0, BUFSIZE);

    nlMsg = (struct nlmsghdr *) msgBuf;
    rtMsg = (struct rtmsg *) NLMSG_DATA(nlMsg);

    nlMsg->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg)); 
    nlMsg->nlmsg_type = RTM_GETROUTE;   

    nlMsg->nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST;   
    nlMsg->nlmsg_seq = msgSeq++;
    nlMsg->nlmsg_pid = getpid();   

    if (send(sock, nlMsg, nlMsg->nlmsg_len, 0) < 0) {
        fprintf(stderr, "Write To Socket Failed...\n");
        return -1;
    }

/* Read the response */
    if ((len = readNlSock(sock, msgBuf, msgSeq, getpid())) < 0) {
        fprintf(stderr, "Read From Socket Failed...\n");
    return -1;
    }

    rtInfo = (struct route_info *) malloc(sizeof(struct route_info));

    for (; NLMSG_OK(nlMsg, len); nlMsg = NLMSG_NEXT(nlMsg, len)) {
        memset(rtInfo, 0, sizeof(struct route_info));
        parseRoutes(nlMsg, rtInfo);
    }
    free(rtInfo);
    close(sock);
    if (strcmp(gateway, "")!=0)
    {
        fprintf(stdout, "\nGateway : %s\n", gateway);
    }
    return 0;
}

int find_wifi(char* iw_interface){
	wireless_scan_head head;
	wireless_scan *result;
	iwrange range;
	int sock, i = 0;

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
#endif /* linux */
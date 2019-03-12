#include<stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <ifaddrs.h>
#include <sys/types.h>

#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <stdlib.h>

void printHostIp(void);
void printHostIp2(void);
void printHostIp3(void);
void printHostIp4(char *ens_dev, char **ip);
int main(int argc, char *argv[])
{
	printHostIp();
	printf("\n***************************\n\n");
	printHostIp2();
	printf("\n***************************\n\n");
	printHostIp3();
	printf("\n***************************\n\n");
	if (argc == 2) {
		printf("ens_dev = %s\n", argv[1]);
		char *ip = NULL;
		printHostIp4(argv[1], &ip);
		if (ip) {
			printf("ip is %s\naddr is :%p\n", ip,ip);
			free(ip);
		}
	}

	return 0;
}

void printHostIp()
{
	char hostName[255];
	char *ip;
	char **pptr;
	char str[32];
	struct hostent *hostinfo;
	gethostname(hostName, sizeof(hostName));
	hostinfo = gethostbyname(hostName);
	printf("Host name is: %s\n", hostinfo->h_name);
	for (pptr = hostinfo->h_aliases; *pptr != NULL; pptr++) {
		printf("  alias of host: %s\n", *pptr);
	}
	printf("Host addrtype is: %d\n", hostinfo->h_addrtype);
	printf("Host length is: %d\n", hostinfo->h_length);
	printf("Your all IP address are\n");
	switch (hostinfo->h_addrtype) {
	case AF_INET:
	case AF_INET6:
		pptr = hostinfo->h_addr_list;
		/* print all the aliases,inet_ntop() is called */
		for (; *pptr != NULL; pptr++) {
			inet_ntop(hostinfo->h_addrtype, *pptr, str,
				  sizeof(str));
			printf("\taddress: %s\n", str);
		}
		break;
	default:
		printf("unknown address type\n");
		break;
	}
}

void printHostIp2()
{
	char hostName[255];
	char *ip;
	struct hostent *hostinfo;
	gethostname(hostName, sizeof(hostName));
	hostinfo = gethostbyname(hostName);
	printf("Your all IP address are\n");
	int i = 0;
	for (i = 0; hostinfo->h_addr_list[i]; i++) {
		ip = inet_ntoa(*(struct in_addr *)
			       hostinfo->h_addr_list[i]);
		printf("%s\n", ip);
	}
}

void printHostIp3()
{
	struct ifaddrs *ifAddrStruct = NULL;
	void *tmpAddrPtr = NULL;
	getifaddrs(&ifAddrStruct);
	while (ifAddrStruct != NULL) {
		if (ifAddrStruct->ifa_addr->sa_family == AF_INET) {	// check it is IP4
			tmpAddrPtr = &((struct sockaddr_in *)
				       ifAddrStruct->ifa_addr)->sin_addr;
			char addressBuffer[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, tmpAddrPtr, addressBuffer,
				  INET_ADDRSTRLEN);
			printf("%s IP Address %s\n",
			       ifAddrStruct->ifa_name, addressBuffer);
		} else if (ifAddrStruct->ifa_addr->sa_family == AF_INET6) {	// check it is IP6
			tmpAddrPtr = &((struct sockaddr_in *)
				       ifAddrStruct->ifa_addr)->sin_addr;
			char addressBuffer[INET6_ADDRSTRLEN];
			inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer,
				  INET6_ADDRSTRLEN);
			printf("%s IP Address %s\n",
			       ifAddrStruct->ifa_name, addressBuffer);
		}
		ifAddrStruct = ifAddrStruct->ifa_next;

	}
}

void printHostIp4(char *ens_dev, char **ip)
{
	int sock_get_ip;
	char ipaddr[50];
	char *iptmp;
	struct sockaddr_in *sin;
	struct ifreq ifr_ip;
	if ((sock_get_ip = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("socket create failse...GetLocalIp!\n");
	}
	memset(&ifr_ip, 0, sizeof(ifr_ip));
	strncpy(ifr_ip.ifr_name, ens_dev, sizeof(ifr_ip.ifr_name) - 1);
	if (ioctl(sock_get_ip, SIOCGIFADDR, &ifr_ip) < 0) {
		printf("ioctl failure...GetLocalIp!\n");
	}
	sin = (struct sockaddr_in *) &ifr_ip.ifr_addr;
	strcpy(ipaddr, inet_ntoa(sin->sin_addr));

	printf("local ip:%s \n", ipaddr);
	iptmp = malloc(strlen(ipaddr));
	strcpy(iptmp, ipaddr);
	printf("local ip addr: %p\n",iptmp);
	close(sock_get_ip);
	*ip = iptmp;

}

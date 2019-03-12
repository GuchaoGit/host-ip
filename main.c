#include<stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <ifaddrs.h>
#include <sys/types.h>

void printHostIp(void);
void printHostIp2(void);
void printHostIp3(void);
int main(void)
{
	printHostIp();
	printf("\n***************************\n\n");
	printHostIp2();
	printf("\n***************************\n\n");
	printHostIp3();

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
			tmpAddrPtr =
			    &((struct sockaddr_in *)
			      ifAddrStruct->ifa_addr)->sin_addr;
			char addressBuffer[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, tmpAddrPtr, addressBuffer,
				  INET_ADDRSTRLEN);
			printf("%s IP Address %s\n",
			       ifAddrStruct->ifa_name, addressBuffer);

		} else if (ifAddrStruct->ifa_addr->sa_family == AF_INET6) {	// check it is IP6
			tmpAddrPtr =
			    &((struct sockaddr_in *)
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

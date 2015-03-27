#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <WS2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

int main(int argc, char **argv)
{
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

    struct addrinfo *res;
    char hostname[NI_MAXHOST];
    char addrstr[100];
    void *ptr;
    struct addrinfo hints;
    int error;

  memset (&hints, 0, sizeof (hints));
  hints.ai_family = PF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags |= AI_CANONNAME;


    printf("%s\n", argv[1]);
    if (error = getaddrinfo(argv[1], NULL, &hints, &res))
    //if (error = getaddrinfo(argv[1], NULL, NULL, &res))
    {
        printf("error using getaddrinfo: %s\n", gai_strerror(error));
    }

   /* if (res) {
        if (error = getnameinfo(res->ai_addr, sizeof(struct sockaddr), hostname, sizeof(hostname), NULL,0,0)) {
            printf("error using getnameinfo: %s\n", gai_strerror(error));
            return 0;
        }
    }*/


    while (res)
    {
      inet_ntop (res->ai_family, res->ai_addr->sa_data, addrstr, 100);

      switch (res->ai_family)
        {
        case AF_INET:
          ptr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
          break;
        case AF_INET6:
          ptr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
          break;
        }
      inet_ntop (res->ai_family, ptr, addrstr, 100);
      //printf ("getaddinfo test : IPv%d address: %s (%s)\n", res->ai_family == PF_INET6 ? 6 : 4, addrstr, res->ai_canonname);
      printf("%s(%s)\n", addrstr, res->ai_canonname);
      res = res->ai_next;
    }

    freeaddrinfo(res);
	WSACleanup();

}
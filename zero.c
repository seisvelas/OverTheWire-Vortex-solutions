/*
** Yeah I didn't want to relearn TCP in C so I just copypasta'd the client.c example
    client from Beej's Guide To Network Programming and modified it to read in unsigned ints. :)
    It's funny cuz normally you want employers to look through your Github and, you know, be wow'd by
    your prowess or whatever. But my Github repos are so fucking ugly haha, like if any employer saw
    this C code, sheesh. Imagine. But here's something to consider: IT FREAKING WORKS!!! YEESSSSSS
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <stdint.h>

uint32_t htonf(float f)
{
	uint32_t p;
	uint32_t sign;

	if (f < 0) { sign = 1; f = -f; }
	else { sign = 0; }
		
	p = ((((uint32_t)f)&0x7fff)<<16) | (sign<<31); // whole part and sign
	p |= (uint32_t)(((f - (int)f) * 65536.0f))&0xffff; // fraction

	return p;
}

float ntohf(uint32_t p)
{
	float f = ((p>>16)&0x7fff); // whole part
	f += (p&0xffff) / 65536.0f; // fraction

	if (((p>>31)&0x1) == 0x1) { f = -f; } // sign bit set

	return f;
}
#define PORT "5842" // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
	int sockfd, numbytes;  
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];

	if (argc != 2) {
	    fprintf(stderr,"usage: client hostname\n");
	    exit(1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			perror("client: connect");
			close(sockfd);
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof s);
	printf("client: connecting to %s\n", s);

	freeaddrinfo(servinfo); // all done with this structure
	unsigned int num1;
unsigned int num2;
unsigned int num3;
unsigned int num4;
	recv(sockfd, &num1, sizeof num1, 0);


	printf("client: received '%i'\n",(unsigned int)ntohf(num1));
	recv(sockfd, &num2, sizeof num2, 0);


	printf("client: received '%i'\n",(unsigned int)ntohf(num2));
	recv(sockfd, &num3, sizeof num3, 0);


	printf("client: received '%i'\n",(unsigned int)ntohf(num3));
	recv(sockfd, &num4, sizeof num4, 0);


	printf("client: received '%i'\n",(unsigned int)ntohf(num4));
	unsigned int hackpass = num1 + num2 + num3 + num4;
	send(sockfd, &hackpass, sizeof(unsigned int), 0);
	char hackinfo[5000];
	recv(sockfd, hackinfo,5000, 0);
	printf("%s\n", hackinfo);
	close(sockfd);

	return 0;
}

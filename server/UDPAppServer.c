/*
 * udp_endp.c
 *
 * a UDP endpoint demo application for use with WiShield UDPApp sketch
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define REMOTE_PORT		12344	// port on Arduino + WiShield
#define LOCAL_PORT		12345	// port on the PC

#define MAXDATASIZE		100

int main()
{
	int sockfd, numbytes;
	char buf[MAXDATASIZE] = "Hello";
	struct sockaddr_in remote, local;
	size_t addr_len;
	unsigned char buf_len;

	memset(&remote, 0, sizeof(remote));

	// setup IP address of WiShield
	remote.sin_family = AF_INET;
	remote.sin_port = htons(REMOTE_PORT);
	inet_pton(AF_INET, "192.168.1.2", &remote.sin_addr);

	// setup socket and connect
	if ((sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		perror("socket");
		exit(0);
	}

	// bind to a specific port on the PC
	local.sin_family = AF_INET;
	local.sin_port = htons(LOCAL_PORT);
	local.sin_addr.s_addr = INADDR_ANY;

	if ( bind(sockfd, (struct sockaddr *)&local, sizeof(local)) == -1) {
		perror("bind");
		exit(0);
	}

	// send "Hello" to WiShield
	if ((numbytes = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&remote, sizeof(remote))) == -1) {
		perror("sendto");
		exit(1);
	}
	buf[numbytes] = '\0';
	printf("%s\n", buf);

	// Wait for response
	addr_len = sizeof(remote);
	if ((numbytes = recvfrom(sockfd, buf, MAXDATASIZE-1 , 0, (struct sockaddr *)&remote, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}
	buf[numbytes] = '\0';
	printf("%s", buf);

	// Get name from user
	scanf("%s", buf);
	buf_len = strlen(buf);
	buf[buf_len] = '\n';

	// Send response to WiShield
	if ((numbytes = sendto(sockfd, buf, buf_len+1, 0, (struct sockaddr *)&remote, sizeof(remote))) == -1) {
		perror("sendto");
		exit(1);
	}

	// Wait for greeting
	addr_len = sizeof(remote);
	if ((numbytes = recvfrom(sockfd, buf, MAXDATASIZE-1 , 0, (struct sockaddr *)&remote, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}
	buf[numbytes] = '\0';
	printf("%s", buf);

	close(sockfd);

	return 0;
}

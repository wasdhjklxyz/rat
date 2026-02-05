#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include "socket.h"

int create_socket(uint16_t port)
{
	int sock, ret;
	struct sockaddr_in addr;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("socket");
		return -1;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;

	ret = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
	if (ret < 0) {
		perror("bind");
		close(sock);
		return -1;
	}

	ret = listen(sock, SOMAXCONN);
	if (ret < 0) {
		perror("listen");
		close(sock);
		return -1;
	}

	return sock;
}

int destroy_socket(int sock)
{
	int ret;

	if (sock < 0) {
		return -1;
	}

	ret = shutdown(sock, SHUT_RDWR);
	if (ret < 0) {
		perror("shutdown");
		close(sock);
		return -1;
	}

	ret = close(sock);
	if (ret < 0) {
		perror("close");
		return -1;
	}

	return 0;
}

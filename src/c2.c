#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

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

int main(void)
{
	int sk_listen, sk_accept, ret;
	ssize_t rd_bytes;
	char buf[64];

	sk_listen = create_socket(7777);
	if (sk_listen < 0) {
		return 1;
	}

	sk_accept = accept(sk_listen, NULL, NULL);
	if (sk_accept < 0) {
		perror("accept");
		close(sk_listen);
		return 1;
	}

	rd_bytes = read(sk_accept, buf, sizeof(buf));
	if (rd_bytes < 0) {
		perror("read");
		close(sk_accept);
		close(sk_listen);
		return 1;
	}

	printf("%s\n", buf);

	ret = destroy_socket(sk_listen);
	if (ret < 0) {
		return 1;
	}

	return 0;
}

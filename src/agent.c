#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#ifndef C2_PORT
#define C2_PORT 0
#endif // C2_PORT

#ifndef C2_HOST
#define C2_HOST "0.0.0.0"
#endif // C2_HOST

int create_socket(const char *host, uint16_t port)
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

	ret = inet_pton(AF_INET, host, &addr.sin_addr);
	if (ret <= 0) {
		perror("inet_pton");
		close(sock);
		return -1;
	}

	ret = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
	if (ret < 0) {
		perror("connect");
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
	int sk, ret;
	ssize_t wr_bytes;
	const char msg[] = "hello, world!";

	sk = create_socket(C2_HOST, C2_PORT);
	if (sk < 0) {
		return 1;
	}

	wr_bytes = write(sk, msg, strlen(msg));
	if (wr_bytes < 0) {
		perror("write");
		close(sk);
		return 1;
	}

	ret = destroy_socket(sk);
	if (ret < 0) {
		return 1;
	}

	return 0;
}

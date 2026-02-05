#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include "net.h"

int main(void)
{
	int sk_listen, sk_accept, ret;
	ssize_t rd_bytes;
	char buf[64];

	sk_listen = create_socket(C2_PORT);
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

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "socket.h"

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

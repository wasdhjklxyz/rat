#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>

#include "net.h"

#ifndef C2_PORT
#define C2_PORT 0
#endif

#define MAX_EVENTS 10
#define BUF_SIZE 1024 // 1KiB

static int create_socket(uint16_t port)
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

static int destroy_socket(int sock)
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

static int set_nonblocking(int fd)
{
	int fl, ret;

	if (fd < 0) {
		return -1;
	}

	fl = fcntl(fd, F_GETFL);
	if (fl < 0) {
		perror("fcntl");
		return -1;
	}

	ret = fcntl(fd, F_SETFL, fl | O_NONBLOCK);
	if (ret < 0) {
		perror("fcntl");
		return -1;
	}

	return 0;
}

static int close_sk_conn(int epfd, int sk_conn)
{
	int ret;

	ret = epoll_ctl(epfd, EPOLL_CTL_DEL, sk_conn, NULL);
	if (ret < 0) {
		perror("epoll_ctl");
		return 1;
	}

	ret = close(sk_conn);
	if (ret < 0) {
		perror("close");
		return 1;
	}

	return 0;
}

int net_start_event_loop(void)
{
	/* FIXME: This function wayyyyy to long */
	int sk_listen, sk_conn, epfd, nfds, n, ret;
	struct epoll_event ev, events[MAX_EVENTS];
	ssize_t rd_bytes;
	char buf[BUF_SIZE];

	sk_listen = create_socket(C2_PORT);
	if (sk_listen < 0) {
		return -1;
	}

	epfd = epoll_create1(0);
	if (epfd < 0) {
		perror("epoll_create1");
		close(sk_listen);
		return -1;
	}

	ev.events = EPOLLIN;
	ev.data.fd = sk_listen;

	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sk_listen, &ev);
	if (ret < 0) {
		perror("epoll_ctl");
		goto err;
	}

	while (1) {
		nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
		if (nfds < 0) {
			perror("epoll_wait");
			goto err;
		}

		for (n = 0; n < nfds; n++) {
			if (events[n].data.fd == sk_listen) {
				sk_conn = accept(sk_listen, NULL, NULL);
				if (sk_conn < 0) {
					perror("accept");
					goto err;
				}

				ret = set_nonblocking(sk_conn);
				if (ret < 0) {
					close(sk_conn);
					goto err;
				}

				ev.events = EPOLLIN;
				ev.data.fd = sk_conn;

				ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sk_conn,
						&ev);
				if (ret < 0) {
					perror("epoll_ctl");
					close(sk_conn);
					goto err;
				}
			} else {
				sk_conn = events[n].data.fd;

				if (!(events[n].events &
				      EPOLLIN)) { // NOTE: Not avail to read
					if (events[n].events &
					    (EPOLLHUP | EPOLLERR)) {
						close_sk_conn(epfd, sk_conn);
						if (ret < 0) {
							close(sk_conn);
							goto err;
						}
					}
					continue;
				}

				errno = 0;
				rd_bytes = read(sk_conn, buf, BUF_SIZE);
				if (rd_bytes <= 0) {
					if (rd_bytes == 0) {
						close_sk_conn(epfd, sk_conn);
						continue;
					} else if (errno == EAGAIN ||
						   errno == EWOULDBLOCK) {
						continue;
					} else {
						close_sk_conn(epfd, sk_conn);
						goto err;
					}
				}
				printf("%s\n", buf);
			}
		}
	}

	close(epfd);

	ret = destroy_socket(sk_listen);
	if (ret < 0) {
		return -1;
	}

	return 0;
err:
	close(epfd);
	close(sk_listen);
	return -1;
}

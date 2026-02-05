#ifndef RAT_AGENT_SOCK_H
#define RAT_AGENT_SOCK_H

#ifndef C2_PORT
#define C2_PORT 0
#endif // C2_PORT

#ifndef C2_HOST
#define C2_HOST "0.0.0.0"
#endif // C2_HOST

#include <stdint.h>

int create_socket(const char *host, uint16_t port);
int destroy_socket(int sock);

#endif // RAT_AGENT_SOCK_H

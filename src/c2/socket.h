#ifndef RAT_C2_SOCKET_H
#define RAT_C2_SOCKET_H

#ifndef C2_PORT
#define C2_PORT 0
#endif

#include <stdint.h>

int create_socket(uint16_t port);
int destroy_socket(int sock);

#endif // RAT_C2_SOCKET_H

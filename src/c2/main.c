#include "net.h"

int main(void)
{
	int ret;

	ret = net_start_event_loop();
	if (ret < 0) {
		return 1;
	}

	return 0;
}

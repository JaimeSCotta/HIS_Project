#include "shell.h"
#include "msg.h"
int main(void)
{
	msg_t msgq[8];
//	(void) msgq;
	msg_init_queue(msgq, 8);
	static char buf [SHELL_DEFAULT_BUFSIZE];
	shell_run_forever(NULL, buf, sizeof(buf));
	return 0;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// libraries added
#include "time.h"
#include <xtimer.h>

#include "shell.h"
#include "msg.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

// Modules added: 
#include "sensordefinition.h"
#include "cmd_start.h"
#include "emcute_thread_header.h"
#include "pub.h"
#include "discon.h"
#include "gen_sensors_values.h"
#include "con.h"
//Test 1
#define EMCUTE_PORT (1883U)
#define EMCUTE_PRIO (THREAD_PRIORITY_MAIN - 1)
//End Test 1
static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static const shell_command_t shell_commands[] = {
    {"start", "Start the station", cmd_start},
    {NULL, NULL, NULL}};

int main(void)
{
    puts("MQTT-SN application\n");
    puts("Type 'help' to get started. Have a look at the README.md for more "
         "information.");

    /* the main thread needs a msg queue to be able to run `ping6`*/
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* start the emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* start shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    /* should be never reached */
    return 0;
}

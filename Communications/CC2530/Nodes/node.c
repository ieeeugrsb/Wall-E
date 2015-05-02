/*
    node.c - Client firmware for CC2530 microcontroller.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITN+ESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"

#include <string.h>
#include <stdio.h>
#include "dev/leds.h"
#include "dev/serial-line.h"

#define MAX_COMMAND_LEN 100
static char command[MAX_COMMAND_LEN];
static uint16_t command_len;

#define LOCAL_PORT 3000
static struct uip_udp_conn *conn;

/* Declare the process and select it for autostart */
PROCESS(node_process, "Main process for a node");
AUTOSTART_PROCESSES(&node_process);

/* --------------------------------------------------------------- */
/* UDP/IP handler event                                            */
/* --------------------------------------------------------------- */
static void tcpip_handler()
{
    // Check that what we have received is a new packet
    if (!uip_newdata())
        return;

    // Clean buffer
    memset(command, 0, MAX_COMMAND_LEN);

    // Get the data
    command_len = uip_datalen();
    memcpy(command, uip_appdata, command_len);

    // Send the command by a serial line
    printf("%s", command);
}

/* --------------------------------------------------------------- */
/* Serial intput handler event                                     */
/* --------------------------------------------------------------- */
static void input_handler(char* input)
{
    // TODO
}

/* --------------------------------------------------------------- */
/* Network configuration                                           */
/* --------------------------------------------------------------- */
static void network_config()
{
    uip_ipaddr_t ipaddr;

    // Set the remote IP to stablish a "UDP connection".
    uip_ip6addr(&ipaddr, 0xAAAA, 0, 0, 0, 0x0212, 0x4b00, 0x02cb, 0x0f32);
    ushort16_t remote_port = 3000;

    // Create the "connection" so we receive package only from that address
    conn = udp_new(&ipaddr, UIP_HTONS(remote_port), NULL);
    if (!conn)
        return;

    // Bind the "connection" to a local port
    udp_bin(conn, UIP_HTONS(LOCAL_PORT));
}

/* --------------------------------------------------------------- */
/* Main Process                                                    */
/* --------------------------------------------------------------- */
PROCESS_THREAD(node_process, ev, data)
{
    // All the process start with this
    PROCESS_BEGIN();

    // Configure the network
    network_config();
    if (!conn) {
        printf("E01\n");
        PROCESS_EXIT();
    }

    // Main, infinite, loop of the process
    while (1) {
        // Wait, block the process, until an event happens.
        // Meanwhile, other process will continue running.
        PROCESS_WAIT_EVENT();

        // Check the type of event that unblock the process
        if (ev == tcpip_event)
            tcpip_handler();
        else if (ev == serial_line_event_message)
            input_handler((char*)data);
    }

    // All the process ends with this
    PROCESS_END();
}

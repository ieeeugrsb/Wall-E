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

#define DEBUG DEBUG_PRINT
#include "net/uip-debug.h"

#define UIP_IP_BUF   ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])
#define UIP_UDP_BUF  ((struct uip_udp_hdr *)&uip_buf[uip_l2_l3_hdr_len])

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
    // "putstring" does not depend of the debug verbosity level
    printf("%s\n", command);

    // Debug messages
    PRINTF("# Recevied %u bytes from [", command_len);
    PRINT6ADDR(&UIP_IP_BUF->srcipaddr);
    PRINTF("]:%u\n", UIP_HTONS(UIP_UDP_BUF->srcport));
}

/* --------------------------------------------------------------- */
/* Serial intput handler event                                     */
/* --------------------------------------------------------------- */
static void input_handler(char* input)
{
    // If we haven't be able to reach the DAG RPL Router give error
    if (uip_ds6_get_global(ADDR_PREFERRED) == NULL) {
        printf("E02\n");
        return;
    }

    // Send the data with magic
    uip_udp_packet_send(conn, input, strlen(input));

    // Debug messages
    PRINTF("# %s\n", input);
    PRINTF("# Sent %u bytes to [", strlen(input));
    PRINT6ADDR(&conn->ripaddr);
    PRINTF("]:%u\n", UIP_HTONS(conn->rport));
}

/* --------------------------------------------------------------- */
/* Print our IPs and set them as final                             */
/* --------------------------------------------------------------- */
static void check_local_addresses()
{
    int i;
    uint8_t state;

    PRINTF("# IPv6 addresses:\n");
    for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
        state = uip_ds6_if.addr_list[i].state;
        if(uip_ds6_if.addr_list[i].isused &&
           (state == ADDR_TENTATIVE || state == ADDR_PREFERRED)) {

            PRINTF("# ");
            PRINT6ADDR(&uip_ds6_if.addr_list[i].ipaddr);
            PRINTF("\n");

            // Hack to make address "final"
            if (state == ADDR_TENTATIVE)
                uip_ds6_if.addr_list[i].state = ADDR_PREFERRED;
        }
    }
}

/* --------------------------------------------------------------- */
/* Network configuration                                           */
/* --------------------------------------------------------------- */
static void network_config()
{
    uip_ipaddr_t ipaddr;
    uint16_t rport = 3000;

    // Add our global IP address appending our MAC.
    uip_ip6addr(&ipaddr, 0xAAAA, 0, 0, 0, 0, 0, 0, 0);
    uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr);
    uip_ds6_addr_add(&ipaddr, 0, ADDR_AUTOCONF);

    // Print and check the state of our IPs
    check_local_addresses();

    // Set the remote IP.
    uip_ip6addr(&ipaddr, 0xAAAA, 0, 0, 0, 0x0212, 0x4b00, 0x02cb, 0x0f32);

    // Create the "connection" so we receive package only from that address
    conn = udp_new(&ipaddr, UIP_HTONS(rport), NULL);
    if (!conn)
        return;

    // Bind the "connection" to a local port
    udp_bind(conn, UIP_HTONS(LOCAL_PORT));
}

/* --------------------------------------------------------------- */
/* Main Process                                                    */
/* --------------------------------------------------------------- */
PROCESS_THREAD(node_process, ev, data)
{
    // All the process start with this
    PROCESS_BEGIN();
    PRINTF("# Starting...\n");

    // Configure the network
    network_config();
    if (!conn) {
         printf("E01\n");
         PROCESS_EXIT();
    }

    // Main, infinite, loop of the process
    PRINTF("# Ready!\n");
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

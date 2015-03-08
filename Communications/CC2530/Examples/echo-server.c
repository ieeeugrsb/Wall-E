/*
    echo-server.c - Firmware example for CC2530 microcontroller.

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
#include "contiki-net.h"
#include "dev/leds.h"
#include <stdio.h>
#include <string.h>

/* Declare connection socket */
static struct psock socket;

/* Buffer of incoming data */
static char buffer[50];

/* -------------------------------------------------------------------------- */
/* Socket Process */
/* -------------------------------------------------------------------------- */
static PT_THREAD(handle_connection(struct psock *p))
{
    // So, as always let's start
    PSOCK_BEGIN(p);

    // Let's send a hello world
    PSOCK_SEND_STR(p, "Welcome, please type something and press return.\n");

    // Let's block this thread untile a new line is received
    PSOCK_READTO(p, '\n');

    // And what we receive, we send it back, MUAHAHA
    PSOCK_SEND_STR(p, "Got the following data: ");
    PSOCK_SEND(p, buffer, PSOCK_DATALEN(p));
    PSOCK_SEND_STR(p, "Good bye!\r\n");

    // Finally close the socket
    PSOCK_CLOSE(p);

    // The End...
    PSOCK_END(p);
}

/* Now, let's declare the main process */
PROCESS(example_psock_server_process, "Example protosocket server");
AUTOSTART_PROCESSES(&example_psock_server_process);

/* -------------------------------------------------------------------------- */
/* Main Process */
/* -------------------------------------------------------------------------- */
PROCESS_THREAD(example_psock_server_process, ev, data)
{
    // The Start...
    PROCESS_BEGIN();

    // So, here we configure the network listening at a TCP port.
    tcp_listen(UIP_HTONS(12345));

    while (1) {
        // Wait a TCP / IP event...
        PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);

        // If someone has connected, let's init the connection
        if (uip_connected()) {
            // Bind the buffer to the socket
            PSOCK_INIT(&socket, buffer, sizeof(buffer));

            // So now, let's keep the connection open waiting for something
            while (!(uip_aborted() || uip_closed() || uip_timedout())) {
                // Wait for an TCIP / IP event
                PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);

                // And then, let's send to handle the data
                handle_connection(&socket);
            }
        }
    }

    // ... the End
    PROCESS_END();
}

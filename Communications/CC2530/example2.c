/*
    example2.c - Firmware example for CC2530 microcontroller.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this file.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "contiki.h"
#include "dev/leds.h"
#include <stdio.h>

/* Application specific event value */
static process_event_t event_data_ready;

/* Let's declare the processes */
PROCESS(temp_process, "Temperature process");
PROCESS(print_process, "Print process");

/* And start them automatically */
AUTOSTART_PROCESSES(&temp_process, &print_process);

/* -------------------------------------------------------------------------- */
/* Temperature Process */
/* -------------------------------------------------------------------------- */
PROCESS_THREAD(temp_process, ev, data)
{
    // Static variables to keep unchanged between calls
    static struct etimer timer;
    static int count = 0;
    static int average, valid_measure;

    // But this one, as are recalculated every time, not need to be static
    int measure;
    uint8_t msb, lsb;

    // LET'S START THE PARTY
    PROCESS_BEGIN();

    // allocate the event
    event_data_ready = process_alloc_event();

    // Initialize the temperature sensor
    // TODO: It depends on the driver
    average = 0;

    // so, let's set to read periocally
    etimer_set(&timer, CLOCK_CONF_SECOND / 4);

    while (1) {
        // Wait our turn
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

        // Yeah, colors!
        leds_toggle(LEDS_BLUE);

        // You know, depends on the driver
        msb = 0; // read MSB
        lsb = 0; // read LSB

        // Let's do some calcs
        measure = ((uint16_t)msb) << 2;
        measure += (lsb >> 6) & 0x03;

        average += measure;
        count++;

        if (count == 4) {
            // Enough data, let's communicate the average
            valid_measure = average >> 2;
            average = 0;
            count = 0;

            // Post an event for printing
            process_post(&print_process, event_data_ready, &valid_measure);
        }

        // Reset timer
        etimer_reset(&timer);
    }

    // IT'S THE END
    PROCESS_END();
}

/* -------------------------------------------------------------------------- */
/* Printing Process */
/* -------------------------------------------------------------------------- */
PROCESS_THREAD(print_process, ev, data)
{
    // No variables need, all the info is in data
    // So let's start
    PROCESS_BEGIN();

    while (1) {
        // Wait for data
        PROCESS_WAIT_EVENT_UNTIL(ev == event_data_ready);

        // So now, display it (with ugly hack for decimals)
        printf("temperature = %u.%u\n", (*(int*)data) >> 2,
               ((*(int*)data) & 0x03) * 25);
    }

    // The end...
    PROCESS_END();
}

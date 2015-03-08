/*
    example1.c - Firmware example for CC2530 microcontroller.

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

/* First declare the processes */
PROCESS(hello_world_process, "Hello world process");
PROCESS(blink_process, "LED blink process");

/* Select the processes that start automatically */
AUTOSTART_PROCESSES(&hello_world_process, &blink_process);

/* -------------------------------------------------------------------------- */
/* Hello World Process */
/* -------------------------------------------------------------------------- */
PROCESS_THREAD(hello_world_process, ev, data)
{
    // Declared as static to keep values between kernel calls.
    static struct etimer timer;
    static int count = 0;

    // All the process start with this
    PROCESS_BEGIN();

    // Configure timer for one second per event (wait_event call)
    etimer_set(&timer, CLOCK_CONF_SECOND);
    while (1) {
        // Wait for an event (only timer events in this case)
        PROCESS_WAIT_EVENT();

        if (ev == PROCESS_EVENT_TIMER) {
            // do something, like a printf
            count++;

            // reset the timer
            etimer_reset(&timer);
        }
    }

    // All the process ends with this
    PROCESS_END();
}

/* -------------------------------------------------------------------------- */
/* LED Blink Process */
/* -------------------------------------------------------------------------- */
PROCESS_THREAD(blink_process, ev, data)
{
    // Again, static vars for multiple kernel calls
    static struct etimer timer;
    static uint8_t leds_state = 0;

    // Let's start
    PROCESS_BEGIN();

    while (1) {
        // Now, instead of set before and reset each time, let's set each time.
        etimer_set(&timer, CLOCK_CONF_SECOND / 4);

        // And we can't wait for a single event
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

        // So now, it's our turns, let's update the leds state
        leds_off(0xFF);
        leds_on(leds_state);
        leds_state += 1;
    }

    // Bye, bye
    PROCESS_END();
}

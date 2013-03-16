/*
 Copyright (C) 2013 John Anthony

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <msp430g2211.h>
#include <stdint.h>

#define LENGTH(x) (sizeof(x) / sizeof(x[0]))
#define SWITCH BIT0

static void display_init(void);
static void roll_die(void);

int main(void) {
    /*
     * Disable the chip's watchdog timer, which would otherwise put
     * the chip into sleep mode after a while if we infinite loop.
     * When you know how to wait properly (i.e. in a non-busy way) you
     * should re-enable this
     */
    WDTCTL = WDTPW + WDTHOLD;
    display_init(); /* See function description */

    /* Busy while loop. This is terrible but simple */
    while (1) {
        if ((P1IN & SWITCH) == 0)
            roll_die();
    }
}

static void
display_init(void) {
    P1OUT = 0; /* Turn off all LEDs until the button is pressed */
    /*
     * Set P1 (port 1) pins 1 through 7 as output ports (we're using
     * one each for our LEDs) but leaving BIT0 (which represents
     * pin 0) "off", which means that we are using it for input. It's
     * where we have connected our momentary switch
     */
    P1DIR  = 0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7;
}

/*
 * Our main lifting function. It is called when the button is pressed
 * and returns when the button is released, setting LEDs to a loading
 * "thumper" along the way and finally to a random dice display
 */
static void
roll_die(void) {
    /*
     * Think of these as representations of each number. The first
     * entry is just the middle LED (represents 1), the second entry
     * for two diagonally opposite LEDs (represents 2) etc.
     */
    static const uint8_t dice_displays[] = {
        BIT4,
        BIT3 | BIT5,
        BIT3 | BIT4 | BIT5,
        BIT1 | BIT3 | BIT5 | BIT7,
        BIT1 | BIT3 | BIT4 | BIT5 | BIT7,
        BIT1 | BIT2 | BIT3 | BIT5 | BIT6 | BIT7
    };
    /*
     * "Thumper" aka waiting display and index 'i'. It literally just
     * lights up LEDs in a circular order and we run through it while
     * the button is held down to show the user something is happening
     * That, plus it looks good.
     */
    static const uint8_t thump_displays[] = {
        BIT5,
        BIT6,
        BIT7,
        BIT3,
        BIT2,
        BIT1
    };
    uint16_t n; /* Index for our random number */
    uint16_t i; /* Index for our thumper position */

    /*
     * Loop continuously while the button (attached to P1 BIT0 aka
     * P1.0) is held down. This works to effectively give us a random
     * value for n based upon the length of time the switch is held
     * down for low tolerances of "random." We only need a low
     * tolerance, though, because our range is so small (1-6)
     */
    for (n = 0, i = 0; (P1IN & SWITCH) == 0; ++n) {
        /*
         * Every 6000 ticks we move our "thumper" one step, which
         * is our pretty display that flashes around while the button
         * is held down
         */
        if (n % 600 == 0) {
            i += 1;
            if (i >= LENGTH(thump_displays))
                i = 0;
            P1OUT = thump_displays[i];
        }
    }

    /*
     * Treat n just like a random number - that loop will have been
     * running so fast that this is as good as a random selection
     */
    n %= LENGTH(dice_displays);

    /*
     * Finally, set the P1 output pins according to a pattern held
     * for the appropriate number
     */
    P1OUT = dice_displays[n];
}

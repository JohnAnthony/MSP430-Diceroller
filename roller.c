#include <msp430g2211.h>
#include <stdint.h>

#define LENGTH(x) (sizeof(x) / sizeof(x[0]))
#define SWITCH BIT0

static void display_init(void) {
    P1DIR  = BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7;
}

static void roll_die(void) {
    /* Dice rolls 1 to 6 and index, 'n'*/
    static const uint8_t dice_displays[] = {
        BIT4,
        BIT3 | BIT5,
        BIT3 | BIT4 | BIT5,
        BIT1 | BIT3 | BIT5 | BIT7,
        BIT1 | BIT3 | BIT4 | BIT5 | BIT7,
        BIT1 | BIT2 | BIT3 | BIT5 | BIT6 | BIT7
    }; 
    uint16_t n;
    /* "Thumper" aka waiting display and index 'i' */
    static const uint8_t thump_displays[] = {
        BIT5,
        BIT6,
        BIT7,
        BIT3,
        BIT2,
        BIT1
    };
    uint16_t i;

    for (n = 0, i = 0; (P1IN & SWITCH) == 0; ++n) {
        if (n == 6000) {
            n = 0;
            i += 1;
            if (i >= LENGTH(thump_displays))
                i = 0;
            P1OUT = thump_displays[i];
        }
    }

    n %= LENGTH(dice_displays);
    P1OUT = dice_displays[n];
}

static void wait(uint16_t d) {
    for (; d>0; d--)
        nop();
}

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;
    display_init();

    /* Initially don't show anything until a roll is made */
    P1OUT = 0;       

    while (1) {
        if ((P1IN & SWITCH) == 0)
            roll_die();
        // wait(0x700);
    }
}

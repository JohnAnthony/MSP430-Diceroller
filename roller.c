#include <msp430g2211.h>

#define LENGTH(x) (sizeof(x) / sizeof(x[0]))

/* How to display the individual results */
static unsigned int DICE_1 = BIT3;
static unsigned int DICE_2 = BIT1 | BIT5;
static unsigned int DICE_3 = BIT1 | BIT3 | BIT5;
static unsigned int DICE_4 = BIT0 | BIT1 | BIT5 | BIT6;
static unsigned int DICE_5 = BIT0 | BIT1 | BIT3 | BIT5 | BIT6;
static unsigned int DICE_6 = BIT0 | BIT1 | BIT2 | BIT4 | BIT5 | ;

/* The order in which to display the results */
static unsigned int DICE_DISPLAYS[] = {
    DICE_1, DICE_2, DICE_3, DICE_4, DICE_5, DICE_6
};

/* Busy wait */
/* TODO: Use a proper timer wait */
static void wait(unsigned int d) {
    for (; d>0; d--)
        nop();
}

int main(void) {
    unsigned int p1values;
    unsigned int dice;

    WDTCTL = WDTPW + WDTHOLD;
    P1DIR  = BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6;

    dice = 0;
    P1OUT = 0;       /* Initially don't show anything until a roll is made */
    while (1) {
        while (!(P1IN & BIT7)) {
            dice++;
            if (dice >= LENGTH(DICE_DISPLAYS))
                dice = 0;
            P1OUT = DICE_DISPLAY[dice];
            wait(0x2000);
        }
        wait(0x8000);
    }
}

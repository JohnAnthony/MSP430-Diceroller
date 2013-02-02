#include <msp430g2211.h>

#define LENGTH(x) (sizeof(x) / sizeof(x[0]))

static unsigned int DICE_VALUE;
static unsigned int DICE_DISPLAYS[] = {
    BIT3,                                       /* One */
    BIT1 | BIT5,                                /* Two */
    BIT1 | BIT3 | BIT5,                         /* Three */
    BIT0 | BIT1 | BIT5 | BIT6,                  /* Four */
    BIT0 | BIT1 | BIT3 | BIT5 | BIT6,           /* Five */
    BIT0 | BIT1 | BIT2 | BIT4 | BIT5 | BIT6     /* Six */
};

static unsigned int dice_inc(void) {
    DICE_VALUE++;
    if (DICE_VALUE >= LENGTH(DICE_DISPLAYS))
        DICE_VALUE = 0;
    return DICE_DISPLAYS[DICE_VALUE];
}

/* Busy wait */
/* TODO: Use signals */
static void wait(unsigned int d) {
    for (; d>0; d--)
        nop();
}

int main(void) {
    int p1values;

    WDTCTL = WDTPW + WDTHOLD;
    P1DIR  = BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6;

    p1values = 0;    /* Initially don't show anything until a roll is made */
    DICE_VALUE = -1; /* This way it wraps immediately and starts at a value of
                       1 when the button is first pressed */
    while (1) {
        while (!(P1IN & BIT7)) {
            P1OUT = p1values = dice_inc();
            wait(0x2fff);
        }
    }
}

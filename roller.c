#include <msp430g2211.h>

#define LENGTH(x) (sizeof(x) / sizeof(x[0]))

static unsigned int DICE_DISPLAYS[] = {
    BIT3,                                       /* One */
    BIT1 | BIT5,                                /* Two */
    BIT1 | BIT3 | BIT5,                         /* Three */
    BIT0 | BIT1 | BIT5 | BIT6,                  /* Four */
    BIT0 | BIT1 | BIT3 | BIT5 | BIT6,           /* Five */
    BIT0 | BIT1 | BIT2 | BIT4 | BIT5 | BIT6     /* Six */
};

/* Busy wait */
/* TODO: Use signals */
void wait(unsigned int d) {
    for (; d>0; d--)
        nop();
}

int main(void) {
    int dicevalue;

    WDTCTL = WDTPW + WDTHOLD;
    P1DIR  = BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6;
    P2DIR  = 0x01;

    dicevalue = 0;
    while (1) {
        P1OUT = DICE_DISPLAYS[dicevalue];

        /* Literally just roll around the values to show each is correct */
        dicevalue++;
        if (dicevalue >= LENGTH(DICE_DISPLAYS))
            dicevalue = 0;

        wait(0xffff);
        wait(0xffff);
        wait(0xffff);
    }
}

#include <lab/servo/servo.h>

void servo_init()
{
    // Timer 1B, GPIO PB5
    SYSCTL_RCGCGPIO_R |= 0b10;      // Page 340
    SYSCTL_RCGCTIMER_R |= 0b10;     // Page 405

    while (SYSCTL_PRGPIO_R & 0b10 == 0) {}
    while (SYSCTL_PRTIMER_R & 0b10 == 0) {}

    GPIO_PORTB_DIR_R |= 0b100000;   // Page 663
    GPIO_PORTB_DEN_R |= 0b100000;
    GPIO_PORTB_AFSEL_R |= 0b100000; // Page 672
    GPIO_PORTB_PCTL_R = ((GPIO_PORTB_PCTL_R & ~0xF00000) | 0x700000);   // Page 689

    TIMER1_CTL_R &= ~0b100000000;   // Page 737
    TIMER1_CFG_R = ((TIMER1_CFG_R & ~0b111) | 0b100);   // Page 727
    TIMER1_TBMR_R |= 0b1000;
    TIMER1_TBMR_R &= ~0b100;
    TIMER1_TBMR_R = ((TIMER1_TBMR_R & ~0b11) | 0x2);
    TIMER1_CTL_R &= ~0b100000000000000;

    // Count down
    TIMER1_TBMR_R &= ~0b10000;
}

void servo_move(uint16_t degrees)
{
    static uint16_t previousDegrees = 90;

    // Stop the timer
    TIMER1_CTL_R &= ~0b100000000;   // Page 737

    // y = 800/9 * x + 1600
    // Number of clock pulses
    unsigned long waitTime = 320000;
    unsigned long pulses = (((float)800 / 9) * degrees) + 16000 + waitTime;

    // Load that value into the timers
    TIMER1_TBPR_R &= ~0xFF;
    TIMER1_TBPR_R |= (pulses >> 16);

    TIMER1_TBILR_R &= ~0xFFFF;
    TIMER1_TBILR_R |= (pulses & 0xFFFF);

    // Stop at waitTime
    TIMER1_TBMATCHR_R &= ~0xFFFF;
    TIMER1_TBMATCHR_R |= (waitTime & 0xFFFF);

    TIMER1_TBPMR_R &= ~0xFF;
    TIMER1_TBPMR_R |= (waitTime >> 16);

    // Start the timer
    TIMER1_CTL_R |= 0b100000000;   // Page 737

    uint32_t delay = abs(degrees - previousDegrees) * SERVO_TURN_SPEED;
    previousDegrees = degrees;
    timer_waitMillis(2000);
}

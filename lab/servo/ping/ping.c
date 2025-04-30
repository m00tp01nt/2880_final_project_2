#include <lab/servo/ping/ping.h>
#include <lab/timer/Timer.h>
#include <inc/tm4c123gh6pm.h>

volatile uint32_t g_start_time = 0x00000000; // Captured start time (rising edge)
volatile uint32_t g_end_time = 0x00000000; // Captured end time (falling edge) //initializing before edge detection
volatile enum
{
    LOW, HIGH, DONE
} g_state = LOW; // State machine for ISR

void ping_init(void)
{
    SYSCTL_RCGCTIMER_R |= 0x08; // Enable clock for Timer 3 (bit 3 = 0b00001000)
    SYSCTL_RCGCGPIO_R |= 0x02;   // Enable clock for Port B (bit 1 = 0b00000010)

    GPIO_PORTB_DEN_R |= 0x08; // Enable digital I/O for PB3 (bit 3 = 0b00001000)
    GPIO_PORTB_AFSEL_R |= 0x08;     // Enable alternate function on PB3
    GPIO_PORTB_PCTL_R &= ~0x0000F000;      // Clear PMC3
    GPIO_PORTB_PCTL_R |= 0x00007000;       // Set PB3 to T3CCP1 function

    TIMER3_CTL_R &= ~0x0100;               // Disable Timer 3B (TBEN = bit 8)
    TIMER3_CFG_R = 0x04;                   // 16-bit timer configuration
    TIMER3_TBMR_R = 0x07;                  // Edge-time capture mode, count down
    TIMER3_CTL_R |= 0x0C00; // Capture both edges (TBEVENT = 0b11 at bits 10-11)
    TIMER3_TBILR_R = 0xFFFF;               // Lower 16 bits = max
    TIMER3_TBPR_R = 0xFF;                  // Upper 8 bits = max
    TIMER3_IMR_R |= 0x00000400; // Enable capture event interrupt (CBEIM = bit10)
    NVIC_EN1_R |= 0b00000000000000000000000000010000; // Enable IRQ 35 (bit 4 of EN1)
    IntRegister(INT_TIMER3B, TIMER3B_Handler); // INT_TIMER3B = 35 = 0x23
    IntMasterEnable();                     // Enable global interrupts
    TIMER3_CTL_R |= 0x0100;                // Re-enable Timer 3B
}

void ping_trigger(void)
{
    TIMER3_CTL_R &= ~0x0100;               // Disable Timer 3B
    TIMER3_IMR_R &= ~0x00000400;           // Disable capture interrupt
    GPIO_PORTB_AFSEL_R &= ~0x08;           // Disable alternate function
    GPIO_PORTB_DIR_R |= 0x08;              // Set PB3 as output

    GPIO_PORTB_DATA_R &= ~0x08;            // Set PB3 LOW
    timer_waitMicros(0x05);                // Wait 5 microseconds
    GPIO_PORTB_DATA_R |= 0x08;             // Set PB3 HIGH
    timer_waitMicros(0x05);                // Wait 5 microseconds
    GPIO_PORTB_DATA_R &= ~0x08;            // Set PB3 LOW

    TIMER3_ICR_R = 0x00000400;         // Clear interrupt flag (CBECINT = bit 2)
    GPIO_PORTB_AFSEL_R |= 0x08;            // Re-enable alternate function
    GPIO_PORTB_DIR_R &= ~0x08;             // Set PB3 as input
    TIMER3_IMR_R |= 0x00000400;            // Re-enable capture interrupt
    TIMER3_CTL_R |= 0x0100;                // Re-enable Timer 3B
}

void TIMER3B_Handler(void)
{
    TIMER3_ICR_R = 0x00000400; // Clear capture event interrupt (bit 2 = 0b00000100)

    if (g_state == LOW)
    {
        // First edge (rising) detected
        g_start_time = (TIMER3_TBR_R & 0xFFFF) | ((TIMER3_TBPR_R & 0xFF) << 0x10);
        g_state = HIGH;
    }
    else if (g_state == HIGH)
    {
        // Second edge (falling) detected
        g_end_time = (TIMER3_TBR_R & 0xFFFF) | ((TIMER3_TBPR_R & 0xFF) << 0x10);
        g_state = DONE; // Both edges received, ready to compute distance
    }
}

float ping_getDistance(void)
{
    ping_trigger(); // Start sensor trigger and wait for echo
    while (g_state != DONE)
        ; // Wait for ISR to complete both edges
    uint32_t pulse_width;

    if (g_start_time > g_end_time)
    {
        pulse_width = g_start_time - g_end_time; // Normal case
    }
    else
    {
        pulse_width = (0xFFFF - g_end_time + g_start_time); // Handle timer overflow0xFFFFFF -
    }

    float distance_cm = pulse_width * (.0000000625) * 343 * 100 / 2; //(1.0 / 0xF4240);// * 0xF4240 / 0xA; // Convert ticks to microseconds (16 MHz)
    g_state = LOW; // Reset state machine
    return distance_cm;
}

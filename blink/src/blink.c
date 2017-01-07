#include "blink.h"
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

/**
 * DELAY may contain up to 256 entries, due to delayIdx being uint8
 */
uint16_t DELAY[] = {
  MS_TO_CLK(1000),
  MS_TO_CLK(1000),
  MS_TO_CLK(1000),
  MS_TO_CLK(500),
  MS_TO_CLK(500),
  MS_TO_CLK(500),
  MS_TO_CLK(250),
  MS_TO_CLK(250),
  MS_TO_CLK(250),
  MS_TO_CLK(250)
};
#define LED_ON_MS MS_TO_CLK(50)
#define DELAY_LEN ((sizeof(DELAY) / sizeof(int)))

volatile uint8_t delayIdx = 0;
volatile uint16_t delayCnt = 0;
volatile bool LED_ON = false;

ISR(TIMER0_OVF_vect) {
  delayCnt--;
  if (delayCnt == 0) {
    LED_ON ^= 1;
    
    if (LED_ON) {
      // Turn off after LED_ON_MS
      delayCnt = LED_ON_MS;
      setBit(LED_PORT, LED);
    }
    else {
      // Turn on after DELAY[delayIdx]
      delayCnt = DELAY[delayIdx++] - LED_ON_MS;
      clearBit(LED_PORT, LED);
    }
    
    // Reset counter at end of sequence
    if (delayIdx == DELAY_LEN) {
      delayIdx = 0;
    }
  }
}

int main (void) {
  delayCnt = 1;
  /* set pin 0 for output*/
  setBit(LED_DDR, LED);
  
  // Enable timer synchronization and reset until timer is configured
  GTCCR = _BV(TSM) | _BV(PSR0);
  // Enable timer overflow interrupt
  setBit(TIMSK, TOIE0);
  // Set initial timer value
  clearReg(TCNT0);
  // Set timer pre-scaler
  TCCR0B = PRESCALE_REG;
  // Enable timer
  GTCCR = 0;
  
  // Enable sleep mode
  set_sleep_mode(SLEEP_MODE_IDLE);  
  sleep_enable();
  // Turn interrupts on.
  sei();
  
  
  // Loop forever
  while(true) {
    // Put CPU to sleep
    sleep_cpu();
  }
}

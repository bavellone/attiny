#include <math.h> 
#define setBit(P,B) P |= _BV(B)
#define clearBit(P,B) P &= ~_BV(B)
#define toggleBit(P,B) P ^= _BV(B)

#define clearReg(R) R = 0x00

#define LED PB0
#define LED_PORT PORTB
#define LED_DDR DDRB

#define F_CPU 1000000UL
/**
 * PRESCALE_REG: controls timer prescaler
 * CS02 | CS01 | CS00 | Scale factor
 *  0      0      1     x1
 *  0      1      0     x8
 *  0      1      1     x64
 *  1      0      0     x256
 *  1      0      1     x1024
 */
#define PRESCALE_REG (_BV(CS01))
#define PRESCALE_CLK 8

#define MS_TO_CLK(ms) ((ms * (F_CPU / 1000)) / PRESCALE_CLK / 256)

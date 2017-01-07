# ATTiny85 Programming Notes

## Absolute Maximum Ratings

Characteristic | Rating
--- | ---
Operating Temp | -55C to +125C
Storage Temp | -65C to +150C
Pin Voltage wrt GND | -0.5V to VCC + 0.5V
Max Operating Voltage | 6V
DC Current per I/O Pin | 40mA
DC Current VCC and GND | 200mA

## DC Characteristics

Parameter | Condition | Min | Typical | Max | Units
--- | --- | --- | --- | --- | ---
I/O Pin Input Leakage Current | VCC = 5.5V | |< 0.05 | 1 | uA
I/O Pin Pull-up Resistor | VCC = 5.5V | 20 | | 50 | kΩ
Power Supply Current | Active 1 MHz, VCC = 2V | | 0.3 | 0.55 | mA
Power Supply Current | Active 4 MHz, VCC = 3V | | 1.5 | 2.5 | mA
Power Supply Current | Active 8 MHz, VCC = 5V | | 5 | 8 | mA
Power Supply Current | Idle 1 MHz, VCC = 2V | | 0.1 | 0.2 | mA
Power Supply Current | Idle 4 MHz, VCC = 3V | | 0.35 | 0.6 | mA
Power Supply Current | Idle 8 MHz, VCC = 5V | | 1.2 | 2 | mA
Power Supply Current (Power-down mode) | WDT enabled, VCC = 3V | | | 10 | µA
Power Supply Current (Power-down mode) | WDT disabled, VCC = 3V | | | 2 | µA

## Ports
Each of the AVR Digital I/O ports is associated with three (3) I/O register. A Data Direction Register (DDRX), A Pin Register (PINX) and a Port Register (PORTX). Where X is the port A, B, C, etc.

*Note: Each physical pin of a port is configured independently and thus a port can have some of its pins configured as input an the others as output pins.*

#### Data Direction Register (DDR)

DDRX is an 8-bit register which stores configuration information for the pins of PortX. Writing a 1 in the pin location in the DDRX makes the physical pin of that port an output pin and writing a 0 makes that pin an input pin.

The DDRXN bit in the DDRX Register selects the direction of this pin. If DDRXN is written logic one, PXN is configured
as an output pin. If DDRXN is written logic zero, PXN is configured as an input pin.
If PORTXN is written logic one when the pin is configured as an input pin, the pull-up resistor is activated. To switch
the pull-up resistor off, PORTXN has to be written logic zero or the pin has to be configured as an output pin. The
port pins are tri-stated when reset condition becomes active, even if no clocks are running.
If PORTXN is written logic one when the pin is configured as an output pin, the port pin is driven high (one). If
PORTXN is written logic zero when the pin is configured as an output pin, the port pin is driven low (zero). 

#### Port In (PIN)

PINX is an 8-bit register that stores the logic value, the current state, of the physical pins on PortX. So to read the values on the pins of PortX, you read the values that are in its PIN register.

#### Port Out (PORT)

PORTX is an 8-bit register which stores the logic values that currently being outputted on the physical pins of PortX if the pins are configured as output pins. So to write values to a port, you write the values to the PORT register of that port.

#### Layout

MCU Pin # | Port #
--- | ---
1 | PB5
2 | PB3
3 | PB4
4 | (GND)
5 | PB0
6 | PB1
7 | PB2
8 | (VCC)


## Interrupts
The AVR provides several different interrupt sources. These interrupts and the separate Reset Vector each have a
separate Program Vector in the Program memory space. All interrupts are assigned individual enable bits which
must be written logic one together with the Global Interrupt Enable bit in the Status Register in order to enable the
interrupt

The lower the address the higher is the priority level. RESET has the highest priority, and next
is INT0 – the External Interrupt Request 0.

When an interrupt occurs, the Global Interrupt Enable I-bit is cleared and all interrupts are disabled. The user software
can write logic one to the I-bit to enable nested interrupts. All enabled interrupts can then interrupt the current
interrupt routine. The I-bit is automatically set when a Return from Interrupt instruction – RETI – is executed. 

When the AVR exits from an interrupt, it will always return to the main program and execute one more instruction
before any pending interrupt is served. Note that the Status Register is not automatically stored when entering an interrupt routine, nor restored when
returning from an interrupt routine. This must be handled by software.

When using the CLI instruction to disable interrupts, the interrupts will be immediately disabled. No interrupt will be
executed after the CLI instruction, even if it occurs simultaneously with the CLI instruction. The following example
shows how this can be used to avoid interrupts during the timed EEPROM write sequence.

#### Assembly Code Example
    in r16, SREG ; store SREG value
    cli ; disable interrupts during timed sequence
    sbi EECR, EEMPE ; start EEPROM write
    sbi EECR, EEPE
    out SREG, r16 ; restore SREG value (I-bit)

#### C Code Example
    char cSREG;
    cSREG = SREG; /* store SREG value */
    /* disable interrupts during timed sequence */
    _CLI();
    EECR |= (1<<EEMPE); /* start EEPROM write */
    EECR |= (1<<EEPE);
    SREG = cSREG; /* restore SREG value (I-bit) */

### Interrupt Response Time
The interrupt execution response for all the enabled AVR interrupts is four clock cycles minimum. After four clock
cycles the Program Vector address for the actual interrupt handling routine is executed. During this four clock cycle
period, the Program Counter is pushed onto the Stack. The vector is normally a jump to the interrupt routine, and
this jump takes three clock cycles. If an interrupt occurs during execution of a multi-cycle instruction, this instruction
is completed before the interrupt is served. If an interrupt occurs when the MCU is in sleep mode, the interrupt execution
response time is increased by four clock cycles. This increase comes in addition to the start-up time from the
selected sleep mode.

A return from an interrupt handling routine takes four clock cycles. During these four clock cycles, the Program
Counter (two bytes) is popped back from the Stack, the Stack Pointer is incremented by two, and the I-bit in SREG
is set.

## Code Examples

[100kHz square wave generator](http://www.atmel.com/webdoc/AVRLibcReferenceManual/assembler_1ass_example.html)


## Links and References

[AVR Libc Reference](http://www.atmel.com/webdoc/AVRLibcReferenceManual/index.html)  
[CR2032 Datasheet](http://data.energizer.com/PDFs/cr2032.pdf)

/* Simple example for Teensy USB Development Board
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2008 PJRC.COM, LLC
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>
#include "usb_serial.h"

#define LED_CONFIG	(DDRD |= (1<<6))
#define LED_ON		(PORTD |= (1<<6))
#define LED_OFF		(PORTD &= ~(1<<6))
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

// Configure the USART for 115200bps, 8n1
void uart_init() {
	UBRR1H = 0;
	UBRR1L = 3;

	UCSR1A = 0x00;        // No 2x mode, no MPCMn mode
	UCSR1B = (1<<TXEN1);  // Enable transmitter but not receiver
	UCSR1C = (1<<UCSZ11) | (1<<UCSZ10); // Async, no parity bit, 1 stop bit, 8-bit
}

// Write a single byte to the serial port
static
void send_serial_byte(
	const char c
)
{
	// Wait until the data register is empty, then send the byte
	loop_until_bit_is_set(UCSR1A, UDRE1);
	UDR1 = c;
}

int main(void)
{
	CPU_PRESCALE(0);
	usb_init();
	uart_init();

	// Set the SN75176 to transmit mode
	//PD0 -> high
	DDRD |= _BV(0);
        PORTD |= _BV(0);

        // Turn on the LEDs
        //PB5, PB6 high


	while (1)
	{
		const int8_t n = usb_serial_available();
		if (n <= 0)
			continue;

		const uint8_t irq_flags = SREG;
		cli();

		int8_t i;
		for (i = 0 ; i < n ; i++)
		{
#define CDC_RX_ENDPOINT		3
			UENUM = CDC_RX_ENDPOINT;
			const uint8_t c = UEDATX;
			send_serial_byte(c);
		}

		// Release the USB buffer
		UEINTX = 0x6B;

		// Re-enabled interrupts
		SREG = irq_flags;
	}
}


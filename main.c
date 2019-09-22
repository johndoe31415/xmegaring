/*
	xmegaring - RS232-controlled LED ring
	Copyright (C) 2019-2019 Johannes Bauer

	This file is part of xmegaring.

	xmegaring is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; this program is ONLY licensed under
	version 3 of the License, later versions are explicitly excluded.

	xmegaring is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with xmegaring; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	Johannes Bauer <JohannesBauer@gmx.de>
*/

#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "hal.h"

static void spi_sendbyte(uint8_t value) {
	for (uint8_t i = 0; i < 8; i++) {
		MOSI_SetConditional((value & 1) != 0);
		SCK_PulseNop();
		value >>= 1;
	}
}

static void clock_configure_ext(void) {
	/* External clock is oscillator */
	OSC.XOSCCTRL = OSC_XOSCSEL_EXTCLK_gc;

	/* Enable external oscillator and internal 2MHz RC */
	OSC.CTRL = OSC_RC2MEN_bm | OSC_XOSCEN_bm;

	/* Wait for external clock to become ready */
	while (!(OSC.STATUS & OSC_XOSCRDY_bm));

	/* Enable switching register and change clock */
	CCP = CCP_IOREG_gc;
	CLK.CTRL = CLK_SCLKSEL_XOSC_gc;

	/* Then discable internal RC oscillator */
	OSC.CTRL &= ~OSC_RC2MEN_bm;
}

static void ledring_setbrightness(uint8_t brightness) {
	TCD0.CCB = 255 - brightness;
}

static void ledring_init(void) {
	LEDRing_SetActive();

	/*
	 * Single-slope PWM frequency of ~381 Hz
	 * 25e6 / 256 / 256 = 381.5
	 */
	TCD0.CTRLA = TC_CLKSEL_DIV256_gc;
	TCD0.CTRLB = TC0_CCBEN_bm | TC_WGMODE_SS_gc;
	TCD0.PER = 255;
	ledring_setbrightness(10);
}

static void ledring_send(uint32_t led_data) {
	CS_SetActive();
	spi_sendbyte(led_data >> 24);
	spi_sendbyte(led_data >> 16);
	spi_sendbyte(led_data >> 8);
	spi_sendbyte(led_data >> 0);
	CS_SetInactive();
}

static void rs232_init(void) {
	USARTE0.CTRLC = USART_CHSIZE_8BIT_gc;				/* 8N1 */
	USARTE0.CTRLA = USART_RXCINTLVL_LO_gc;				/* Receive IRQ: Low Prio */

	const uint16_t bsel = 1608;
	const int8_t bscale = -7;
	USARTE0.BAUDCTRLA = bsel & 0xff;
	USARTE0.BAUDCTRLB = ((bsel & 0xf00) >> 8) | ((bscale & 0xf) << 4);

	USARTE0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;		/* Enable RX & TX */
}

static void rs232_send(uint8_t character) {
	while (!(USARTE0.STATUS & USART_DREIF_bm));		/* Wait until TX buffer empty */
	USARTE0.DATA = character;
}

ISR(USARTE0_RXC_vect) {
	uint8_t rxed_char = USARTE0.DATA;
	rs232_send(rxed_char + 1);
}

int main(void) {
	initHAL();
	DebugLED_SetActive();
	clock_configure_ext();
	rs232_init();
	ledring_init();

	PMIC.CTRL = PMIC_LOLVLEN_bm;
	sei();

	uint32_t x = 0xaa550000;
	while (true) {
		x++;
		ledring_send(x);
		_delay_ms(100);
	}
}

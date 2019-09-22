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
#include <util/crc16.h>
#include "hal.h"

#define SOF_BYTE		0x5a
#define EOF_BYTE		0xa5

struct ledring_config_t {
	uint8_t brightness;
	uint32_t configuration;
} __attribute__ ((packed));

struct rx_command_payload_t {
	uint8_t sof;
	struct ledring_config_t ledring;
	uint8_t eof;
} __attribute__ ((packed));

struct rx_command_t {
	struct rx_command_payload_t payload;
	uint16_t crc;
} ;

static struct rx_command_t rx_command;
static uint8_t rx_buf_length;

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

static void ledring_send(uint32_t led_data) {
	CS_SetActive();
	spi_sendbyte(led_data >> 0);
	spi_sendbyte(led_data >> 8);
	spi_sendbyte(led_data >> 16);
	spi_sendbyte(led_data >> 24);
	CS_SetInactive();
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
	ledring_setbrightness(0);
	ledring_send(0);
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

static void rs232_send_hex_nibble(uint8_t nibble) {
	nibble &= 0xf;
	rs232_send((nibble < 10) ? (nibble + '0') : (nibble - 10 + 'a'));
}

static void rs232_send_hex_byte(uint8_t byte) {
	rs232_send_hex_nibble(byte >> 4);
	rs232_send_hex_nibble(byte >> 0);
}

static void rs232_send_hex_word(uint16_t word) {
	rs232_send_hex_byte(word >> 8);
	rs232_send_hex_byte(word >> 0);
}

static void timeout_timer_init(void) {
	/* Timeout after 100ms @ (25 MHz / 256) */
	/* c 'round(100e-3/(256/25e6))' */
	TCD1.PER = 9766;
	TCD1.INTCTRLA = TC_OVFINTLVL_LO_gc;
	TCD1.CTRLA = TC_CLKSEL_DIV256_gc;
}

static void timeout_reset(void) {
	TCD1.CNT = 0;
}

static bool command_crc_correct(void) {
	uint16_t crc = 0xffff;
	const uint8_t *payloadbuf = (const uint8_t*)&rx_command.payload;
	for (uint8_t i = 0; i < sizeof(rx_command.payload); i++) {
		crc = _crc_ccitt_update(crc, payloadbuf[i]);
	}
	return crc == rx_command.crc;
}

static void process_command(void) {
	if (rx_command.payload.eof != EOF_BYTE) {
		return;
	}

	if (!command_crc_correct()) {
		rs232_send('C');
		return;
	}

	ledring_send(rx_command.payload.ledring.configuration);
	ledring_setbrightness(rx_command.payload.ledring.brightness);
	rs232_send('O');
}

ISR(USARTE0_RXC_vect) {
	uint8_t rxed_char = USARTE0.DATA;
	timeout_reset();

	if (rx_buf_length >= sizeof(rx_command)) {
		/* Something has gone horribly wrong. */
		rx_buf_length = 0;
		return;
	}

	uint8_t *rx_buf = (uint8_t*)&rx_command;
	rx_buf[rx_buf_length++] = rxed_char;
	if ((rx_buf_length == 1) && (rx_command.payload.sof != SOF_BYTE)) {
		/* Not start-of-frame, discard immediately. */
		rx_buf_length = 0;
	} else if (rx_buf_length == sizeof(rx_command)) {
		/* Command finished */
		process_command();
		rx_buf_length = 0;
	}
}

ISR(TCD1_OVF_vect) {
	/* Reset current command */
	rx_buf_length = 0;
}

int main(void) {
	initHAL();
	DebugLED_SetActive();
	clock_configure_ext();
	rs232_init();
	ledring_init();
	timeout_timer_init();

	PMIC.CTRL = PMIC_LOLVLEN_bm;
	sei();

	while (true) {
	}
}

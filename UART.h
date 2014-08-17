/*
 * UART.h
 * Copyright (C) 2014 Krystian Dużyński <krystian.duzynski@gmail.com>
 */

#ifndef __UART_H__
#define __UART_H__

#include "public.h"

#define UART_BAUD_SELECT(baud,freq) (freq/16/baud-1)
#define UART_BAUD_SELECT_DOUBLE(baud,freq) (freq/8/baud-1)

//! [required] UART baud rate - value obtained from UART_BAUD_SELECT or UART_BAUD_SELECT_DOUBLE
#ifndef UART_BAUD
#  error UART_BAUD must be defined
#endif
#ifndef UART_DATABITS
#  define UART_DATABITS 8
#endif
#ifndef UART_DOUBLESPEED
#  define UART_DOUBLESPEED 0
#endif
#ifndef UART_STOPBITS
#  define UART_STOPBITS 1
#endif
// parity (0 - none, 1 - even, 2 - odd)
#ifndef UART_PARITY
#  define UART_PARITY 0
#endif

void uartInit();
void uartTransmitText(const char* text);
void uartTransmitText_P(const char* t);
void uartTransmitUInt8(uint8_t num);
void uartTransmitInt8(int8_t num);
void uartTransmitUInt32(uint32_t num);
void uartTransmitInt32(int32_t num);
void uartTransmitBin(uint8_t num);
uint8_t uartIsDataAvailable();
uint8_t uartReceive();
void uartFlush();
void uartEnableRXInterrupt();
void uartDisableRXInterrupt();

static inline void uartTX(uint8_t data)
{
	while (bit_is_clear(UCSRA, UDRE));
	UDR = data;
}
static inline uint8_t uartRX()
{
	while (bit_is_clear(UCSRA, RXC));
	return UDR;
}

#endif

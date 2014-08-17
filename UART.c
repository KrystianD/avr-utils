/*
 * UART.c
 * Copyright (C) 2014 Krystian Dużyński <krystian.duzynski@gmail.com>
 */

#include "UART.h"

void uartInit()
{
#if UART_STOPBITS == 1
#  define _UART_STOPBITS 0
#elif UART_STOPBITS == 2
#  define _UART_STOPBITS (_BV(USBS))
#endif

#if UART_DATABITS == 5
#  define _UART_9BIT      0
#  define _UART_DATABITS	0
#elif UART_DATABITS == 6
#  define _UART_9BIT      0
#  define _UART_DATABITS	(_BV(UCSZ0))
#elif UART_DATABITS == 7
#  define _UART_9BIT      0
#  define _UART_DATABITS	(_BV(UCSZ1))
#elif UART_DATABITS == 8
#  define _UART_9BIT      0
#  define _UART_DATABITS	(_BV(UCSZ1) | _BV(UCSZ0))
#elif UART_DATABITS == 9
#  define _UART_9BIT      (_BV(UCSZ2))
#  define _UART_DATABITS	(_BV(UCSZ1) | _BV(UCSZ0))
#endif

#if UART_PARITY == 0
#  define _UART_PARITY	0
#elif UART_PARITY == 1
#  define _UART_PARITY	(_BV(UPM1))
#elif UART_PARITY == 2
#  define _UART_PARITY	(_BV(UPM2))
#endif

	UBRRH = (uint8_t)((UART_BAUD) >> 8);
	UBRRL = (uint8_t)(UART_BAUD);
	
#if UART_DOUBLESPEED == 1
	UCSRA = _BV(U2X);
#endif
	
	UCSRB = _BV(RXEN) | _BV(TXEN) | _UART_9BIT;
	
#ifdef URSEL
	UCSRC = _BV(URSEL) | _UART_STOPBITS | _UART_DATABITS | _UART_PARITY;
#else
	UCSRC = _UART_STOPBITS | _UART_DATABITS | _UART_PARITY;
#endif
}
void uartTransmitText(const char* text)
{
	while (*text)
		uartTX(*text++);
}
void uartTransmitText_P(const char* t)
{
	char c;
	while ((c = pgm_read_byte(t++)))
		uartTX(c);
}
void uartTransmitUInt8(uint8_t num)
{
	char str[10];
	ultoa(num, str, 10);
	uartTransmitText(str);
}
void uartTransmitInt8(int8_t num)
{
	char str[10];
	ltoa(num, str, 10);
	uartTransmitText(str);
}
void uartTransmitUInt32(uint32_t num)
{
	char str[15];
	ultoa(num, str, 10);
	uartTransmitText(str);
}
void uartTransmitInt32(int32_t num)
{
	char str[15];
	ltoa(num, str, 10);
	uartTransmitText(str);
}
void uartTransmitBin(uint8_t num)
{
	char str[9], str2[9] = "00000000";
	ultoa(num, str, 2);
	str[sizeof(str) - 1] = 0;
	strcpy(str2 + 8 - strlen(str), str);
	uartTransmitText(str2);
}
uint8_t uartIsDataAvailable()
{
	return bit_is_set(UCSRA, RXC) ? true : false;
}
bool uartReceiveTimeout(uint8_t* data, uint16_t timeoutMiliseconds)
{
	while (timeoutMiliseconds && bit_is_clear(UCSRA, RXC))
	{
		_delay_ms(1);
		timeoutMiliseconds--;
	}
	if (bit_is_clear(UCSRA, RXC))
		return false;
	*data = UDR;
	return true;
}
void uartFlush()
{
	uint8_t tmp;
	while (bit_is_set(UCSRA, RXC))
		tmp = UDR;
}
void uartEnableRXInterrupt()
{
	sbi(UCSRB, RXCIE);
}
void uartDisableRXInterrupt()
{
	cbi(UCSRB, RXCIE);
}

/*
 * public.h
 * Copyright (C) 2014 Krystian Dużyński <krystian.duzynski@gmail.com>
 */

#ifndef __PUBLIC_H__
#define __PUBLIC_H__

#ifndef F_CPU
#error F_CPU must be defined!
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <util/twi.h>
#include <util/crc16.h>
#include <stdlib.h>
#include <string.h>

int main() __attribute__((noreturn));

#define sbi(var,bit)	(var) |= (1 << (bit))
#define cbi(var,bit)	(var) &= ~(1 << (bit))
#define tbi(var,bit)	(var) ^= (1 << (bit))

#define LOW(x) ((x) & 0xff)
#define HIGH(x) (((x) >> 8) & 0xff)

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// -------------------------------------------------------
// IO
// -------------------------------------------------------

#define PIN_FROM_PORT(port) _SFR_IO8(_SFR_IO_ADDR(port) - 2)
#define DDR_FROM_PORT(port) _SFR_IO8(_SFR_IO_ADDR(port) - 1)
#define PORT_FROM_PORT(port) _SFR_IO8(_SFR_IO_ADDR(port))

#define DEFINE_IO(x,port,pin) \
	const static uint8_t x ## _PIN_ADDR = _SFR_IO_ADDR(port) - 2; \
	const static uint8_t x ## _DDR_ADDR = _SFR_IO_ADDR(port) - 1; \
	const static uint8_t x ## _PORT_ADDR = _SFR_IO_ADDR(port); \
	const static uint8_t x ## _PIN_NR = pin;

#define PIN_FROM_DEFINE(x) 		_SFR_IO8(x ## _PIN_ADDR)
#define DDR_FROM_DEFINE(x) 		_SFR_IO8(x ## _DDR_ADDR)
#define PORT_FROM_DEFINE(x) 	_SFR_IO8(x ## _PORT_ADDR)
#define PIN_NR_FROM_DEFINE(x) x ## _PIN_NR

static uint8_t	min(uint8_t n1, uint8_t n2)
{
	return n1 < n2 ? n1 : n2;
}
static uint8_t	max(uint8_t n1, uint8_t n2)
{
	return n1 > n2 ? n1 : n2;
}
static uint16_t	minU16(uint16_t n1, uint16_t n2)
{
	return n1 < n2 ? n1 : n2;
}
static uint16_t	maxU16(uint16_t n1, uint16_t n2)
{
	return n1 > n2 ? n1 : n2;
}
static int16_t	minI16(int16_t n1, int16_t n2)
{
	return n1 < n2 ? n1 : n2;
}
static int16_t	maxI16(int16_t n1, int16_t n2)
{
	return n1 > n2 ? n1 : n2;
}
static int16_t	absI16(int16_t n)
{
	return n < 0 ? -n : n;
}

#define IO_PUSH_PULL(x)    _IO_PUSH_PULL(x)
#define _IO_PUSH_PULL(x,y) __IO_PUSH_PULL(_SFR_IO_ADDR(x),y)
#define IO_INPUT(x)        _IO_INPUT(x)
#define _IO_INPUT(x,y)     __IO_INPUT(_SFR_IO_ADDR(x),y)
#define IO_INPUT_PP(x)     _IO_INPUT_PP(x)
#define _IO_INPUT_PP(x,y)  __IO_INPUT_PP(_SFR_IO_ADDR(x),y)
#define IO_HIGH(x)         _IO_HIGH(x)
#define _IO_HIGH(x,y)      __IO_HIGH(_SFR_IO_ADDR(x),y)
#define IO_LOW(x)          _IO_LOW(x)
#define _IO_LOW(x,y)       __IO_LOW(_SFR_IO_ADDR(x),y)
#define IO_TOGGLE(x)       _IO_TOGGLE(x)
#define _IO_TOGGLE(x,y)    __IO_TOGGLE(_SFR_IO_ADDR(x),y)
#define IO_IS_HIGH(x)      _IO_IS_HIGH(x)
#define _IO_IS_HIGH(x,y)   __IO_IS_HIGH(_SFR_IO_ADDR(x),y)
#define IO_IS_LOW(x)       _IO_IS_LOW(x)
#define _IO_IS_LOW(x,y)    __IO_IS_LOW(_SFR_IO_ADDR(x),y)

static inline void __IO_PUSH_PULL(unsigned char port, unsigned char pin)
{
	DDR_FROM_PORT(_SFR_IO8(port)) |= 1 << pin;
}
static inline void __IO_INPUT(unsigned char port, unsigned char pin)
{
	DDR_FROM_PORT(_SFR_IO8(port)) &= ~(1 << pin);
}

static inline void __IO_INPUT_PP(unsigned char port, unsigned char pin)
{
	DDR_FROM_PORT(_SFR_IO8(port)) &= ~(1 << pin);
	_SFR_IO8(port) |= 1 << pin;
}
static inline void __IO_HIGH(unsigned char port, unsigned char pin)
{
	_SFR_IO8(port) |= 1 << pin;
}
static inline void __IO_LOW(unsigned char port, unsigned char pin)
{
	_SFR_IO8(port) &= ~(1 << pin);
}
static inline void __IO_TOGGLE(unsigned char port, unsigned char pin)
{
	_SFR_IO8(port) ^= 1 << pin;
}

static inline uint8_t __IO_IS_HIGH(unsigned char port, unsigned char pin)
{
	return (PIN_FROM_PORT(_SFR_IO8(port)) & (1 << pin)) ? 1 : 0;
}
static inline uint8_t __IO_IS_LOW(unsigned char port, unsigned char pin)
{
	return (PIN_FROM_PORT(_SFR_IO8(port)) & (1 << pin)) == 0;
}

#endif

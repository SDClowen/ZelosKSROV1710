#ifndef __ZELOS_LIBAUTH_SECURITY_HPP__
#define __ZELOS_LIBAUTH_SECURITY_HPP__

#include <stdint.h>

#define HIBYTE(w)	((unsigned char)((w & 0xff00) >> 8))
#define LOBYTE(w)	((unsigned char)(w & 0x00ff))
#define LOWORD(w)	((unsigned short)(w))
#define HIWORD(w)	((unsigned short)(((w) >> 16) & 0xFFFF))

void Func_X_2(uint8_t *stream, uint32_t key, uint8_t keyByte);

uint32_t ModExp( uint32_t P, uint32_t X, uint32_t G );

// This function was written by jMerlin as part of the article "How to generate the security bytes for SRO"
uint32_t GenerateValue( uint32_t value );

uint8_t GenerateCheckByte(const char* packet, int length, uint32_t seed );

#endif

#include "auth/security.h"
#include "auth/data.h"

// Helper function used in the handshake
void Func_X_2(uint8_t *stream, uint32_t key, uint8_t keyByte)
{
	stream[0] ^= (stream[0] + LOBYTE(LOWORD(key)) + keyByte);
	stream[1] ^= (stream[1] + HIBYTE(LOWORD(key)) + keyByte);
	stream[2] ^= (stream[2] + LOBYTE(HIWORD(key)) + keyByte);
	stream[3] ^= (stream[3] + HIBYTE(HIWORD(key)) + keyByte);
	stream[4] ^= (stream[4] + LOBYTE(LOWORD(key)) + keyByte);
	stream[5] ^= (stream[5] + HIBYTE(LOWORD(key)) + keyByte);
	stream[6] ^= (stream[6] + LOBYTE(HIWORD(key)) + keyByte);
	stream[7] ^= (stream[7] + HIBYTE(HIWORD(key)) + keyByte);
}

uint32_t ModExp( uint32_t P, uint32_t X, uint32_t G )
{
    uint64_t result = 1;
    uint64_t mult = G;

	if (X == 0)
        return 1;

	while( X )
	{
		if(X & 1)
            result = ( mult * result ) % P;

		X = X >> 1;

		mult = ( mult * mult ) % P;
	}
    return (uint32_t)result;
}

uint32_t GenerateValue( uint32_t value )
{
    uint32_t val = value;
	for( int i = 0; i < 32; i++ )
		val = (((((((((((val >> 2)^val) >> 2)^val) >> 1)^val) >> 1)^val) >> 1)^val)&1)|((((val&1) << 31)|(val >> 1))&0xFFFFFFFE);
	return val;
}

uint8_t GenerateCheckByte(const char* packet, int length, uint32_t seed )
{
	if( !packet )
		return 0;

    uint32_t checksum = 0xFFFFFFFF;
	length &= 0x7FFF;
	const char* ptr = packet;
    uint32_t moddedseed = seed << 8;
	for( int i = 0; i < length; i++ )
		checksum = ( checksum >> 8 ) ^ table[moddedseed + ( ( *(ptr++) ^ checksum ) & 0xFF )];
	unsigned char result = (unsigned char)(( (checksum>>24)&0xFF ) + ( (checksum>>8)&0xFF ) + ( (checksum>>16)&0xFF ) + ( checksum&0xFF ));
	return result;
}


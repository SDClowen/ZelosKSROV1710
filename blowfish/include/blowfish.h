#ifndef __BLOWFISH_HPP__
#define __BLOWFISH_HPP__

/*
    Blowfish
    By: Jim Conger (original Bruce Schneier)
    Url: http://www.schneier.com/blowfish-download.html
*/

#include <stdint.h>

#define MAXKEYBYTES 	56		// 448 bits max
#define NPASS           16		// SBox passes

#define DWORD  		uint32_t
#define WORD  		uint16_t
#define BYTE  		uint8_t
typedef BYTE* LPBYTE;
typedef WORD* LPWORD;
typedef DWORD* LPDWORD;

class Blowfish
{
private:
    DWORD 		* PArray;
    DWORD		(* SBoxes)[256];
    void 		Blowfish_encipher(DWORD *xl, DWORD *xr);
    void 		Blowfish_decipher(DWORD *xl, DWORD *xr);

public:
    Blowfish();
    ~Blowfish();
    void 		Initialize(BYTE key[], int keybytes);
    DWORD		GetOutputLength(DWORD lInputLong);
    DWORD		Encode(BYTE * pInput, BYTE * pOutput, DWORD lSize);
    void		Decode(BYTE * pInput, BYTE * pOutput, DWORD lSize);
};

#endif //__BLOWFISH_HPP__

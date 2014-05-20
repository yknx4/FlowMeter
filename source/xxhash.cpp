/*
   xxHash - Fast Hash algorithm
   Copyright (C) 2012, Yann Collet.
   BSD 2-Clause License (http://www.opensource.org/licenses/bsd-license.php)

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:

       * Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
       * Redistributions in binary form must reproduce the above
   copyright notice, this list of conditions and the following disclaimer
   in the documentation and/or other materials provided with the
   distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	You can contact the author at :
	- xxHash source repository : http://code.google.com/p/xxhash/
*/



//**************************************
// Tuning parameters
//**************************************
// FORCE_NATIVE_FORMAT :
// By default, xxHash library provides endian-independant Hash values.
// Results are therefore identical for big-endian and little-endian CPU.
// This comes at a  performance cost for big-endian CPU, since some swapping is required to emulate little-endian format.
// Should endian-independance be of no importance to your application, you may uncomment the #define below
// It will improve speed for Big-endian CPU.
// This option has no impact on Little_Endian CPU.
//#define FORCE_NATIVE_FORMAT 1



//**************************************
// Includes
//**************************************

#include "xxhash.h"







//**************************************
// Compiler-specific Options & Functions
//**************************************
#define GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)

// Note : under GCC, it may sometimes be faster to enable the (2nd) macro definition, instead of using win32 intrinsic
#if defined(_WIN32)
#  define XXH_rotl32(x,r) _rotl(x,r)
#else
#  define XXH_rotl32(x,r) ((x << r) | (x >> (32 - r)))
#endif

#if defined(_MSC_VER)     // Visual Studio
#  define XXH_swap32 _byteswap_ulong
#elif GCC_VERSION >= 403
#  define XXH_swap32 __builtin_bswap32
#else
static inline unsigned int XXH_swap32 (unsigned int x) {
                        return  ((x << 24) & 0xff000000 ) |
                                ((x <<  8) & 0x00ff0000 ) |
                                ((x >>  8) & 0x0000ff00 ) |
                                ((x >> 24) & 0x000000ff );
                 }
#endif



//**************************************
// Constants
//**************************************
#define PRIME32_1   2654435761U
#define PRIME32_2   2246822519U
#define PRIME32_3   3266489917U
#define PRIME32_4    668265263U
#define PRIME32_5    374761393U



//**************************************
// Macros
//**************************************
#define XXH_LE32(p)  ( *(unsigned int*)(p))



//****************************
// Simple Hash Functions
//****************************

unsigned int XXH32(const void* input, int len, unsigned int seed)
{

	const unsigned char* p = (const unsigned char*)input;
	const unsigned char* const bEnd = p + len;
	unsigned int h32;

	if (len>=16)
	{
		const unsigned char* const limit = bEnd - 16;
		unsigned int v1 = seed + PRIME32_1 + PRIME32_2;
		unsigned int v2 = seed + PRIME32_2;
		unsigned int v3 = seed + 0;
		unsigned int v4 = seed - PRIME32_1;

		do
		{
			v1 += XXH_LE32(p) * PRIME32_2; v1 = XXH_rotl32(v1, 13); v1 *= PRIME32_1; p+=4;
			v2 += XXH_LE32(p) * PRIME32_2; v2 = XXH_rotl32(v2, 13); v2 *= PRIME32_1; p+=4;
			v3 += XXH_LE32(p) * PRIME32_2; v3 = XXH_rotl32(v3, 13); v3 *= PRIME32_1; p+=4;
			v4 += XXH_LE32(p) * PRIME32_2; v4 = XXH_rotl32(v4, 13); v4 *= PRIME32_1; p+=4;
		} while (p<=limit) ;

		h32 = XXH_rotl32(v1, 1) + XXH_rotl32(v2, 7) + XXH_rotl32(v3, 12) + XXH_rotl32(v4, 18);
	}
	else
	{
		h32  = seed + PRIME32_5;
	}

	h32 += (unsigned int) len;
	
	while (p<=bEnd-4)
	{
		h32 += XXH_LE32(p) * PRIME32_3;
		h32 = XXH_rotl32(h32, 17) * PRIME32_4 ;
		p+=4;
	}

	while (p<bEnd)
	{
		h32 += (*p) * PRIME32_5;
		h32 = XXH_rotl32(h32, 11) * PRIME32_1 ;
		p++;
	}

	h32 ^= h32 >> 15;
	h32 *= PRIME32_2;
	h32 ^= h32 >> 13;
	h32 *= PRIME32_3;
	h32 ^= h32 >> 16;

	return h32;


}



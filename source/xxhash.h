//-------------------------------------------------------------------
#ifndef __xxhash_H__
#define __xxhash_H__
//-------------------------------------------------------------------

#include <arduino.h>

//-------------------------------------------------------------------

//-------------------------------------------------------------------

//****************************
// Simple Hash Functions
//****************************

unsigned int XXH32 (const void* input, int len, unsigned int seed);

/*
XXH32() :
	Calculate the 32-bits hash of "input", of length "len"
	"seed" can be used to alter the result
	This function successfully passes all SMHasher tests.
	Speed on Core 2 Duo @ 3 GHz (single thread, SMHasher benchmark) : 5.4 GB/s
	Note that "len" is type "int", which means it is limited to 2^31-1.
	If your data is larger, use the advanced functions below.
*/



//-------------------------------------------------------------------

//===================================================================
// -> DO NOT WRITE ANYTHING BETWEEN HERE...
// 		This section is reserved for automated code generation
// 		This process tries to detect all user-created
// 		functions in main_sketch.cpp, and inject their  
// 		declarations into this file.
// 		If you do not want to use this automated process,  
//		simply delete the lines below, with "&MM_DECLA" text 
//===================================================================
//---- DO NOT DELETE THIS LINE -- @MM_DECLA_BEG@---------------------
//---- DO NOT DELETE THIS LINE -- @MM_DECLA_END@---------------------
// -> ...AND HERE. This space is reserved for automated code generation!
//===================================================================


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------


#include <string.h>
#include <math.h>

#include "seal.ntt.h"
#include <android/log.h>

#define SEAL_TAG "Grey Seal Ntt (Native)"
#define Seal_Log(...) __android_log_print(ANDROID_LOG_INFO, SEAL_TAG, __VA_ARGS__)
#define Seal_LogError(...) __android_log_print(ANDROID_LOG_ERROR, SEAL_TAG, __VA_ARGS__)

#define F_HASX 0x1
#define F_HASDOT 0x2
#define F_INVERSE 0x4

enum SealNtt_NumberType {
	NT_Error = -1,
	NT_Integer = 0,
	NT_Float = 4,
	NT_Hex = 2
};

enum SealNtt_NumberType Ntt_DetermineNumberType(const char* str){
	int flags = 0;

	const char* start = str;

	size_t length = strlen(str);
	if(length > 1 && strncmp(str, "-", 1) == 0){
		flags |= F_INVERSE;
		str++;
	}
	// Hexadecimal prefix
	if(length > 2 && strncmp(str, "0x", 2) == 0){
		if(flags & F_INVERSE) return NT_Error;
		flags |= F_HASX;
		str += 2;		//Skip the prefix
	}

	while(*str != '\0'){
		char current = *str;
		if(current == '.'){
			if(flags & F_HASDOT || flags & F_HASX){	
				// If we already have a dot or the number has an hex prefix, then the formatting is wrong
				return NT_Error;
			}
			flags |= F_HASDOT;
		}else if(!(('0' <= current && current <= '9') 
			|| (flags & F_HASX && 
			(('a' <= current && current <= 'f') 
				|| ('A' <= current && current <= 'F'))))){
			// If the character is invalid in the current format, the quit
			return NT_Error;
		}
		++str;
	}

	if(flags & F_HASDOT) return NT_Float;
	if(flags & F_HASX) return NT_Hex;
	return NT_Integer;
}

int SealNtt_StringToNum(const char* str, float* f){	
	enum SealNtt_NumberType kind = Ntt_DetermineNumberType(str);
	switch (kind)
	{
	case NT_Float: *f = strtof(str, NULL); return SEAL_NTT_SUCCESS;
	case NT_Hex: *f = (float)strtol(str + 2, NULL, 16); return SEAL_NTT_SUCCESS;
	case NT_Integer: *f = (float)atoi(str); return SEAL_NTT_SUCCESS;
	default:
		SealNtt_RaiseError(NTTE_InvalidNumber, str);
		return SEAL_NTT_ERROR;
	}
}

int SealNtt_StringToInt(const char* str, int* i){
	enum SealNtt_NumberType kind = Ntt_DetermineNumberType(str);

	switch (kind)
	{
	case NT_Hex: *i = (int)strtol(str + 2, NULL, 16); return SEAL_NTT_SUCCESS;
	case NT_Integer: *i = atoi(str); return SEAL_NTT_SUCCESS;
	default: SealNtt_RaiseError(NTTE_InvalidNumber, str); return SEAL_NTT_ERROR;
	}
}

char* SealNtt_ConcatString(char* buffer, size_t bufferSize, char* extend, size_t amount)
{
	if(buffer == NULL || extend == NULL) return NULL;
	
	char* nBuffer = malloc(bufferSize + amount + 1);
	char* bPtr = buffer; int i = 0; char *oPtr = nBuffer;
	while(*bPtr != '\0' && i < bufferSize)
	{
		*oPtr = *bPtr;
		i++; ++bPtr; ++oPtr;
	}
	bPtr = extend; i = 0;
	while(*bPtr != '\0' && i < amount)
	{
		*oPtr = *bPtr;
		i++; ++bPtr; ++oPtr;
	}
	*oPtr = '\0';
	

	free(buffer);

	return nBuffer;
}
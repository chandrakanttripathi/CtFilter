#include "SHA1.h"

// Rotate x bits to the left
#ifndef ROL32
#ifdef _MSC_VER
#define ROL32(_val32, _nBits) _rotl(_val32, _nBits)
#else
#define ROL32(_val32, _nBits) (((_val32)<<(_nBits))|((_val32)>>(32-(_nBits))))
#endif
#endif

#ifdef SHA1_LITTLE_ENDIAN
#define SHABLK0(i) (io_Sha1->m_block->l[i] = \
	(ROL32(io_Sha1->m_block->l[i],24) & 0xFF00FF00)  \
	| (ROL32(io_Sha1->m_block->l[i],8) & 0x00FF00FF))
#else
#define SHABLK0(i) (io_Sha1->m_block->l[i])
#endif

#define SHABLK(i) (io_Sha1->m_block->l[i&15] = \
	ROL32(io_Sha1->m_block->l[(i+13)&15] ^ io_Sha1->m_block->l[(i+8)&15] \
	^ io_Sha1->m_block->l[(i+2)&15] ^ io_Sha1->m_block->l[i&15],1))

// SHA-1 rounds
#define _R0(v,w,x,y,z,i) { z+=((w&(x^y))^y)+SHABLK0(i)+0x5A827999+ROL32(v,5); w=ROL32(w,30); }
#define _R1(v,w,x,y,z,i) { z+=((w&(x^y))^y)+SHABLK(i)+0x5A827999+ROL32(v,5); w=ROL32(w,30); }
#define _R2(v,w,x,y,z,i) { z+=(w^x^y)+SHABLK(i)+0x6ED9EBA1+ROL32(v,5); w=ROL32(w,30); }
#define _R3(v,w,x,y,z,i) { z+=(((w|x)&y)|(w&x))+SHABLK(i)+0x8F1BBCDC+ROL32(v,5); w=ROL32(w,30); }
#define _R4(v,w,x,y,z,i) { z+=(w^x^y)+SHABLK(i)+0xCA62C1D6+ROL32(v,5); w=ROL32(w,30); }



void Initialize(PSHA1 io_Sha1)
{
	io_Sha1->m_block = (SHA1_WORKSPACE_BLOCK *)io_Sha1->m_workspace;

	ShaReset(io_Sha1);
}

void ShaReset(PSHA1 io_Sha1)
{
	// SHA1 initialization constants
	io_Sha1->m_state[0] = 0x67452301;
	io_Sha1->m_state[1] = 0xEFCDAB89;
	io_Sha1->m_state[2] = 0x98BADCFE;
	io_Sha1->m_state[3] = 0x10325476;
	io_Sha1->m_state[4] = 0xC3D2E1F0;

	io_Sha1->m_count[0] = 0;
	io_Sha1->m_count[1] = 0;
}


void ShaTransform(PSHA1 io_Sha1,UINT_32 *state, UINT_8 *buffer)
{
	// Copy state[] to working vars
	UINT_32 a = state[0], b = state[1], c = state[2], d = state[3], e = state[4];

#if !defined(KERNEL_MODE)
	memcpy( io_Sha1->m_block, buffer, 64);
#else
	RtlCopyMemory(io_Sha1->m_block, buffer, 64);
#endif

	// 4 rounds of 20 operations each. Loop unrolled.
	_R0(a,b,c,d,e, 0); _R0(e,a,b,c,d, 1); _R0(d,e,a,b,c, 2); _R0(c,d,e,a,b, 3);
	_R0(b,c,d,e,a, 4); _R0(a,b,c,d,e, 5); _R0(e,a,b,c,d, 6); _R0(d,e,a,b,c, 7);
	_R0(c,d,e,a,b, 8); _R0(b,c,d,e,a, 9); _R0(a,b,c,d,e,10); _R0(e,a,b,c,d,11);
	_R0(d,e,a,b,c,12); _R0(c,d,e,a,b,13); _R0(b,c,d,e,a,14); _R0(a,b,c,d,e,15);
	_R1(e,a,b,c,d,16); _R1(d,e,a,b,c,17); _R1(c,d,e,a,b,18); _R1(b,c,d,e,a,19);
	_R2(a,b,c,d,e,20); _R2(e,a,b,c,d,21); _R2(d,e,a,b,c,22); _R2(c,d,e,a,b,23);
	_R2(b,c,d,e,a,24); _R2(a,b,c,d,e,25); _R2(e,a,b,c,d,26); _R2(d,e,a,b,c,27);
	_R2(c,d,e,a,b,28); _R2(b,c,d,e,a,29); _R2(a,b,c,d,e,30); _R2(e,a,b,c,d,31);
	_R2(d,e,a,b,c,32); _R2(c,d,e,a,b,33); _R2(b,c,d,e,a,34); _R2(a,b,c,d,e,35);
	_R2(e,a,b,c,d,36); _R2(d,e,a,b,c,37); _R2(c,d,e,a,b,38); _R2(b,c,d,e,a,39);
	_R3(a,b,c,d,e,40); _R3(e,a,b,c,d,41); _R3(d,e,a,b,c,42); _R3(c,d,e,a,b,43);
	_R3(b,c,d,e,a,44); _R3(a,b,c,d,e,45); _R3(e,a,b,c,d,46); _R3(d,e,a,b,c,47);
	_R3(c,d,e,a,b,48); _R3(b,c,d,e,a,49); _R3(a,b,c,d,e,50); _R3(e,a,b,c,d,51);
	_R3(d,e,a,b,c,52); _R3(c,d,e,a,b,53); _R3(b,c,d,e,a,54); _R3(a,b,c,d,e,55);
	_R3(e,a,b,c,d,56); _R3(d,e,a,b,c,57); _R3(c,d,e,a,b,58); _R3(b,c,d,e,a,59);
	_R4(a,b,c,d,e,60); _R4(e,a,b,c,d,61); _R4(d,e,a,b,c,62); _R4(c,d,e,a,b,63);
	_R4(b,c,d,e,a,64); _R4(a,b,c,d,e,65); _R4(e,a,b,c,d,66); _R4(d,e,a,b,c,67);
	_R4(c,d,e,a,b,68); _R4(b,c,d,e,a,69); _R4(a,b,c,d,e,70); _R4(e,a,b,c,d,71);
	_R4(d,e,a,b,c,72); _R4(c,d,e,a,b,73); _R4(b,c,d,e,a,74); _R4(a,b,c,d,e,75);
	_R4(e,a,b,c,d,76); _R4(d,e,a,b,c,77); _R4(c,d,e,a,b,78); _R4(b,c,d,e,a,79);

	// Add the working vars back into state
	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
	state[4] += e;

	// Wipe variables
#ifdef SHA1_WIPE_VARIABLES
	a = b = c = d = e = 0;
#endif
}

// Use this function to hash in binary data and strings
void Update(PSHA1 io_Sha1, UINT_8 *data, UINT_32 len)
{
	UINT_32 i, j;

	j = (io_Sha1->m_count[0] >> 3) & 63;

	if((io_Sha1->m_count[0] += len << 3) < (len << 3)) io_Sha1->m_count[1]++;

	io_Sha1->m_count[1] += (len >> 29);

	if((j + len) > 63)
	{
		i = 64 - j;
#if !defined(KERNEL_MODE)
		memcpy(&io_Sha1->m_buffer[j], data, i);
#else
		RtlCopyMemory(&io_Sha1->m_buffer[j], data, i);
#endif
		ShaTransform(io_Sha1,io_Sha1->m_state, io_Sha1->m_buffer);

		for( ; i + 63 < len; i += 64) 
			ShaTransform(io_Sha1, io_Sha1->m_state, &data[i]);

		j = 0;
	}
	else 
		i = 0;

#if !defined(KERNEL_MODE)
	memcpy(&io_Sha1->m_buffer[j], &data[i], len-i );
#else
	RtlCopyMemory(&io_Sha1->m_buffer[j], &data[i], len - i);
#endif
}


void Finalize(PSHA1 io_Sha1)
{
	UINT_32 i;
	UINT_8 finalcount[8];

	for(i = 0; i < 8; i++)
		finalcount[i] = (UINT_8)((io_Sha1->m_count[((i >= 4) ? 0 : 1)]
			>> ((3 - (i & 3)) * 8) ) & 255); // Endian independent

	Update(io_Sha1,(UINT_8 *)"\200", 1);

	while ((io_Sha1->m_count[0] & 504) != 448)
		Update(io_Sha1,(UINT_8 *)"\0", 1);

	Update(io_Sha1,finalcount, 8); // Cause a SHA1Transform()

	for(i = 0; i < 20; i++)
	{
		io_Sha1->m_digest[i] = (UINT_8)((io_Sha1->m_state[i >> 2] >> ((3 - (i & 3)) * 8) ) & 255);
	}

	// Wipe variables for security reasons
#ifdef SHA1_WIPE_VARIABLES
	i = 0;
	memset(io_Sha1->m_buffer, 0, 64);
	memset(io_Sha1->m_state, 0, 20);
	memset(io_Sha1->m_count, 0, 8);
	memset(finalcount, 0, 8);
	ShaTransform(io_Sha1,io_Sha1->m_state, io_Sha1->m_buffer);
#endif
}

// After calling Finalize() you can get the signature by calling this function
// It returns signature in raw (binary) form.
BOOLEAN GetBinSignature( PSHA1 i_Sha1, UINT_8 *o_Buffer )
{
#if !defined(KERNEL_MODE)
	memcpy(o_Buffer, i_Sha1->m_digest, 20);
#else
	RtlCopyMemory(o_Buffer, i_Sha1->m_digest, 20);
#endif
	return TRUE;
}

// After calling Finalize() you can get the signature by calling this function
// It returns signature in Hexadecimal string form.
BOOLEAN GetHexSignature( PSHA1 i_Sha1, WCHAR *o_HexBuffer )
{
	UINT_8 Buffer[SIGNATURE_SIZE];

	if( FALSE == GetBinSignature( i_Sha1, Buffer ) )
		return FALSE;

	return BinToHex( Buffer, SIGNATURE_SIZE, o_HexBuffer);
}

// This functin converts signature from binary form to hexadecimal representation
// in string format.
BOOLEAN BinToHex( UINT_8 *BinBuffer, UINT_32 BinBufferLen, WCHAR *HexBuffer	)
{
	unsigned char i;
	WCHAR szTemp[3] = {0};
#if defined(KERNEL_MODE)
	unsigned int strLen=0;
	UNICODE_STRING usTemp;
	UNICODE_STRING usHexBuffer;

	// Initialize string with dummy two character string so that it allocates memory.
	RtlInitUnicodeString(&usTemp, L"00");
	strLen = ((BinBufferLen+1) * 2)*sizeof(WCHAR); // Null terminated character counted.
	usHexBuffer.Length=0;
	usHexBuffer.MaximumLength=strLen;
	usHexBuffer.Buffer = HexBuffer;
#endif

	if(HexBuffer == NULL) return FALSE;

	HexBuffer[0] = UNICODE_NULL;
	for(i = 0; i < BinBufferLen; i++)
	{
#if defined(KERNEL_MODE)
		RtlIntegerToUnicodeString( BinBuffer[i], 10, &usTemp);
		RtlAppendUnicodeStringToString( &usHexBuffer, &usTemp);
#else
		swprintf(szTemp,L"%02X", BinBuffer[i]);
		wcscat_s(HexBuffer,HEX_SIGN_BUF_SIZE, szTemp);
#endif
	}

	return TRUE;
}

// This functin converts signature from hexadecimal string format
// to binary representation in raw bytes.
BOOLEAN HexToBin( WCHAR *HexBuffer, UINT_32 HexStringLen, UINT_8 *BinBuffer )
{
	unsigned int value,i;

	for( i=0; i < HexStringLen; i+=2)
	{
		value = ToHex(HexBuffer[i]) * 16 + ToHex(HexBuffer[i+1]) ;
		BinBuffer[i] = value;
	}
	return TRUE;
}

unsigned char ToHex( TCHAR ch )
{
	unsigned char ret=0;

#if defined(KERNEL_MODE)
	ch = RtlUpcaseUnicodeChar(ch); 
#else
#ifdef  UNICODE 
	ch = towupper(ch);
#else
	ch = toupper(ch);
#endif
#endif

	ret = ch - '0';
	if( ret<=9 )
		return ret;

	return (ch - 'A' ) + 0x0A;
}

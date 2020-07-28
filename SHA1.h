#ifndef __SHA1__
#define __SHA1__

#include <ntddk.h> // Needed for RtlCopyMemory

// You can define the endian mode in your files, without modifying the SHA1
// source files. Just #define SHA1_LITTLE_ENDIAN or #define SHA1_BIG_ENDIAN
// in your files, before including the SHA1.h header file. If you don't
// define anything, the class defaults to little endian.

#if !defined(SHA1_LITTLE_ENDIAN) && !defined(SHA1_BIG_ENDIAN)
#define SHA1_LITTLE_ENDIAN
#endif

// Same here. If you want variable wiping, #define SHA1_WIPE_VARIABLES, if
// not, #define SHA1_NO_WIPE_VARIABLES. If you don't define anything, it
// defaults to wiping.

#if !defined(SHA1_WIPE_VARIABLES) && !defined(SHA1_NO_WIPE_VARIABLES)
#define SHA1_WIPE_VARIABLES
#endif


/////////////////////////////////////////////////////////////////////////////
// Define 8- and 32-bit variables

#ifndef UINT_32

#define UINT_8 unsigned char

#if (ULONG_MAX == 0xFFFFFFFF)
#define UINT_32 unsigned long
#else
#define UINT_32 unsigned int
#endif

#endif

/////////////////////////////////////////////////////////////////////////////
// Defines

// Maximum number of bytes to read from the file at a time.
#define MAX_BUFFER_SIZE 3072

// Size of the signature
#ifndef SIGNATURE_SIZE
	#define SIGNATURE_SIZE 20
#endif

#define HEX_SIGN_SIZE 40
#define HEX_SIGN_BUF_SIZE HEX_SIGN_SIZE+1

/////////////////////////////////////////////////////////////////////////////
// Declare SHA1 workspace

typedef union
{
	UINT_8  c[64];
	UINT_32 l[16];
} SHA1_WORKSPACE_BLOCK,*PSHA1_WORKSPACE_BLOCK;

typedef struct _SHA1
{
	UINT_32 m_state[5];
	UINT_32 m_count[2];
	UINT_32 __reserved1[1];
	UINT_8  m_buffer[64];
	UINT_8  m_digest[20];
	UINT_32 __reserved2[3];

	// Member variables
	UINT_8 m_workspace[64];
	PSHA1_WORKSPACE_BLOCK m_block; // SHA1 pointer to the byte array above

}SHA1,*PSHA1;

void Initialize(PSHA1 Sha1);
void ShaReset(PSHA1 Sha1);

// Update the hash value
void Update(PSHA1 Sha1, UINT_8 *data, UINT_32 len);

// Finalize hash and report
void Finalize(PSHA1 Sha1);

// Private SHA-1 transformation
void ShaTransform(PSHA1 Sha1,UINT_32 *state, UINT_8 *buffer);

// After calling Finalize() you can get the signature by calling this function
// It returns signature in raw (binary) form.
BOOLEAN GetBinSignature( PSHA1 Sha1, UINT_8 *Buffer );

// After calling Finalize() you can get the signature by calling this function
// It returns signature in Hexadecimal string form.
BOOLEAN GetHexSignature( PSHA1 Sha1, WCHAR *HexBuffer );

// This functin converts signature from binary form to hexadecimal representation
// in string format.
BOOLEAN BinToHex( UINT_8 *BinBuffer, UINT_32 BinBufferLen, WCHAR *HexBuffer );

// This functin converts signature from hexadecimal string format
// to binary representation in raw bytes.
BOOLEAN HexToBin( WCHAR *HexBuffer, UINT_32 HexStringLen, UINT_8 *BinBuffer );

unsigned char ToHex( TCHAR ch );

#endif __SHA1__	
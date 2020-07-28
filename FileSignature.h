#ifndef __FILESIGNATURE__
#define __FILESIGNATURE__

NTSTATUS ShaHashFile( PUNICODE_STRING FileName, unsigned char* Digest);

#endif __FILESIGNATURE__
#include "Pch.h"
#include "SHA1.h"
#include "FileSignature.h"

NTSTATUS ShaHashFile( PUNICODE_STRING FileName, unsigned char* Digest)
{
	NTSTATUS Status = STATUS_SUCCESS;
	OBJECT_ATTRIBUTES ObjectAttributes;
	HANDLE Handle;
	IO_STATUS_BLOCK IoStatus;
	PVOID Buffer;
	SHA1 Sha1;
	LARGE_INTEGER Offset;
	
	__try
	{
		//*********************************************************
		// Open file
		InitializeObjectAttributes( &ObjectAttributes, FileName, 
			OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE , NULL, NULL );

		Status = ZwOpenFile( &Handle, GENERIC_READ | SYNCHRONIZE, &ObjectAttributes, 
			&IoStatus, FILE_SHARE_READ, FILE_SYNCHRONOUS_IO_NONALERT);
		if( !NT_SUCCESS(Status) )
			return Status;

		//*********************************************************
		// Allocate buffer to read data
		Buffer = ExAllocatePoolWithTag( NonPagedPool, MAX_BUFFER_SIZE, CT_POOL_TAG );
		if( Buffer==NULL )
		{
			ZwClose( Handle );
			return STATUS_INSUFFICIENT_RESOURCES;
		}

		//********************************************************
		// Read data from file and calculate SHA1 signature
		Initialize( &Sha1 );
		Offset.HighPart = -1;
		Offset.LowPart = FILE_USE_FILE_POINTER_POSITION;
		while(1)
		{
			Status = ZwReadFile( Handle, NULL, NULL, NULL,&IoStatus, 
				Buffer, MAX_BUFFER_SIZE, NULL, 0);
			
			if( STATUS_END_OF_FILE == Status )
				break;

			if( !NT_SUCCESS(Status) )
			{
				ExFreePool( Buffer );
				ZwClose( Handle );
				return Status;
			}

			Update( &Sha1, Buffer, IoStatus.Information );
			if( IoStatus.Information < MAX_BUFFER_SIZE )
				break;
		}
		Finalize( &Sha1 );
		GetBinSignature( &Sha1, Digest );

		//*******************************************************
		ExFreePool( Buffer );
		//close file
		ZwClose( Handle );
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("###ERR: Exception caught in ShaHashFile().Reason:-Unknown\n");
		if(Buffer) ExFreePool(Buffer);
		if(Handle) ZwClose( Handle );
		return STATUS_UNHANDLED_EXCEPTION;
	}

	return STATUS_SUCCESS;
}

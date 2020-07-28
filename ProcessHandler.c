#include "Pch.h"
#include "SHA1.h"
#include "ProcessHandler.h"
#include "FileSignature.h"
#include "ntstrsafe.h"

UCHAR malware_signature[SIGNATURE_SIZE] = {0x13,0xaa,0xef,0x86,0xf4,0x50,0xd0,0x09,0x74,0xcc,0x03,0x2f,0xfa,0x7c,0x85,0xcb,0xea,0x30,0x91,0xa0};

VOID CreateProcessNotificationRoutine (
    IN HANDLE  ParentId,
    IN HANDLE  ProcessId,
    IN BOOLEAN  Create )
{
	NTSTATUS Status = STATUS_SUCCESS;
	PUNICODE_STRING usPath = NULL;
	UCHAR Signature[SIGNATURE_SIZE] = {0};
	PDEVICE_EXTENSION deviceExtension = g_FilterCDO->DeviceExtension;

	UNREFERENCED_PARAMETER( ParentId );

	DbgPrint("CtFilter : CreateProcessNotificationRoutine : Enter\n");
	//***************************************************************
	// 1. Remove process from trusted application list when process dies.
	if(!Create)
	{
		return;
	}

	//***************************************************************
	// 2. Get full path of the executable.
	usPath = (PUNICODE_STRING)ExAllocatePoolWithTag( NonPagedPool, (1024 + sizeof(UNICODE_STRING) + 1), CT_POOL_TAG);
	if(NULL == usPath)
		return;

	RtlZeroMemory( usPath, (1024 + sizeof(UNICODE_STRING) + 1));
	usPath->MaximumLength = 1024;
	if( FALSE == GetProcessNameFromId( ProcessId, usPath ) )
	{
		DbgPrint("CtFilter : CreateProcessNotificationRoutine : GetProcessNameFromId failed\n");
		return;
	}

	DbgPrint("CtFilter : CreateProcessNotificationRoutine : Process = %wZ\n",usPath);

	//***************************************************************
	// 3. Get signature of the executable.
	Status = ShaHashFile( usPath, Signature );
	if( !NT_SUCCESS(Status) )
	{
		DbgPrint("CtFilter : CreateProcessNotificationRoutine : ShaHashFile failed\n");
		return;
	}

	if(usPath)
	{
		ExFreePool(usPath);
	}
	
	//***************************************************************
	// 4. Compare executable signature with malware signature.
	if (SIGNATURE_SIZE == RtlCompareMemory(malware_signature, Signature, SIGNATURE_SIZE))
	{
		DbgPrint("\n*************************** ALERT ************************************\n");
		DbgPrint("CtFilter : CreateProcessNotificationRoutine : ALERT MALWARE FOUND !!!!!!!!!!!!!!\n");
		DbgPrint("***************************************************************\n");
		return;
	}

	return;
}

BOOLEAN GetProcessNameFromId( 
	HANDLE hProcessId, 
	PUNICODE_STRING Path)
{
	NTSTATUS Status = STATUS_SUCCESS;
	HANDLE handle;
	OBJECT_ATTRIBUTES objectAttributes;
	CLIENT_ID  ClientId;
	ULONG cbRet=0;

	// Get the full path name of the executable.
	InitializeObjectAttributes (&objectAttributes, 0, 0, 0 , 0);

    ClientId.UniqueProcess=hProcessId;
    ClientId.UniqueThread=0;

	Status=ZwOpenProcess(&handle, STANDARD_RIGHTS_READ/*PROCESS_ALL_ACCESS*/, &objectAttributes, &ClientId);
	if( !NT_SUCCESS(Status) ){
		DbgPrint("CtFilter : GetProcessNameFromId : Error in opening process ErrorCode: %d\n", Status);
		return FALSE;
	}

	Status = ZwQueryInformationProcess(
		handle,
		ProcessImageFileName,
		Path,
		Path->MaximumLength,
		&cbRet);

	if(!NT_SUCCESS(Status))
	{
		DbgPrint("CtFilter : GetProcessNameFromId : Error in getting path ErrorCode: %d Required = %d\n", Status, cbRet);
		ZwClose( handle );
		return FALSE;
	}

	ZwClose( handle );
	
	return TRUE;
}

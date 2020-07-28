#include "Pch.h"
#include "ProcessHandler.h"
#include "Sha1.h"
#include "FileSignature.h"
#include <ntstrsafe.h>


PDEVICE_OBJECT g_FilterCDO;
extern CONST FLT_REGISTRATION FilterRegistration;

//
//  Assign text sections for each routine.
//

#ifdef ALLOC_PRAGMA
#pragma alloc_text(INIT, DriverEntry)

#pragma alloc_text(PAGE, Unload)
#endif


NTSTATUS DriverEntry( __in PDRIVER_OBJECT DriverObject,
				__in PUNICODE_STRING RegistryPath)
{
    NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_EXTENSION deviceExtension = NULL;
	UNREFERENCED_PARAMETER( RegistryPath );

	DbgPrint("CtFilter : DriverEntry: Start to load driver CtFilter\n");

    //
    //  Creating control device object
    //
    status = CreateControlDeviceObject( DriverObject );
    if (!NT_SUCCESS( status )) 
	{
        return status;
    }

	deviceExtension = (PDEVICE_EXTENSION)DriverObject->DeviceObject->DeviceExtension;

	status = PsSetCreateProcessNotifyRoutine(CreateProcessNotificationRoutine,FALSE);

	if (status != STATUS_SUCCESS) 
	{
		DeleteControlDeviceObject();
        return status;
	}
	

    //  Register with FltMgr to tell it our callback routines
    //

    status = FltRegisterFilter( DriverObject,
                                &FilterRegistration,
                                &deviceExtension->Filter );

    if (!NT_SUCCESS( status )) 
	{
		PsSetCreateProcessNotifyRoutine(CreateProcessNotificationRoutine,TRUE);
		DeleteControlDeviceObject();
        return status;
    }

    //
    //  Start filtering i/o
    //

    status = FltStartFiltering( deviceExtension->Filter );

    if (!NT_SUCCESS( status )) 
	{
        FltUnregisterFilter( deviceExtension->Filter );
		PsSetCreateProcessNotifyRoutine(CreateProcessNotificationRoutine,TRUE);
        DeleteControlDeviceObject();
        return status;
    }

	DbgPrint("CtFilter : DriverEntry: CtFilter driver loaded successfully\n");
	return status;

}

NTSTATUS Unload ( __in FLT_FILTER_UNLOAD_FLAGS Flags )
{
	PDEVICE_EXTENSION deviceExtension = g_FilterCDO->DeviceExtension;


    PAGED_CODE();

    UNREFERENCED_PARAMETER( Flags );

	DbgPrint("CtFilter : Unload : Unloading driver\n");
	
	FltUnregisterFilter( deviceExtension->Filter );
    deviceExtension->Filter = NULL;

	DbgPrint("CtFilter : Unload : Unloading driver complete\n");
	
    return STATUS_SUCCESS;
}

VOID DriverUnload( PDRIVER_OBJECT Driver)
{
	UNREFERENCED_PARAMETER( Driver );

	DbgPrint("CtFilter : DriverUnload : Unloading driver\n");
	DeleteControlDeviceObject();
	DbgPrint("CtFilter : DriverUnload : Unloading driver complete\n");
}


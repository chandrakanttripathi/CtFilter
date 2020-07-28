#include "Pch.h"
#include "ProcessHandler.h"
#include "FileSignature.h"

//
//  Assign text sections for each routine.
//

#ifdef  ALLOC_PRAGMA
    #pragma alloc_text( PAGE,     CreateControlDeviceObject)
    #pragma alloc_text( PAGE,     DeleteControlDeviceObject)
    #pragma alloc_text( PAGE,     MajorFunction)
#endif


NTSTATUS CreateControlDeviceObject( __inout PDRIVER_OBJECT DriverObject )
{
    NTSTATUS			status;
    UNICODE_STRING		nameString;
	UNICODE_STRING		DosNameString;
    ULONG				i;
	PDEVICE_EXTENSION	deviceExtension;

    PAGED_CODE();

    //
    // Create our control device object
    //
	DbgPrint("CtFilter : CreateControlDeviceObject : Creating Filter Oobject..\n");

    RtlInitUnicodeString( &nameString, NT_CONTROL_DEVICE_OBJECT_NAME );
    status = IoCreateDevice( DriverObject,
                             sizeof(DEVICE_EXTENSION),
                             &nameString,
                             FILE_DEVICE_DISK_FILE_SYSTEM,
                             FILE_DEVICE_SECURE_OPEN,
                             FALSE,
                             &g_FilterCDO);

    if ( !NT_SUCCESS( status ) ) {
		DbgPrint("CtFilter : CreateControlDeviceObject : IoCreateDevice failed with status 0x%x.\n", status);
        return status;
    }

    RtlInitUnicodeString(&DosNameString, DOS_CONTROL_DEVICE_OBJECT_NAME);

    status = IoCreateSymbolicLink(&DosNameString, &nameString);

    if ( !NT_SUCCESS(status) )
    {
		DbgPrint("CtFilter : CreateControlDeviceObject : IoCreateSymbolicLink failed..\n");
    }

	deviceExtension = g_FilterCDO->DeviceExtension;
	RtlZeroMemory( deviceExtension, sizeof(DEVICE_EXTENSION) );

    //
    // Initialize the driver object with this driver's entry points.
    // Most are simply passed through to some other device driver.
    //

    for (i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++) {

        DriverObject->MajorFunction[i] = MajorFunction;
    }

	DriverObject->DriverUnload = DriverUnload;

	DbgPrint("CtFilter : CreateControlDeviceObject : Creating File Filter successful..\n");

    return STATUS_SUCCESS;
}



VOID DeleteControlDeviceObject(VOID)
{
	UNICODE_STRING SymbolicName;

    PAGED_CODE();

	PsSetCreateProcessNotifyRoutine(CreateProcessNotificationRoutine,TRUE);

   //
    // Delete the symbolic link
    //
    RtlInitUnicodeString(&SymbolicName, DOS_CONTROL_DEVICE_OBJECT_NAME);
    IoDeleteSymbolicLink(&SymbolicName);

    //
    // Delete our control device object
    //
	DbgPrint("CtFilter : DeleteControlDeviceObject : Deleting Device Control..\n");
    IoDeleteDevice( g_FilterCDO );
}


NTSTATUS MajorFunction(
    __inout PDEVICE_OBJECT DeviceObject,
    __inout PIRP Irp
    )
/*++

Routine Description:

    This routine handles the IRPs that are directed to the control
    device object.

Arguments:

    DeviceObject - control device object
    Irp          - the current Irp to process

Return Value:

    Returns STATUS_INVALID_DEVICE_REQUEST if the CDO doesn't support that request
    type, or the appropriate status otherwise.

--*/
{
    NTSTATUS status;
    PIO_STACK_LOCATION irpSp;
	PULONG Key;

    UNREFERENCED_PARAMETER( DeviceObject );

    PAGED_CODE();

    ASSERT( IS_MY_CONTROL_DEVICE_OBJECT( DeviceObject ) );


    //
    //  default to success
    //

    status = STATUS_SUCCESS;

    irpSp = IoGetCurrentIrpStackLocation(Irp);

	DbgPrint("CtFilter : DeleteControlDeviceObject : [MajorFunction entry ( Irp = %p, irpSp->MajorFunction = 0x%x )\n",
                 Irp,
                 irpSp->MajorFunction);
    return status;
}

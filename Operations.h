#ifndef __OPERATIONS__
#define __OPERATIONS__

NTSTATUS CreateControlDeviceObject( __inout PDRIVER_OBJECT DriverObject);

VOID DeleteControlDeviceObject(VOID);

NTSTATUS MajorFunction(
    __inout PDEVICE_OBJECT DeviceObject,
    __inout PIRP Irp
    );

#endif __OPERATIONS__
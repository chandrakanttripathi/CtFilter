#ifndef __CT_FILTER__
#define __CT_FILTER__

// control device object extension
typedef struct _DEVICE_EXTENSION
{

    //
    //  Handle to minifilter returned from FltRegisterFilter()
    //

    PFLT_FILTER Filter;

	// Can use this Flag to enable and disable scanning mode
	BOOLEAN ScanningMode;

}DEVICE_EXTENSION,*PDEVICE_EXTENSION;

//
//  Control Device Object for this filter
//
extern PDEVICE_OBJECT g_FilterCDO;

//
//  Macro to test if this is my control device object
//

#define IS_MY_CONTROL_DEVICE_OBJECT(_devObj) \
    (((_devObj) == g_FilterCDO) ? \
            (ASSERT(((_devObj)->DriverObject == g_FilterCDO->DriverObject)) \
                    , TRUE) : \
            FALSE)
			
//
//  The name of filter
//

#define NT_CONTROL_DEVICE_OBJECT_NAME               L"\\FileSystem\\Filters\\NtCTFilter"
#define DOS_CONTROL_DEVICE_OBJECT_NAME              L"\\DosDevices\\DosCTFilter"

//---------------------------------------------------------------------------
//  Function prototypes
//---------------------------------------------------------------------------

NTSTATUS DriverEntry(
    __in PDRIVER_OBJECT DriverObject,
    __in PUNICODE_STRING RegistryPath
    );

NTSTATUS Unload (
    __in FLT_FILTER_UNLOAD_FLAGS Flags
    );

VOID  DriverUnload( 
	__in PDRIVER_OBJECT Driver
	);


#define CT_POOL_TAG		'TC'

#endif __CT_FILTER__
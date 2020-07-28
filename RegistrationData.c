#include <fltKernel.h>
#include <suppress.h>
#include "CtFilter.h"


//---------------------------------------------------------------------------
//  Registration information for FLTMGR.
//---------------------------------------------------------------------------

//
//  Tells the compiler to define all following DATA and CONSTANT DATA to
//  be placed in the INIT segment.
//

#ifdef ALLOC_DATA_PRAGMA
    #pragma data_seg("INIT")
    #pragma const_seg("INIT")
#endif

CONST FLT_OPERATION_REGISTRATION Callbacks[] = {
    { IRP_MJ_OPERATION_END }
};

//
//  This defines what we want to filter with FltMgr
//
CONST FLT_REGISTRATION FilterRegistration = {

    sizeof(FLT_REGISTRATION),   //  Size
    FLT_REGISTRATION_VERSION,   //  Version
    0,                          //  Flags

    NULL,                       //  Context
    Callbacks,                  //  Operation callbacks

    Unload,                   //  FilterUnload

    NULL,                       //  InstanceSetup
    NULL,                       //  InstanceQueryTeardown
    NULL,                       //  InstanceTeardownStart
    NULL,                       //  InstanceTeardownComplete

    NULL,                       //  GenerateFileName
    NULL,                       //  GenerateDestinationFileName
    NULL                        //  NormalizeNameComponent

};

//
//  Tells the compiler to restore the given section types back to their previous
//  section definition.
//

#ifdef ALLOC_DATA_PRAGMA
    #pragma data_seg()
    #pragma const_seg()
#endif


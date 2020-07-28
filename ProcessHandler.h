#ifndef __PROCESS_HANDLER_H__
#define __PROCESS_HANDLER_H__


VOID CreateProcessNotificationRoutine (
    IN HANDLE  ParentId,
    IN HANDLE  ProcessId,
    IN BOOLEAN  Create
    );

NTSTATUS ZwQueryInformationProcess(
	HANDLE ProcessHandle,
	PROCESSINFOCLASS ProcessInformationClass,
	PVOID ProcessInformation,
	ULONG ProcessInformationLength,
	PULONG ReturnLength
	);

BOOLEAN GetProcessNameFromId( 
	HANDLE hProcessId, 
	PUNICODE_STRING Path
	);

#endif //__PROCESS_HANDLER_H__
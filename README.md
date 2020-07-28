# CtFilter
Mini Filter driver for filtering executing binary based on signature.


Currently this code is tested with Windows 7.
Environment:
WDK 7600.16385.1

Compilation Steps:
1. Start driver environment command prompt
2. Go to source code folder.
3. Run command "build -ceZ"
4. This will compile source and generate .sys and .pdb files in bin folder

Installation Process
1. Copy .sys file and inf\CtFilter.inf file in a folder on machine to install.
2. Right click inf file and click install.
3. Copy CtFilter.sys to system32/drivers folder.
4. Open command prompt in administration mode.
5. Type command "fltmc load CtFilter"

Enable Debug Print on machine
1. Open regedit.exe in administrator mode.
2. Create Key "Debug Print Filter" at location "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager"
3. Create DWORD value named "DEFAULT" with value 0xffffffff
4. Restart machine
5. This will enable DbgPrint on machine and debug prints can be viewed in WinDbg or DebugView.

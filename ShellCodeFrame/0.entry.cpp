#include "header.h"
#pragma comment(linker,"/entry:EntryMain")
void EntryMain()
{
	CreateShellCode(); 
}

void CreateShellCode()
{
	HANDLE hFile = CreateFileA("ShellCode.bin", GENERIC_ALL, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (hFile==INVALID_HANDLE_VALUE)
	{
		MessageBoxA(NULL, "CreateFileA Error", "Error", MB_ERR_INVALID_CHARS);
		return;
	}
	DWORD dwSize = (DWORD)ShellCodeEnd - (DWORD)ShellCodeStart;
	DWORD dwWrite = 0;;
	WriteFile(hFile, ShellCodeStart, dwSize, &dwWrite,NULL);
	CloseHandle(hFile);
}
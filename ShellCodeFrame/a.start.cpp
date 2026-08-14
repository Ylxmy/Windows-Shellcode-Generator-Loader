#include "header.h"
#include "api.h"

__declspec(naked)void ShellCodeStart()
{
	__asm
	{
		jmp ShellCodeEntry;
	}
}

void ShellCodeEntry()
{
	Functions fn;
	Initfunctions(&fn);
	char szText[] = { 'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', 0 };
	char szCaption[] = { 'T', 'i', 'p', 0 };
	fn.fnMessageBoxA(NULL, szText, szCaption, MB_OK);
}
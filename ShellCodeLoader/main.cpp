#include <stdio.h>
#include <windows.h>

int main(int argc, char* argv[])
{
    // Open the ShellCode file for reading
    HANDLE hFile = CreateFileA("ShellCode.bin", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        printf("Error: Failed to open ShellCode.bin\n");
        return -1;
    }

    // Get the size of the ShellCode file
    DWORD dwFileSize = GetFileSize(hFile, NULL);
    if (dwFileSize == INVALID_FILE_SIZE)
    {
        printf("Error: Failed to determine file size\n");
        CloseHandle(hFile);
        return -1;
    }

    // Allocate memory with read, write, and execute permissions
    LPVOID lpBuffer = VirtualAlloc(NULL, dwFileSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (lpBuffer == NULL)
    {
        printf("Error: Failed to allocate executable memory\n");
        CloseHandle(hFile);
        return -1;
    }

    // Read the ShellCode into the allocated memory
    DWORD dwBytesRead = 0;
    if (!ReadFile(hFile, lpBuffer, dwFileSize, &dwBytesRead, NULL) || dwBytesRead != dwFileSize)
    {
        printf("Error: Failed to read ShellCode into memory\n");
        VirtualFree(lpBuffer, 0, MEM_RELEASE);
        CloseHandle(hFile);
        return -1;
    }

    // Execute the ShellCode
    __asm
    {
        call lpBuffer
    }

    // Clean up (note: this line may not be reached if ShellCode doesn't return)
    VirtualFree(lpBuffer, 0, MEM_RELEASE);
    CloseHandle(hFile);
    return 0;
}
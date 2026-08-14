# ShellCode

A comprehensive Windows shell code development framework for creating location-independent executable code with dynamic API analysis. This project provides tools for creating shell code with advanced evasion techniques.

## Features

- **Position-Independent Code Generation**: Creates shellcode that can execute from any memory location
- **Dynamic API Resolution**: Uses hash-based API resolution to avoid static imports
- **Cross-Architecture Support**: Compatible with x86, x64, and ARM architectures
- **Evasion Techniques**: Implements advanced techniques to bypass security mechanisms
- **Complete Toolchain**: Includes both generator and loader components
- **Minimal Dependencies**: Self-contained framework with no external runtime dependencies

## Installation

### Build

- Download the project to your computer.
- Extract the Project to a Folder.
- Download Visual Studio to your computer
- Open the solution file (.sln).

3. **Build the projects:**
   - Set configuration to `Release` for optimized shellcode
   - Build `ShellCodeFrame` project first
   - Build `ShellCodeLoader` project second

4. **Verify build output:**
   ```
   Release/
   ├── ShellCode.bin          # Generated shellcode binary
   ├── ShellCodeFrame.exe     # Shellcode generator
   └── ShellCodeLoader.exe    # Shellcode loader/tester
   ```

## Quick Start

### Generate Shellcode

```cpp
// Run the ShellCodeFrame generator
./Release/ShellCodeFrame.exe

// This creates ShellCode.bin containing your position-independent code
```

### Test Shellcode

```cpp
// Load and execute the generated shellcode
./Release/ShellCodeLoader.exe

// Expected output: Message box displaying "Hello GuiShou"
```

### Basic Integration

```cpp
#include <windows.h>

int main() {
    // Load shellcode from file
    HANDLE hFile = CreateFileA("ShellCode.bin", GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    DWORD dwSize = GetFileSize(hFile, NULL);
    
    // Allocate executable memory
    LPVOID lpBuffer = VirtualAlloc(NULL, dwSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    
    // Read and execute
    DWORD dwRead;
    ReadFile(hFile, lpBuffer, dwSize, &dwRead, NULL);
    ((void(*)())lpBuffer)();  // Execute shellcode
    
    // Cleanup
    VirtualFree(lpBuffer, 0, MEM_RELEASE);
    CloseHandle(hFile);
    return 0;
}
```

## Project Structure

```
ShellCode/
├── ShellCodeFrame/          # Main shellcode generator
│   ├── 0.entry.cpp          # Entry point and file generation
│   ├── a.start.cpp          # Shellcode entry and initialization
│   ├── b.work.cpp           # Core functionality and API resolution
│   ├── z.end.cpp            # Shellcode termination marker
│   ├── api.h                # API function definitions and structures
│   ├── hash.h               # Pre-computed API hashes
│   └── header.h             # Common headers and declarations
├── ShellCodeLoader/         # Shellcode testing utility
│   └── main.cpp             # Loader implementation
└── README.md                # This file
```

## Usage

### Custom Shellcode Development

1. **Modify the payload** in `a.start.cpp`:
   ```cpp
   void ShellCodeEntry() {
       Functions fn;
       Initfunctions(&fn);
       
       // Your custom shellcode logic here
       char szMessage[] = {'Y','o','u','r',' ','m','e','s','s','a','g','e',0};
       char szTitle[] = {'T','i','t','l','e',0};
       fn.fnMessageBoxA(NULL, szMessage, szTitle, MB_OK);
   }
   ```

2. **Add new API functions** in `api.h`:
   ```cpp
   typedef BOOL (WINAPI *pfnCreateProcessA)(/* parameters */);
   
   typedef struct _FUNCTIONS {
       pfnLoadLibraryA fnLoadLibraryA;
       pfnMessageBoxA fnMessageBoxA;
       pfnCreateProcessA fnCreateProcessA;  // New function
   } Functions, *Pfunctions;
   ```

3. **Calculate API hashes** and add to `hash.h`:
   ```cpp
   #define HASH_CreateProcessA 0x12345678  // Calculate using hash algorithm
   ```

4. **Initialize new functions** in `b.work.cpp`:
   ```cpp
   void Initfunctions(Pfunctions pfn) {
       pfn->fnLoadLibraryA = (pfnLoadLibraryA)GetProcAddressWithHash(HASH_LoadLibraryA);
       pfn->fnCreateProcessA = (pfnCreateProcessA)GetProcAddressWithHash(HASH_CreateProcessA);
   }
   ```

### Advanced Configuration

#### Memory Layout Control
```cpp
// Adjust memory sections in project settings
#pragma comment(linker, "/SECTION:.text,ERW")
#pragma comment(linker, "/MERGE:.rdata=.text")
#pragma comment(linker, "/MERGE:.data=.text")
```

#### Anti-Analysis Features
```cpp
// Add obfuscation techniques
#define OBFUSCATE_STRING(str) /* Your obfuscation method */
#define ANTI_DEBUG_CHECK() /* Your anti-debug code */
```

## Technical Details

### Hash-Based API Resolution

The framework uses a custom hash algorithm to resolve Windows API functions dynamically:

```cpp
DWORD CalculateHash(const char* functionName, const char* moduleName) {
    DWORD hash = 0;
    // ROR13 hash algorithm implementation
    while (*functionName) {
        hash = ROTR32(hash, 13);
        hash += *functionName++;
    }
    return hash + moduleHash;
}
```

### Memory Protection Bypass

The loader demonstrates several memory protection bypass techniques:

1. **VirtualAlloc with PAGE_EXECUTE_READWRITE**
2. **Direct memory execution via function pointer**
3. **Inline assembly for precise control**

### Cross-Architecture Compatibility

```cpp
#if defined(_WIN64)
    PebAddress = (PPEB)__readgsqword(0x60);  // x64
#elif defined(_M_ARM)
    PebAddress = (PPEB)((ULONG_PTR)_MoveFromCoprocessor(15, 0, 13, 0, 2) + 0);  // ARM
#else
    PebAddress = (PPEB)__readfsdword(0x30);  // x86
#endif
```

## Code Examples

### Basic Message Box Shellcode

```cpp
void ShellCodeEntry() {
    Functions fn;
    Initfunctions(&fn);
    
    // Obfuscated strings to avoid static analysis
    char szText[] = {'H','e','l','l','o',' ','W','o','r','l','d',0};
    char szCaption[] = {'S','h','e','l','l','c','o','d','e',0};
    
    fn.fnMessageBoxA(NULL, szText, szCaption, MB_OK);
}
```

### Process Creation Shellcode

```cpp
void ShellCodeEntry() {
    Functions fn;
    Initfunctions(&fn);
    
    // Load kernel32.dll for CreateProcessA
    char szKernel32[] = {'k','e','r','n','e','l','3','2','.','d','l','l',0};
    fn.fnLoadLibraryA(szKernel32);
    
    // Get CreateProcessA function
    pfnCreateProcessA fnCreateProcessA = (pfnCreateProcessA)GetProcAddressWithHash(HASH_CreateProcessA);
    
    // Execute notepad.exe
    char szNotepad[] = {'n','o','t','e','p','a','d','.','e','x','e',0};
    STARTUPINFOA si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(si);
    
    fnCreateProcessA(NULL, szNotepad, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
}
```

### Network Communication Shellcode

```cpp
void ShellCodeEntry() {
    Functions fn;
    Initfunctions(&fn);
    
    // Load ws2_32.dll for network functions
    char szWs2_32[] = {'w','s','2','_','3','2','.','d','l','l',0};
    HMODULE hWs2_32 = fn.fnLoadLibraryA(szWs2_32);
    
    // Initialize Winsock and create reverse shell
    // ... implementation details
}
```

## Configuration

### Hash Generation

To add new API functions, calculate their hashes using this algorithm:

```python
def calculate_hash(function_name, module_name):
    def ror32(value, shift):
        return ((value >> shift) | (value << (32 - shift))) & 0xFFFFFFFF
    
    # Calculate module hash
    module_hash = 0
    for char in module_name.upper():
        module_hash = ror32(module_hash, 13)
        module_hash = (module_hash + ord(char)) & 0xFFFFFFFF
    
    # Calculate function hash
    function_hash = 0
    for char in function_name:
        function_hash = ror32(function_hash, 13)
        function_hash = (function_hash + ord(char)) & 0xFFFFFFFF
    
    return (function_hash + module_hash) & 0xFFFFFFFF

# Example usage
hash_value = calculate_hash("MessageBoxA", "USER32.DLL")
print(f"#define HASH_MessageBoxA 0x{hash_value:08X}")
```

### Code Style

```cpp
// Use descriptive variable names
DWORD dwFunctionHash = 0;

// Comment complex algorithms
// ROR13 hash algorithm for API resolution
hash = ROTR32(hash, 13);

// Maintain consistent indentation
if (condition) {
    // Action
}
```


## Disclaimer

This software is intended for educational and research purposes only. The authors accept no responsibility for any misuse of this software.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
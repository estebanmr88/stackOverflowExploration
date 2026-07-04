# C++ stack exploration and flow hijacking

This repository contains an educational C++ application designed to demonstrate the internal mechanics of a **Stack-based Buffer Overflow** vulnerability. Unlike standard command-line parameter exploits, this program demonstrates how data read from an external untrusted file can be weaponized to leak memory layouts and hijack execution flow.

### Vulnerability

1. **Stack introspection:**
   * The program prints out the raw values stored directly above the local buffer. This acts as a debugger wrapper, visualizing the contents of the stack frame, including the **Saved Frame Pointer (RBP)** and the **Saved Return Address (RIP)**.

2. **Memory corruption:**
   * The application opens and reads up to 128 bytes from an external text file into a buffer. 
   * It passes this data to `Foo()`, where `memcpy` runs. This instruction contains an off-by-two length bug (`strlen(sz)+2`), but more importantly, it fails to ensure that the source string fits within the local allocation bounds.

3. **Instruction redirection:**
   * By structuring a payload in the .txt file that fills the 24 bytes of `buf`, overwrites the 8 bytes of the saved frame pointer, and overwrites the adjacent return address with the function pointer of `Bar()`, an operator can force the application to jump to `Bar()` upon `Foo()`'s termination.

### Compilation flags and defenses
To successfully observe this exploitation pattern, modern compiler protection mechanisms must be explicitly deactivated. The project includes compiler guidance for both GCC/Clang and MSVC:

* **GCC/Clang:** Must be compiled with `-fno-stack-protector` (disables stack canaries) and `-std=c++11`.
* **MSVC (Windows):** Requires disabling Security Checks (`/GS-`), disabling Control Flow Guard (CFG), and utilizing `#define _CRT_SECURE_NO_WARNINGS` to bypass standard boundary deprecation alerts.

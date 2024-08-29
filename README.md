# Override

This project aims to further your knowledge in the world of elf-like binary exploitation in i386 system

## General instructions
#### Connection to the Rainfall VM (iso provided by the subject):
`user: level00`
`password: level00`
#### get CPU info:
```bash
lscpu
cat /proc/cpuinfo
```

#### Goal
Find a way to read the `.pass` file of the `level0X` user (0X = number of the next level). `.pass` located at the home `/home/user/level0X/.pass`.

Exploit vulnerabilities in the binary of the current level to gain access to the next level. Binary are owned by the next user and have the `s` permission exection (set uid of owner for execution).

## Display kernel protection informations:
```bash
checksec --kernel
sysctl kernel.randomize_va_space
cat /proc/sys/kernel/randomize_va_space
```

```console
GCC stack protector support:              Enabled
  Strict user copy checks:                Disabled
  Enforce read-only kernel data:          Enabled
  Restrict /dev/mem access:               Enabled
  Restrict /dev/kmem access:              Enabled
* grsecurity / PaX: No GRKERNSEC
* Kernel Heap Hardening: No KERNHEAP
kernel.randomize_va_space = 0
```

- Stack protector: Protects against stack buffer overflows (works by placing a canary)
- User copy checks: Protects against user-space memory corruption (prevent copying data between user-space and kernel-space)
- Restrict /dev/mem access: Prevents direct access to physical memory
- Restrict /dev/kmem access: Prevents direct access to kernel memory
- PaX (Patchset for X): Is an extensive security enhancement to the Linux kernel that defends against memory corruption-based exploit and other.
- Kernel Heap Hardening: Protects against heap corruption
- ASLR (Address Space Layout Randomization): Randomizes the memory addresses of the stack, heap, and libraries to make it harder for attackers to predict the location of code or data in memory.

## Display elf binary protection informations:
```bash
checksec --dir .
```

Output (given at login for `level00`):

```console
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   No canary found   NX enabled    No PIE          No RPATH   No RUNPATH   .level00
```

- RELRO (RELocation Read-Only) marks the GOT (Global Offset Table) as read-only, preventing an attacker from overwriting function pointers.
  > - No RELRO: The GOT is not read-only.
  > - Partial RELRO: The GOT is read-only, but the dynamic linker's symbol table is not.
  > - Full RELRO: Both the GOT and the dynamic linker's symbol table are read-only.

  > The GOT is a table of pointers that point to functions or variables in shared libraries. It is used to resolve addresses of functions or variables at runtime. The PLT (Procedure Linkage Table) perfoms the linking between the call instruction and the GOT.



- Canaries helps prevent buffer overflow attacks. They are values placed on the stack that are checked before a function returns to ensure that they have not been modified by a buffer overflow.
- NX (No eXecute) marks certain areas of memory as non-executable.
- PIE (Position Independent Executable) means the program can load dependencies at different memory locations each time it's executed, preventing hardcoded addresses (But the offsets between parts of the program remain the same - relative addresses).
- RPATH and RUNPATH are used to specify the directories in which the dynamic linker should search for shared libraries. (Set to for standard locations)

> See how PIE, ASLR, Relro GOT and PLT works together and how it is exploitable in [this article](https://ir0nstone.gitbook.io/notes/types/stack/aslr/plt_and_got).

## Tools for Working with ELF Files

- `readelf`: Displays information about ELF files, including headers, sections, and segments.
- `objdump`: Provides detailed information about object files, including disassembly of executable code.
- `nm`: Lists symbols from object files.
- `ldd`: Prints shared library dependencies of an ELF executable.
- `strace`: Tracks system calls and signals.
- `ltrace`: Tracks library calls.
- `gdb`: The GNU Debugger.
- `checksec`: A script to check the security properties of an ELF binary.
- `strings`: Prints the printable strings in an object file.
- `file`: Determines the type of an object file.
- `hexdump`: Displays the contents of an object file in hexadecimal format.
- `xxd`: A hexdump utility that can output hexdumps in a variety of formats.
- `pwntools`: A CTF framework and exploit development library.


### Hints:
  > __See ROP (Return Oriented Programing) sush as Ret2Libc__


### References
- [wiki .bss](https://en.wikipedia.org/wiki/.bss)
- [wiki GOT](https://en.wikipedia.org/wiki/Global_Offset_Table)
- [checksec infos](https://opensource.com/article/21/6/linux-checksec)
- [ctf 101: binary exploitation](https://ctf101.org/binary-exploitation/overview/)
- [hacktricks: binary exploitation](https://book.hacktricks.xyz/binary-exploitation/basic-stack-binary-exploitation-methodology)
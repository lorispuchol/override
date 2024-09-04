# level03
The program waits for a string input from `stdin`

## Step 1: Understand the program
1. The `main` function reads the input and converts the input into an integer with `scanf`

2. $(int)result = 322424845 - (int)intput$

3. The function `test()` tests if `result` is equal to `[1-9]` or `[16-21]`
    - If true, `decrypt()` is called with `result` as argument
    - If false, `decrypt()` is called with a random value

4. `decrypt()` performs an operation on each element of the string `str` <__Q}|u`sfg~sf{}|a3__>  
    - $str[i] = str[i] \oplus result$  
    Then
        - If transformed `str` == `Congratulations!`:  __a shell is run__
        - Else: the program terminates

5. So, we have this equation:
    - $`Q` \oplus x = `C`$
    - $`Q` \oplus `C` = x$
    - $x = 18$

> $x$ is in decimal (base 10)  
> $`Q`$ and $`C`$ are in ASCII (base 256)

- [XOR calculator](https://xor.pw/)
- [XOR equations](https://math.stackexchange.com/questions/888866/solve-simple-xor-equation)

## Step 2
We have to ensure that $result = 18$
- $322424845 - input = 18$
- $input = 322424827$

## Exploit
```console
$ ./level03 
***********************************
*		level03		**
***********************************
Password:322424827
$ whoami
level04
$ cat /home/users/level04/.pass
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
```
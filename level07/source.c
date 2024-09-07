// Decompiled with Hex-Rays

//----- (0804861F) --------------------------------------------------------
void prog_timeout()
{
  int v0; // eax

  v0 = sys_exit(1);
}
// 804861F: using guessed type void __noreturn prog_timeout();

//----- (080485C4) --------------------------------------------------------
int clear_stdin()
{
  int result; // eax

  do
    result = getchar();
  while ( (_BYTE)result != 10 && (_BYTE)result != 0xFF );
  return result;
}

//----- (080485E7) --------------------------------------------------------
int get_unum()
{
  int v1 = 0;
  fflush(stdout);
  scanf("%u", &v1);
  clear_stdin();
  return v1;
}
// 8048500: using guessed type int __cdecl __isoc99_scanf(_DWORD, _DWORD);
// 80485E7: using guessed type int var_C[3];


//----- (08048630) --------------------------------------------------------
int store_number(int a1) // a1 = 0
{
  unsigned int unum; // [esp+18h] [ebp-10h]
  unsigned int index; // [esp+1Ch] [ebp-Ch]

  printf(" Number: ");
  unum = get_unum();
  printf(" Index: ");
  index = get_unum();
  if ( index == 3 * (index / 3) || HIBYTE(unum) == 183 )
  {
    puts(" *** ERROR! ***");
    puts("   This index is reserved for wil!");
    puts(" *** ERROR! ***");
    return 1;
  }
  else
  {
    *(_DWORD *)(a1 + 4 * index) = unum;
    return 0;
  }
}

//----- (080486D7) --------------------------------------------------------
int read_number(int a1)
{
  int index; // [esp+1Ch] [ebp-Ch]

  printf(" Index: ");
  index = get_unum();
  printf(" Number at data[%u] is %u\n", index, *(_DWORD *)(a1 + 4 * index));
  return 0;
}

//----- (08048723) --------------------------------------------------------
int main(int argc, const char **argv, const char **envp)
{
  _BYTE v6[400]; // [esp+24h] [ebp-1B8h] BYREF
  int number; // [esp+1B4h] [ebp-28h]
  char s[4]; // [esp+1B8h] [ebp-24h] BYREF

  number = 0;
  *(_DWORD *)s = 0;
  memset(v6, 0, sizeof(v6));
  while ( *argv )
  {
    memset((void *)*argv, 0, strlen(*argv));
    ++argv;
  }
  while ( *envp )
  {
    memset((void *)*envp, 0, strlen(*envp));
    ++envp;
  }
  puts(
    "----------------------------------------------------\n"
    "  Welcome to wil's crappy number storage service!   \n"
    "----------------------------------------------------\n"
    " Commands:                                          \n"
    "    store - store a number into the data storage    \n"
    "    read  - read a number from the data storage     \n"
    "    quit  - exit the program                        \n"
    "----------------------------------------------------\n"
    "   wil has reserved some storage :>                 \n"
    "----------------------------------------------------\n");
  while ( 1 )
  {
    printf("Input command: ");
    number = 1;
    fgets(s, 20, stdin);
    s[strlen(s) - 1] = 0;
    if ( !memcmp(s, "store", 5u) )
    {
      number = store_number((int)v6);
      goto LABEL_13;
    }
    if ( !memcmp(s, "read", 4u) )
    {
      number = read_number((int)v6);
      goto LABEL_13;
    }
    if ( !memcmp(s, "quit", 4u) )
      return 0;
LABEL_13:
    if ( number )
      printf(" Failed to do %s command\n", s);
    else
      printf(" Completed %s command successfully\n", s);
    *(_DWORD *)s = 0;
  }
}
// 8048723: using guessed type char s[4];
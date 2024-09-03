// Decompiled with Hex-Rays

//----- (080485F4) --------------------------------------------------------
int clear_stdin()
{
  int result; // eax

  do
    result = getchar();
  while ( (_BYTE)result != 10 && (_BYTE)result != 0xFF );
  return result;
}

//----- (08048617) --------------------------------------------------------
int get_unum()
{
  int v1[3]; // [esp+1Ch] [ebp-Ch] BYREF

  v1[0] = 0;
  fflush(stdout);
  __isoc99_scanf("%u", v1);
  clear_stdin();
  return v1[0];
}
// 8048530: using guessed type int __isoc99_scanf(const char *, ...);
// 8048617: using guessed type int var_C[3];

//----- (0804864F) --------------------------------------------------------
void __noreturn prog_timeout()
{
  int v0; // eax

  v0 = sys_exit(1);
}
// 804864F: using guessed type void __noreturn prog_timeout();

//----- (08048660) --------------------------------------------------------
// positive sp value has been detected, the output may be wrong!
int __cdecl decrypt(char a1)
{
  unsigned int i; // [esp+20h] [ebp-28h]
  unsigned int v3; // [esp+24h] [ebp-24h]
  char v4[29]; // [esp+2Bh] [ebp-1Dh] BYREF

  *(_DWORD *)&v4[17] = __readgsdword(0x14u);
  strcpy(v4, "Q}|u`sfg~sf{}|a3");
  v3 = strlen(v4); // 16
  for ( i = 0; i < v3; ++i )
    v4[i] = v4[i] ^ a1; // XOR
  if ( !strcmp(v4, "Congratulations!") )
    return system("/bin/sh");
  else
    return puts("\nInvalid Password");
}
// 8048746: positive sp value 4 has been found


//----- (08048747) --------------------------------------------------------
int __cdecl test(int a1, int a2)
{
  int result; // eax
  char v3; // al

  switch ( a2 - a1 )
  {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
      result = decrypt(a2 - a1);
      break;
    default:
      v3 = rand();
      result = decrypt(v3);
      break;
  }
  return result;
}

//----- (0804885A) --------------------------------------------------------
int __cdecl main(int argc, const char **argv, const char **envp)
{
  unsigned int v3; // eax
  int savedregs; // [esp+20h] [ebp+0h] BYREF

  v3 = time(0);
  srand(v3);
  puts("***********************************");
  puts("*\t\tlevel03\t\t**");
  puts("***********************************");
  printf("Password:");
  __isoc99_scanf("%d", &savedregs);
  test(savedregs, 322424845);
  return 0;
}
// 8048530: using guessed type int __isoc99_scanf(const char *, ...);

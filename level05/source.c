// Decompiled with Hex-Rays

//----- (08048444) --------------------------------------------------------
int main(int argc, const char **argv, const char **envp)
{
  char s[100]; // [esp+28h] [ebp-70h] BYREF
  unsigned int i; // [esp+8Ch] [ebp-Ch]

  i = 0;
  fgets(s, 100, stdin);
  for ( i = 0; i < strlen(s); ++i ) {
    if ( s[i] > 64 && s[i] <= 90 ) // if s[i] is an uppercase letter [A-Z]
      s[i] ^= 0x20u; // convert it to lowercase
  }
  printf(s);
  exit(0);
}
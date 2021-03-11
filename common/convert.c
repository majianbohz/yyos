
void convertInt2Str(int myInt, char* str) 
{
  char c = 0;
  for (int i=7; i>=0; i--) 
  {	
    c = (myInt>>(i*4)) & 0x0000000f;
    if (c > 9)
      *str++ = c + 0x37;
    else
      *str++ = c + 0x30;
  }
  *str = 0;	 
}

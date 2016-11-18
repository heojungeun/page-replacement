#include<stdio.h>

int[20] PAGES = {7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1};

void main()
{
  length = sizeof(PAGES)/sizeof(PAGES[0]);
  int i;
  for (i = 0; i < length; ++i) {
    printf("%i\n", PAGES[i]);
  }
}


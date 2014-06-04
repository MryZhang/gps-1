#include <stdio.h>


static long
rss (long a, long b)
{
  long result, c, d;
  c = abs (a);
  d = abs (b);
  if (c == 0 && d == 0)
    result = 0;
  else {
    if (c > d)
      result = (d >> 1) + c;
    else
      result = (c >> 1) + d;
  }
  return (result);
}

#define LIMIT 100000

int main(void)
{

    long i,j;

    for(i=-LIMIT;i<LIMIT;i+=100)
    {
        for(j=-LIMIT;j<LIMIT;j+=100)
        {
            printf("%ld\n",rss(i,j));
        }
    }

    return 0;
}

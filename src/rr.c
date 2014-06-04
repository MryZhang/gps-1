#include <stdio.h>

float a,b;
unsigned int c;
char d;
int main(void)
{
read(0,&d,1);
read(0,&a,4);
read(0,&b,4);
read(0,&c,2);

printf("%i - %f - %f - %i\n",d,a*(180/3.14159),b*(180/3.14159),c);

}

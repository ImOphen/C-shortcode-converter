#include <stdio.h>

void print_number(int *number)
{
  printf("number is %d\n", *number);
}

int main(int argc, char *argv[])
{
    int x;
    scanf("%d", &x);
    signed int *j = &x;
    while(*j > 0)
    {
        print_number(j);
        *j -= 1;
    }
}
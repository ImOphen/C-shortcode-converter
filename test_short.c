#include <stdio.h>
main(){int x;scanf("%d",&x);signed int *j=&x;while(*j>0){printf("number is %d\n",*j);*j-=1;}}
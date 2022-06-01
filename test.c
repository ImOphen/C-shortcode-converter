#include <stdio.h>

int main()
{
	int x;
	scanf("%d", &x);
	int j = x;
	while(j > 0)
	{
		printf("number is %d\n", j);
		j--;
	}
	while(j <= x)
	{
		printf("number is %d\n", j);
		j++;
	}
}
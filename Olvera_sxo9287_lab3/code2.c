#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

int main()
{
	uint8_t a, b; 
	bool gt, eq, lt; 
	
	a = 0;
	b = 6;
	
	printf("a       a==b       a<b       a>b\n");
	
	for(int i = 0; i < 512; i++)
	{
		gt = (a > b); 
		eq = (a == b); 
		lt = (a < b); 
		
		printf("%-10u%-10i%-10i%-10i\n", a, eq, lt, gt);
		
		a++;
	}


	return 0;
}

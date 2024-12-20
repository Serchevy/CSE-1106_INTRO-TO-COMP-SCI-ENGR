#include <stdio.h>

int main()
{
	int A, B;
	
	printf("Enter Value for A: ");
	scanf("%d", &A);
	
	printf("Enter Value for B: ");
	scanf("%d", &B);
	
	printf("A && B = %d\n", A && B); 
	printf("A || B = %d\n", A || B); 
	printf("!(A || B) = %d\n", !(A || B)); 

	return 0;
}

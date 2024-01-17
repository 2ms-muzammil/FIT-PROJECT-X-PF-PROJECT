#include <stdio.h>

// Function prototype for summing the digits of a number

int sum(int y);

int main()
{
	int x;
	printf("Enter a positive number: ");
	scanf("%i", &x);																	
    // Display the sum of the digits of the entered number
							
	printf("\nThe sum of the digits of %i is: %i\n", x, sum(x));
}

// Recursive function to sum the digits of a number

int sum(int y)
{
	int add = 0;
	
    // Base case: if the number is a single digit, return it

	if (y <= 9)
	{
		add = y;
		return add;
	}
	else
	{
        // Recursive case: sum the last digit with the sum of the remaining digits

		add = y % 10;
		return add + sum(y/10);
	}
}
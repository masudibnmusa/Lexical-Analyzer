#include <stdio.h>

// Simple factorial function
int factorial(int n) {
    if (n <= 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

/* Main function with various
   token types */
int main() {
    int number = 5;
    float result = 3.14;
    char letter = 'A';
    char message[] = "Hello, World!";
    
    // Calculate factorial
    int fact = factorial(number);
    
    printf("Factorial of %d is %d\n", number, fact);
    printf("Pi is approximately %.2f\n", result);
    
    return 0;
}

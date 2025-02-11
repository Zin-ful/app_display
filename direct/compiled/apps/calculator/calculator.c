#include <stdio.h>

int main() {
    int num1, num2;
    printf("pick which two numbers to add together\n");
    printf("\n>>> ");
    scanf("%d", &num1);
    printf("\n>>> ");
    scanf("%d", &num2);
    int total = num1 + num2;
    printf("\nyour total is: %d\n", total);
    return 0;
}

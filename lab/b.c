#include <stdio.h>

int main() {
    int i = 0;

    while (1) {
        i = i+1;
        if (i > 10000000) {
            printf("%d\n", i);
            break;
        }
    }
    return 0;
}

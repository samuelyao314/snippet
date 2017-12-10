
#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <sys/resource.h>
#include <inttypes.h>


int main() {
    printf("%ld", (long) 9223372036854775807LL);
    return 0;
}


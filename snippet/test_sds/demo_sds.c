#include "sds.h"

#include <stdio.h>


int main()
{
    {
        sds mystring = sdsnew("Hello World!");
        printf("output1: %s\n", mystring);
        sdsfree(mystring);
    }

    {
        char buf[3];
        sds mystring;
        buf[0] = 'A';
        buf[1] = 'B';
        buf[2] = 'C';
        mystring = sdsnewlen(buf, 3);
        printf("output2: %s of len %zd\n", mystring, sdslen(mystring));
    }

    {
        sds mystring = sdsempty();
        printf("output3: %zd\n", sdslen(mystring));
    }

    {
        sds s1, s2;
        s1 = sdsnew("Hello");
        s2 = sdsdup(s1);
        printf("output4: %s %s\n", s1, s2);
    }

    {
        sds s = sdsnewlen("A\0\0B", 4);
        printf("output5: %d\n", (int)sdslen(s));
    }

    {
        sds s = sdsempty();
        s = sdscat(s, "Hello ");
        s = sdscat(s, "World!");
        printf("output6: %s\n", s);
    }

    {
        sds s1 = sdsnew("aaa");
        sds s2 = sdsnew("bbb");
        s1 = sdscatsds(s1, s2);
        sdsfree(s2);
        printf("output7: %s\n", s1);
    }

    {
        sds s = sdsnew("Hello");
        s = sdsgrowzero(s, 6);
        s[5] = '!';
        printf("output8: %s\n", s);
    }

    {
        sds s;
        int a = 10, b = 20;
        s = sdsnew("The sum is: ");
        s = sdscatprintf(s, "%d+%d=%d", a, b, a+b);
        printf("output9: %s\n", s);
    }

    {
        char *name = "Anna";
        int loc = 2500;
        sds s;
        s = sdscatprintf(sdsempty(), "%s wrote %d lines of LISP", name, loc);
        printf("output10: %s\n", s);
    }

    {
        sds s = sdsfromlonglong(10000);
        printf("output11: %s\n", s);
    }

    {
        sds s = sdsnew("     my string\n\n   ");
        sdstrim(s, "  \n");
        printf("output12: -%s-\n", s);
    }

    {
        sds s = sdsnew("Hello World!");
        sdsrange(s, 1, 4);
        printf("output13: -%s-\n", s);
    }

    {
        sds s = sdsnew("Hello World!");
        sdsrange(s, 6, -1);
        printf("output14: -%s-\n", s);
        sdsrange(s, 0, -2);
        printf("output15: -%s-\n", s);
    }

    {
        sds s1 = sdsnew("abcd");
        sds s2 = sdsempty();
        s1[1] = 1;
        s1[2] = 2;
        s1[3] = '\n';
        s2 = sdscatrepr(s2, s1, sdslen(s1));
        printf("output16: %s\n", s2);
    }

    {
        sds *tokens;
        int count, j;
        sds line = sdsnew("Hello World!");
        tokens = sdssplitlen(line, sdslen(line), " ", 1, &count);

        for (j = 0; j < count; j++) {
            printf("output17: %s\n", tokens[j]);
        }
        sdsfreesplitres(tokens, count);
    }

    {
        char *tokens[3] = {"foo", "bar", "zap"};
        sds s = sdsjoin(tokens, 3, "|");
        printf("output18: %s\n", s);
    }

    {
        sds s = sdsnew("Ladies and gentlemen");
        s = sdscat(s, "... welcome to the C language.");
        printf("output18: %zd\n", sdsAllocSize(s));
        s = sdsRemoveFreeSpace(s);
        printf("output18: %zd\n", sdsAllocSize(s));
    }

    {
        sds s = sdsnew("foobar");
        s[2] = '\0';
        printf("output19: %zd\n", sdslen(s));
        sdsupdatelen(s);
        printf("output19: %zd\n", sdslen(s));
    }


    return 0;
}

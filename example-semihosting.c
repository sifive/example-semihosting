/* Copyright 2020 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static const char file_name[] = "semihost_test.txt";
static const char test_string[] = "hello, world\n";

int main(int argc, char *argv[]) {
    FILE            *f;
    const char      *t;
    int             c;

    /* test: printf */
    printf("Semihost simple printf\n");

    /* test: argument passed */
    printf("*** test: argument passed ***\n");
    if (argc > 0) {
        printf("argc = %d\n",argc);
        for (int i = 0;(i < argc); i++) {
            printf("\targv[%d]: \"%s\"\n",i,argv[i]);
        }
    }
    printf("*** test finish ***\n");

    /* test: open, write, close */
    printf("*** test: open, fprintf, close\n");
    printf("***       open\n");
    f = fopen(file_name, "w");
    if (!f) {
        printf("failed to open \"%s\" for writing\n", file_name);
        exit(1);
    }

    printf("***       fprintf\n");
    if (fprintf(f, "%s", test_string) != strlen(test_string)) {
        printf("failed to fprintf test string %s\n", test_string);
        exit(1);
    }
    printf("***       close\n");
    if (fclose(f) != 0) {
        printf("fclose failed\n");
        exit(1);
    }
    printf("*** test finish ***\n");

    /* test: read previous file */
    printf("*** test: read previous file\n");
    printf("***       open\n");
    f = fopen(file_name, "r");
    if (!f) {
        printf("failed to open \"%s\" for reading\n", file_name);
        exit(1);
    }

    t = test_string;

    printf("***       getc\n");
    while ((c = getc(f)) != EOF) {
        if ((char) c != *t) {
            printf("read incorrect byte %c != %c\n", c, *t);
            exit(1);
        }
        t++;
    }

    printf("***       close\n");
    if (fclose(f) != 0) {
        printf("fclose failed\n");
        exit(1);
    }
    printf("*** test finish ***\n");

    /* test: stdin - for semihost name = ":tt", mode = "r" for stdin */
    printf("*** test: stdin ***\n");
    f = fopen(":tt", "r");
    if (!f) {
        printf("failed to open stdin on host\n");
        exit(1);
    }

    printf("*** test: getc until 'x' ***\n");
    while ((c = getc(f)) != 'x') {
        printf("read: %c\n",c);
    }

    printf("***       close\n");
    if (fclose(f) != 0) {
        printf("fclose failed\n");
        exit(1);
    }
    printf("*** test finish ***\n");

    /* erase remote generated file */
    unlink(file_name);
}

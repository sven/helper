/*
 * Test the UTF-8 resize function.
 */

#include <assert.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include "../utf8_helper.h"


/* Local prototypes */
int check_len(char *src, char *dst, int len);


int main(void)
{
    int res = 0;
    char *dst = NULL;

    // enable locale setting
    assert(NULL != setlocale(LC_ALL, ""));

    printf("Test: 4 byte normal string to 10 byte length");
    fflush(stdout);
    {
	char src[] = "Test";
	unsigned int len = 10;

	dst = utf8_resize(src, len, false, false);
	res = check_len(src, dst, len);
    }

    printf("Test: 10 byte normal string to 10 byte length");
    fflush(stdout);
    {
	char src[] = "TestTest12";
	unsigned int len = 10;

	dst = utf8_resize(src, len, false, false);
	res = check_len(src, dst, len);
    }

    printf("Test: 4 byte UTF-8 (2 chars UTF-8) string to 10 byte length");
    fflush(stdout);
    {
	char src[] = "TeÄÖ";
	unsigned int len = 10;

	dst = utf8_resize(src, len, false, false);
	res = check_len(src, dst, len);
    }

    printf("Test: 4 byte UTF-8 (4 chars UTF-8) string to 10 byte length");
    fflush(stdout);
    {
	char src[] = "ÄÖÜä";
	unsigned int len = 10;

	dst = utf8_resize(src, len, false, false);
	res = check_len(src, dst, len);
    }

    printf("Test: 8 byte UTF-8 (6 chars UTF-8) string to 4 byte length");
    fflush(stdout);
    {
	char src[] = "TäÖÜpäöü";
	unsigned int len = 4;

	dst = utf8_resize(src, len, false, false);
	res = check_len(src, dst, len);
    }

    printf("Test: \"Teststring w/o UTF8\" to 8 byte length");
    fflush(stdout);
    {
	char src[] = "Teststring w/o UTF8";
	unsigned int len = 8;

	dst = utf8_resize(src, len, false, false);
	res = check_len(src, dst, len);
    }

    printf("\nTest result: %s\n\n", (res) ? "error" : "ok");
    return (res);
}


int check_len(char *src, char *dst, int len)
{
    int dst_len = 0;

    dst_len = mbstowcs(NULL, dst, 0);
    if (dst != src) {
	free(dst);
    }

    if (len == dst_len) {
	printf(": ok (%u)\n", dst_len);
	return (0);
    } else {
	printf(": error (%u)\n", dst_len);
	return (-1);
    }
}


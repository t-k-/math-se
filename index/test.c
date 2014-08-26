#include <stdio.h>
#include <stdlib.h>
#include "bdb_wraper.h"

int main() 
{
	int *v;
	int i;
	char *str;
	bdb_init("./test.bdb");	

	/* the same key */
	bdb_put2("abcdef", "haha");
	bdb_put_int("abcdef", 6, 123);

	v = (int*)bdb_get_int("abcdef", 6, &i);
	if (v) {
		printf("v=%d, i=%d\n", *v, i);
		free(v);
	}

	str = bdb_get2("abcdef");
	if (str) {
		printf("str=%s\n", str);
		free(str);
	}

	v = (int*)bdb_get_int("abcdef", 6, &i);
	if (v) {
		printf("v=%d, i=%d\n", *v, i);
		free(v);
	}

	str = bdb_get2("abcdef");
	if (str) {
		printf("str=%s\n", str);
		free(str);
	}

	bdb_release();	
}

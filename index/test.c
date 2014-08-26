#include <stdio.h>
#include <stdlib.h>
#include "bdb_wraper.h"

int main() 
{
	void *bdb = bdb_init("./test.bdb");	
	uint64_t n = bdb_records(bdb);
	printf("%ld\n", n);
	bdb_release(bdb);	
}

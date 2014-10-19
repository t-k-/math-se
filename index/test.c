#include <stdio.h>
#include <stdlib.h>
#include "bdb_wraper.h"

#include <tcutil.h>
#include <tcbdb.h>

int main() 
{
	void *bdb = bdb_init("./test.bdb");	
	uint64_t records; 
	char *key, *value;
	BDBCUR *cur;
	if (bdb == NULL)
		return;

	bdb_put2(bdb, "gun", "shota");
	bdb_put2(bdb, "li", "si");
	bdb_put2(bdb, "dad", "mom");
	bdb_put2(bdb, "boy", "girl");
	bdb_put2(bdb, "zhang", "san");
	bdb_put2(bdb, "holly", "shit");
	bdb_put2(bdb, "eli", "zer");
	bdb_put2(bdb, "apple", "steve");
	bdb_put2(bdb, "ivy", "hall");
	bdb_put2(bdb, "jerk", "fool");
	bdb_put2(bdb, "kick", "out");
	bdb_put2(bdb, "foo", "bar");
	bdb_put2(bdb, "math", "lab");
	bdb_put2(bdb, "cat", "dog");

	records = bdb_records(bdb);
	printf("%ld records.\n", records);

	cur = tcbdbcurnew(bdb);
	tcbdbcurjump2(cur, "zz");
	while ((key = tcbdbcurkey2(cur)) != NULL) {
		value = tcbdbcurval2(cur);
		if (value) {
			printf("%s:%s\n", key, value);
			free(value);
		}
		free(key);
		tcbdbcurnext(cur);
	}

	tcbdbcurdel(cur);

	bdb_release(bdb);	
}

#include <tcutil.h>
#include <tcbdb.h>

TCBDB *bdb = NULL;
char  *last = NULL;

int bdb_init(const char *name) 
{
	bdb = tcbdbnew();
	
	if (!tcbdbopen(bdb, name, BDBOCREAT | BDBOWRITER))
		return 1;

	return 0;
}

uint64_t bdb_records()
{
	return tcbdbrnum(bdb); 
}

int bdb_release() 
{
	int res = tcbdbclose(bdb);
	tcbdbdel(bdb);

	if (res)
		return 0; 
	else
		return 1;
}

int bdb_put2(const char *key, const char *value)
{
	int res = tcbdbput2(bdb, key, value);

	if (res)
		return 0; 
	else
		return 1;
}

char *bdb_get2(const char *key)
{
	last = tcbdbget2(bdb, key);
	return last;
}

void bdb_free_last()
{
	if (last)
		free(last);
}

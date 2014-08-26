#include <tcutil.h>
#include <tcbdb.h>

void *bdb_init(const char *name) 
{
	TCBDB *bdb = tcbdbnew();
	
	if (!tcbdbopen(bdb, name, BDBOCREAT | BDBOWRITER))
		return NULL;

	return bdb;
}

uint64_t bdb_records(void *bdb)
{
	return tcbdbrnum((TCBDB*)bdb); 
}

int bdb_release(void *bdb) 
{
	int res = tcbdbclose((TCBDB*)bdb);
	tcbdbdel((TCBDB*)bdb);

	if (res)
		return 0; 
	else
		return 1;
}

void c_free(void *p) 
{
	free(p);
}

int bdb_put2(void *bdb, const char *key, const char *value)
{
	int res = tcbdbput2((TCBDB*)bdb, key, value);

	if (res)
		return 0; 
	else
		return 1;
}

char *bdb_get2(void *bdb, const char *key)
{
	return tcbdbget2((TCBDB*)bdb, key);
}

int bdb_put_int(void *bdb, const char *key, int ksiz, int value)
{
	int res = tcbdbput((TCBDB*)bdb, key, ksiz, &value, sizeof(int));

	if (res)
		return 0; 
	else
		return 1;
}

int *bdb_get_int(void *bdb, const char *key, int ksiz)
{
	int ret_sz;
	return tcbdbget((TCBDB*)bdb, key, ksiz, &ret_sz);
}

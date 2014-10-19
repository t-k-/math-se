#include <stdio.h>
#include <stdlib.h>
#include "bdb_wraper.h"

#include <tcutil.h>
#include <tcbdb.h>

#define FRML_ID_CMP_LEN 7 
int frml_id_cmp(const char *a, int asiz, 
                const char *b, int bsiz, 
                void *op) 
{
	unsigned int i = 0;
	for (i = 0; i < FRML_ID_CMP_LEN; i++) {
		if (a[i] != b[i])
			return a[i] - b[i];
	}
	
	return 0;
}

int frml_id_cmp_(const char *a, const char *b)
{
	return frml_id_cmp(a, FRML_ID_CMP_LEN, 
	                   b, FRML_ID_CMP_LEN, NULL);
}

int main() 
{
	void *bdb1, *bdb2;
	char *key1, *key2, *val1, *val2;
	char *key, *value;
	BDBCUR *cur1, *cur2;
	char str_key[FRML_ID_CMP_LEN + 1];
	char str_val[FRML_ID_CMP_LEN + 2];
	unsigned int cnt, dup, steps;
	int cmp_res;
	FILE *f1, *f2;
		
	bdb1 = bdb_init2("./test1.bdb", &frml_id_cmp);	
	bdb2 = bdb_init2("./test2.bdb", &frml_id_cmp);	
	if (bdb1 == NULL || bdb2 == NULL)
		return;

#if 0
	srand(92019);

	printf("put db1...\n");
	cnt = 0;
	while (cnt < 10000) {
		snprintf(str_key, FRML_ID_CMP_LEN + 1, 
		         "%0*d\n", FRML_ID_CMP_LEN, rand());
		sprintf(str_val, "v%s", str_key);
		bdb_put2(bdb1, str_key, str_val);
		cnt ++;
	}
	
	printf("put db2...\n");
	cnt = 0;
	while (cnt < 1000090) {
		snprintf(str_key, FRML_ID_CMP_LEN + 1, 
		         "%0*d\n", FRML_ID_CMP_LEN, rand());
		sprintf(str_val, "v%s", str_key);
		bdb_put2(bdb2, str_key, str_val);
		cnt ++;
	}
	
	/*
	f1 = fopen("log1.txt", "w");
	f2 = fopen("log2.txt", "w");

	cur1 = tcbdbcurnew(bdb1);
	tcbdbcurfirst(cur1);
	while ((key = tcbdbcurkey2(cur1)) != NULL) {
		value = tcbdbcurval2(cur1);
		if (value) {
			fprintf(f1, "%s\n", key);
			free(value);
		}
		free(key);
		tcbdbcurnext(cur1);
	}
	tcbdbcurdel(cur1);

	cur2 = tcbdbcurnew(bdb2);
	tcbdbcurfirst(cur2);
	while ((key = tcbdbcurkey2(cur2)) != NULL) {
		value = tcbdbcurval2(cur2);
		if (value) {
			fprintf(f2, "%s\n", key);
			free(value);
		}
		free(key);
		tcbdbcurnext(cur2);
	}
	tcbdbcurdel(cur2);

	fclose(f1);
	fclose(f2);
	*/

#else
#define NUM 10

	printf("go one-by-one 100 times...\n");
	for (cnt = 0, dup = 0, steps = 0; cnt < NUM; cnt++) {
		cur1 = tcbdbcurnew(bdb1);
		tcbdbcurfirst(cur1);
		while ((key = tcbdbcurkey2(cur1)) != NULL) {
			value = tcbdbcurval2(cur1);
			if (value) {
				//printf("one-by-one: %s\n", key);
				if (bdb_get2(bdb2, key)) {
					//printf("%s duplicated.\n", key);
					dup ++;
				}
				free(value);
			}
			free(key);
			tcbdbcurnext(cur1);
			steps ++;
		}
		tcbdbcurdel(cur1);
	}
	printf("%d duplicates in %d steps.\n", dup, steps);
	
	printf("go jumping 100 times...\n");
	for (dup = 0, cnt = 0, steps = 0; cnt < NUM; cnt++) {
		cur1 = tcbdbcurnew(bdb1);
		cur2 = tcbdbcurnew(bdb2);
		tcbdbcurfirst(cur1);
		tcbdbcurfirst(cur2);
		do {
			key1 = tcbdbcurkey2(cur1);
			key2 = tcbdbcurkey2(cur2);
			if (key1 == NULL || key2 == NULL) {
				if (key1)
					free(key1);
				else 
					free(key2);
				break;
			}

			cmp_res = frml_id_cmp_(key1, key2);
			//printf("jumping %s and %s\n", key1, key2);
			steps ++;
			if (cmp_res == 0) {
				val1 = tcbdbcurval2(cur1);
				val2 = tcbdbcurval2(cur2);
				dup ++;

				tcbdbcurnext(cur1);
				free(val1);
				free(val2);
			} else if (cmp_res > 0) /* key1 is larger */ {
				tcbdbcurjump2(cur2, key1);
			} else /* key2 is larger */ {
				tcbdbcurjump2(cur1, key2);
			}

			free(key1);
			free(key2);
		} while (1);
	}
	printf("%d duplicates in %d steps.\n", dup, steps);

#endif

	bdb_release(bdb1);	
	bdb_release(bdb2);	
}

#include <stdio.h>
#include <stdlib.h>
#include "bdb_wraper.h"
#include "rlv_tr.h"

int main(int argc, char **argv) 
{
	void *num_bdb = bdb_init("./col/brw-number.bdb");	
	void *doc_bdb = bdb_init("./col/documents.bdb");	
	uint64_t nnum_bdb, ndoc_bdb;
	char *doc, *key = argv[1];
	int *nbrw;

	if (num_bdb == NULL || doc_bdb == NULL) {
		printf("open db error.\n");
		goto exit;
	} else {
		nnum_bdb = bdb_records(num_bdb);
		ndoc_bdb = bdb_records(doc_bdb);
	}

	if (argc != 2) {
		printf("bad arg. (cmd <doc-id>)\n");
		goto exit;
	}

	nbrw = bdb_get_int(num_bdb, key, DOC_HASH_LEN);

	if (nbrw == NULL) {
		printf("bdb_get_int() returns NULL.\n");
		goto exit;
	}

	printf("#brw: %d\n", *nbrw);
	free(nbrw);

	doc = bdb_get2(doc_bdb, key);

	if (doc == NULL) {
		printf("bdb_get2() returns NULL.\n");
		goto exit;
	}

	printf("document:\n%s", doc);
	free(doc);

exit:
	printf("#records in num bdb: %ld\n", nnum_bdb);
	printf("#records in doc bdb: %ld\n", ndoc_bdb);

	if (num_bdb)
		bdb_release(num_bdb);
	if (doc_bdb)
		bdb_release(doc_bdb);
}

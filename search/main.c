#include <stdio.h>
#include "rlv_tr.h"
#include "bdb_wraper.h"
#include "rank.h"

int main(int argc, char *argv[])
{
	char *query = argv[1];
	void *bdb_doc, *bdb_num;
	struct list_it li_query_brw;

	if (argc != 2) {
		printf("invalid argument format.\n");
		return __LINE__;
	}

	if (redis_cli_init("127.0.0.1", DEFAULT_REDIS_PORT)) {
		printf("redis server is down.\n");
		return __LINE__;
	}

	if (0 == strcmp(query, "#flush")) {
		printf("flush redis DB...\n");
		redis_cli_free();
		return 0;

	} else if (0 == strcmp(query, "#shutdown")) {
		printf("shutdown redis DB...\n");
		redis_shutdown();
		return 0;
	}

	bdb_doc = bdb_init("./collection/documents.bdb");
	if (NULL == bdb_doc) {
		printf("tcbdb doc open error.\n");
		redis_cli_free();
		return __LINE__;
	}
	
	bdb_num = bdb_init("./collection/brw-number.bdb");
	if (NULL == bdb_num) {
		printf("tcbdb num open error.\n");
		redis_cli_free();
		bdb_release(bdb_doc);
		return __LINE__;
	}

	printf("query: %s\n", query);

	mak_rank("ranking set", query, bdb_num);
	pri_rank("ranking set", 0, -1, bdb_doc, bdb_num);
	del_rank("ranking set");
	
	bdb_release(bdb_doc);
	bdb_release(bdb_num);
	redis_cli_free();
	
	printf(COLOR_BLUE "(query: `%s')\n" COLOR_RST, query);
	return 0;
}

#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include "rlv_tr.h"
#include "bdb_wraper.h"
#include "rank.h"

int main(int argc, char *argv[])
{
	int opt; 
	uint64_t n_relevant;
	void *bdb_doc, *bdb_num;
	int64_t start = 0, end = 1;
	char options[] = "s:e:q:h", *query = NULL;

	if (argc == 1) {
usage:
		fprintf(stderr, "Usage: %s [%s]\n", argv[0], options);
		return 1;
	}
	
	while ((opt = getopt(argc, argv, options)) != -1) {
		switch (opt) {
		case 's': sscanf(optarg, "%" PRId64, &start); break;
		case 'e': sscanf(optarg, "%" PRId64, &end); break;
		case 'q': query = strdup(optarg); break;
		case 'h':
		default: goto usage;
		}
	}

	if (end != -1 && end < start) {
		printf("bad start end position.\n");
		return 1;
	} else if (query == NULL) {
		printf("no query given.\n");
		return 1;
	} else {
		printf("search `%s' ranking from %" PRId64 " to %" 
		       PRId64 "...\n", query, start, end);
	}

	if (redis_cli_init("127.0.0.1", DEFAULT_REDIS_PORT)) {
		printf("redis server is down.\n");
		free(query);
		return __LINE__;
	}

	if (0 == strcmp(query, "#flush")) {
		printf("flush redis DB...\n");
		redis_cli_free();
		free(query);
		return 0;

	} else if (0 == strcmp(query, "#shutdown")) {
		printf("shutdown redis DB...\n");
		redis_shutdown();
		free(query);
		return 0;
	}

	bdb_doc = bdb_init("./col/documents.bdb");
	if (NULL == bdb_doc) {
		printf("tcbdb doc open error.\n");
		redis_cli_free();
		free(query);
		return __LINE__;
	}
	
	bdb_num = bdb_init("./col/brw-number.bdb");
	if (NULL == bdb_num) {
		printf("tcbdb num open error.\n");
		redis_cli_free();
		bdb_release(bdb_doc);
		free(query);
		return __LINE__;
	}

	printf("query: %s\n", query);

	mak_rank("ranking set", query, bdb_num, &n_relevant);
	pri_rank("ranking set", start, end, bdb_doc, bdb_num);
	del_rank("ranking set");
	
	bdb_release(bdb_doc);
	bdb_release(bdb_num);
	redis_cli_free();
	
	printf(COLOR_BLUE "(query: `%s', #relevant: %" PRIu64 
	       ")\n" COLOR_RST, query, n_relevant);

	free(query);
	return 0;
}

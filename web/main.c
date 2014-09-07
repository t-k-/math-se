#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "rlv_tr.h"
#include "bdb_wraper.h"
#include "rank.h"

#define RES_PER_PAGE 10

static void cat(char *file)
{
	size_t len = 0;
	char *line = NULL;
	FILE *f = fopen(file, "r");
	if (f == NULL)
		return;

	while (getline(&line, &len, f) != -1)
		printf("%s", line);

	free(line);
	fclose(f);
}

static void replace_plus(char *str)
{
	while (str[0] != '\0') {
		if (str[0] == '+') 
			str[0] = ' ';
		str ++;
	}
}

static char *q_unescape(char *str)
{
	CURL *curl;
	char *ret;
	curl = curl_easy_init();
	ret = curl_easy_unescape(curl, str, 0, NULL);
	curl_easy_cleanup(curl);
	return ret;
}

static void _print_rank(const char* frml_hash, void *arg)
{
#if 0
	struct doc_frml *df = redis_frml_map_get(frml_hash);
	P_CAST(fsa, struct _final_score_arg, arg);
	char *doc = bdb_get2(fsa->bdb_doc, frml_hash);
	int *num = bdb_get_int(fsa->bdb_num, frml_hash, 
	                       DOC_HASH_LEN);

	printf(COLOR_BLUE "< #%ld >: \n" COLOR_RST, (*fsa->rank) ++);
	printf("doc #%s (brws=%d):\n%s\n", 
	       short_hash(frml_hash), *num, doc);
	free(doc);
	free(num);
#endif
	rlv_tr_print(df);
}

int main()
{
	void *bdb_doc, *bdb_num;
	int64_t start;
	char *query, *env_str, stdin_str[64];

	printf("Content-type: text/html\n\n");

	/* get POST argument */
	scanf("%s", stdin_str);
	replace_plus(stdin_str);

	/* get GET argument, use `export QUERY_STRING=q=a+b' to
	 * test cgi program. */
	env_str = getenv("QUERY_STRING");

	if (env_str == NULL) {
		printf("cgi: getenv() returns null.");
		return __LINE__;
	}
	sscanf(env_str, "s=%" PRId64, &start);

	if (redis_cli_init("127.0.0.1", DEFAULT_REDIS_PORT)) {
		printf("cgi: redis server is down.");
		return __LINE__;
	}

	bdb_doc = bdb_init("./collection/documents.bdb");
	if (NULL == bdb_doc) {
		printf("cgi: tcbdb doc open error.");
		redis_cli_free();
		return __LINE__;
	}
	
	bdb_num = bdb_init("./collection/brw-number.bdb");
	if (NULL == bdb_num) {
		printf("cgi: tcbdb num open error.");
		redis_cli_free();
		bdb_release(bdb_doc);
		return __LINE__;
	}

	query = q_unescape(stdin_str);
	query[0] = ' ';
	query[1] = ' ';
	/* printf("query=%s, start=%" PRId64 "\n", query, start); */

	cat("head.cat");
	printf("%s", query); curl_free(query);
	printf("\\Big\\}[/imath]</h2>");
	printf("<p>%d result(s)</p>", 123);
	cat("neck.cat");

//	if (fail) {
//		printf("<li><h1>query syntax error. X_X </h1></li>\n");
//		exit(1);
//	} else if (results == 0) {
//		printf("<li><h1>we have tried our best... </h1></li>\n");
//		exit(1);
//	}
//
//	end_flag = 0;
//	while (getline(&line, &len, f_rank) != -1) {
//		if (cnt / 2 >= start + RES_PER_PAGE - 1)
//			goto quit;
//
//		if (cnt >= 2 * (start - 1)) {
//			if (cnt % 2 == 0) {
//				strcpy(last_line, line);
//			} else {
//				printf("<li><table border=\"0\"><tr><td><a href=%s"
//						"><h3>%s</h3></a><span style=\"color:green;\">"
//						"%s</span></td><td>[dmath]%s"
//						"[/dmath]</td></tr></table></li>",
//						line, last_line, line, last_line);
//			}
//		}
//
//		cnt ++;
//	}
//	end_flag = 1;
//
	cat("ass.cat");
//
//	//printf("<span class=\"cnt_%d_endflag_%d\"></span>", cnt, end_flag);
//
//	printf(
//  "<form action=\"/cgi/result.bin?s=%d\" method=\"post\" "
//  "name=\"my_pr\"><input type=\"hidden\" name=\"q\" value=\"%s\"/>"
//   "</form>", max(1, start - RES_PER_PAGE), arg);
//
//	printf(
//  "<form action=\"/cgi/result.bin?s=%d\" method=\"post\" "
//  "name=\"my_ne\"><input type=\"hidden\" name=\"q\" value=\"%s\"/>"
//  "</form>", cnt/2 + 1, arg);
//
//	if (start > 1)
//		cat("previous.cat");
//	printf("</td><td>");
//	if (!end_flag)
//		cat("next.cat");
	cat("tail.cat");

/*
	mak_rank("ranking set", query, bdb_num);
	pri_rank("ranking set", 0, -1, bdb_doc, bdb_num);
	redis_z_rrange(rank_set, &_print_rank, start, end, &fsa);
	del_rank("ranking set");
	
	bdb_release(bdb_doc);
	bdb_release(bdb_num);
	redis_cli_free();
*/
	return 0;
}

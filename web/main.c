#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "rlv_tr.h"
#include "bdb_wraper.h"
#include "rank.h"

#define RES_PER_PAGE 7

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

static char *query_subst(char *query)
{
	uint i;
	query[0] = ' ';
	query[1] = ' ';

	for (i = 0; i < strlen(query); i++) {
		if (query[i] == '\n')
			query[i] = ' ';
	}
}

struct _html_li_arg {
	void *bdb_doc;	
	void *bdb_num;	
	uint  nitems;
};

static void _html_li(const char* frml_hash, void *arg)
{
	struct doc_frml *df = redis_frml_map_get(frml_hash);

	P_CAST(hla, struct _html_li_arg, arg);
	char *doc = bdb_get2(hla->bdb_doc, frml_hash);
	int *num = bdb_get_int(hla->bdb_num, frml_hash, DOC_HASH_LEN);
	char url[4096];
	char tex[4096];
	int i = 0;
	char *field;

	if (doc == NULL || num == NULL)
		return;
	
	field = strtok(doc, "\n\n");
	if (field) 
		strcpy(url, field);
	else 
		return;

	field = strtok(NULL, "\n\n");
	if (field) 
		strcpy(tex, field);
	else 
		return;

	/* printf("url=%s tex=%s nbrw=%d ID=%s<br/>", 
	          url, tex, *num, frml_hash); */
	printf("<li><table border=\"0\"><tr><td>"
	       "<a target=\"_blank\" href=\"%s\"><h3>%s</h3></a>"
	       "<span style=\"color:green;\">%s</span>"
	       "</td><td>[dmath]%s[/dmath]"
	       "<span style=\"color:white;\">"
	       "ID=%s, score=%f, nbrw=%d."
	       "</span></td></tr></table></li>",
	       url, tex, url, tex, frml_hash, df->score, *num);
	hla->nitems ++;
	free(doc);
	free(num);
}

int main()
{
	struct _html_li_arg hla;
	int64_t start, end;
	uint64_t n_relevant;
	char *query, *env_str, stdin_str[64];

	printf("Content-type: text/html\n\n");

	/* get POST argument */
	scanf("%s", stdin_str);
	replace_plus(stdin_str);

	/* get GET argument, use `export QUERY_STRING=s=123' to
	 * test cgi program. */
	env_str = getenv("QUERY_STRING");

	if (env_str == NULL) {
		printf("cgi: getenv() returns null.");
		return __LINE__;
	}

	sscanf(env_str, "s=%" PRId64, &start);
	
	if (start < 0) {
		printf("cgi: start < 0.");
		return __LINE__;
	}

	if (redis_cli_init("127.0.0.1", DEFAULT_REDIS_PORT)) {
		printf("cgi: redis server is down.");
		return __LINE__;
	}

	hla.bdb_doc = bdb_init("./collection/documents.bdb");
	if (NULL == hla.bdb_doc) {
		printf("cgi: tcbdb doc open error.");
		redis_cli_free();
		return __LINE__;
	}
	
	hla.bdb_num = bdb_init("./collection/brw-number.bdb");
	if (NULL == hla.bdb_num) {
		printf("cgi: tcbdb num open error.");
		redis_cli_free();
		bdb_release(hla.bdb_doc);
		return __LINE__;
	}

	query = q_unescape(stdin_str);
	query_subst(query);

	printf("<!-- stdin=%s query=%s, start=%" PRId64 " -->", 
	       stdin_str, query, start);

	cat("head.cat");
	printf("[imath]q=\\Big\\{%s\\Big\\}[/imath]", query);
	cat("neck.cat");
	
	hla.nitems = 0;
	if (mak_rank("ranking set", query, hla.bdb_num, &n_relevant)) {
		printf("<li><h1>Query syntax error: %s </h1></li>\n",
		       parser_error_dscr);
		end = -1;
	} else {
		end = (start + 1) * RES_PER_PAGE - 1;
		redis_z_rrange("ranking set", &_html_li, 
				start * RES_PER_PAGE, 
				end, &hla);
		del_rank("ranking set");

		if (hla.nitems == 0)
			printf("<li><h1>No matching document.</h1></li>\n");
	}
	
	cat("ass.cat");

	printf("<form action=\"/math-se-bin/rank.cgi?s=%" PRId64 "\" "
	       "method=\"post\" name=\"prev\">"
	       "<input type=\"hidden\" name=\"q\" value=\"%s\">"
	       "</form>", start - 1, query);
	printf("<form action=\"/math-se-bin/rank.cgi?s=%" PRId64 "\" "
	       "method=\"post\" name=\"next\">"
	       "<input type=\"hidden\" name=\"q\" value=\"%s\">"
	       "</form>", start + 1, query);

	if (start > 0) {
		cat("prev.cat");
		printf("</td><td>");
		if (end < n_relevant - 1)
			cat("next.cat");
	} else {
		if (end < n_relevant - 1) {
			cat("next.cat");
			printf("</td><td>");
		}
	}

	cat("tail.cat");

	curl_free(query);
	bdb_release(hla.bdb_doc);
	bdb_release(hla.bdb_num);
	redis_cli_free();
	return 0;
}

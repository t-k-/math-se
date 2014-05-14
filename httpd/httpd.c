#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include "../inter-def.h"

#define RES_PER_PAGE 10

void cat(char *file)
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

int wc_l(char *file)
{
	size_t len = 0;
	int cnt = 0;
	char *line = NULL;
	FILE *f = fopen(file, "r");
	if (f == NULL)
		return 0;

	while (getline(&line, &len, f) != -1) 
		cnt ++;
	free(line);

	fclose(f);
	return cnt;
}

void replace_plus(char *str)
{
	while (str[0] != '\0') {
		if (str[0] == '+') 
			str[0] = ' ';
		str ++;
	}
}

int main()
{
	CURL *curl;
	char *unescape;
	char *get_str;
	char arg[2048];
	char cmd[2048];
	char *line;
	char last_line[4096];
	size_t len = 0;
	int cnt = 0;
	int start;
	FILE *f_rank = NULL;
	int end_flag;
	printf("Content-type: text/html\n\n");

	scanf("%s", arg);
	get_str = getenv("QUERY_STRING");

	if (get_str == NULL) {
		printf("no GET arg.\n");
		return 0;
	}

	sscanf(get_str, "s=%d", &start);

	replace_plus(arg);
	curl = curl_easy_init();
	unescape = curl_easy_unescape(curl, arg, 0, NULL);

	sprintf(arg, "%s", unescape);
	unescape[0] = ' ';
	unescape[1] = ' ';
	strcpy(arg, unescape);
	free(unescape);

	if (start == 0) {
		sprintf(cmd, "./ma-se '%s'", arg);
		system(cmd);
		start = 1;
	} 

	cat("head.cat");
	printf("[imath] \\quad %s [/imath] : </p><hr class=\"rgsep\">"
			"<ul style=\"list-style-type: none;\">", arg);

	f_rank = fopen("rank", "r");
	if (f_rank == NULL) {
		printf("no thing we can find :( <br/>");
		return 0;
	}

	end_flag = 0;
	while (getline(&line, &len, f_rank) != -1) {
		if (cnt / 2 >= start + RES_PER_PAGE - 1)
			goto quit;

		if (cnt >= 2 * (start - 1)) {
			if (cnt % 2 == 0) {
				strcpy(last_line, line);
			} else {
				printf("<li><table border=\"0\"><tr><td><a href=%s"
						"><h3>%s</h3></a><span style=\"color:green;\">"
						"%s</span></td><td>[dmath]%s"
						"[/dmath]</td></tr></table></li>",
						line, last_line, line, last_line);
			}
		}

		cnt ++;
	}
	end_flag = 1;

quit:
	free(line);
	
	fclose(f_rank);
	cat("ass.cat");

	printf("<span class=\"cnt_%d_endflag_%d\"></span>", cnt, end_flag);

	printf(
  "<form action=\"/cgi/result.bin?s=%d\" method=\"post\" "
  "name=\"my_pr\"><input type=\"hidden\" name=\"q\" value=\"%s\"/>"
   "</form>", max(1, start - 10), arg);

	printf(
  "<form action=\"/cgi/result.bin?s=%d\" method=\"post\" "
  "name=\"my_ne\"><input type=\"hidden\" name=\"q\" value=\"%s\"/>"
  "</form>", cnt/2 + 1, arg);

	if (start > 1)
		cat("previous.cat");
	printf("</td><td>");
	if (!end_flag)
		cat("next.cat");
	cat("tail.cat");

	return 0;
}

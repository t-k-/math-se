#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inter-def.h"

typedef struct {
	unsigned int len, num;
	unsigned int w[512];
}br_wd_t;

void print_bwd(br_wd_t *bwd)
{
	unsigned int i;
	printf("num=%d, length=%d, weight=[", bwd->num, bwd->len);
	for (i = 0; bwd->w[i] != 0; i++) {
		printf("%d ", bwd->w[i]);
	}
	printf("]\n");
}

void str2bwd(char *str, br_wd_t *out)
{
	char *p, sub_str[STR_BUFLEN];
	int d, cnt = 0;
	out->len = 0;
	out->num = 0;
	out->w[0] = 0;
	sscanf(str, "%dX%s", &out->num, sub_str);

	p = sub_str;
	while (*p != '\0') {
		sscanf(sub_str, "%d-", &d);
		out->len ++;
		out->w[cnt++] = d;
		while (*p != '-') {
			*p = ' ';
			if (*(p + 1) == '\0')
				break;
			p ++;
		}
		*p = ' ';
		p ++;
	}
	out->w[cnt] = 0;
}

void candy2rank(char *bwd_str)
{
	FILE *fcandy = fopen("candy", "r");
	char tmp_str[STR_BUFLEN];
	char dir_str[STR_BUFLEN];
	char doc_str[STR_BUFLEN];
	br_wd_t query, doc;
	str2bwd(bwd_str, &doc);
	printf("query branch word: ");
	print_bwd(&doc);

	if (fcandy == NULL)
		return;
	
	while (fgets(tmp_str, sizeof(tmp_str), fcandy)) {
		if (tmp_str[0] == 'r') {
			sscanf(tmp_str, "referred_by %s\n", dir_str);
			dir_str[strlen(dir_str) - strlen("posting") - 1] = '\0';
		} else {
			sscanf(tmp_str, "%s %s\n", bwd_str, doc_str);
			str2bwd(bwd_str, &query);
			printf("doc branch word: ");
			print_bwd(&query);

			printf("SCORE %s/%s\n", dir_str, doc_str);
		}
	}

	fclose(fcandy);
}

int main(int argc, const char *argv[])
{
	FILE *fquery = fopen("query", "w");
	FILE *fbody;
	char tmp_str[STR_BUFLEN];
	char dir_str[STR_BUFLEN];
	char num_str[32];
	char bwd_str[STR_BUFLEN];

	if (fquery == NULL) {
		printf("cannot open file for writing.\n");
		return 0;
	}

	if (argc != 2) {
		printf("no query argument.\n");
		return 0;
	}

	fprintf(fquery, "%s\n", argv[1]);
	fclose(fquery);

	system("rm " OUT_FBODY);
	system("./ma-pa null < query");

	fbody = fopen(OUT_FBODY, "r");

	if (fbody == NULL) {
		printf("bad query.\n");
		return 0;
	}

	printf("query: \n");
	system("cat " OUT_FBODY);

	system("> candy");
	while (fgets(tmp_str, sizeof(tmp_str), fbody)) {
		sscanf(tmp_str, "%s %s %s\n", dir_str, num_str, bwd_str);
		printf("under %s:\n", dir_str);

		sprintf(tmp_str, "test -d %s", dir_str);
		if (system(tmp_str) != 0 /* test return 0 */) {
			printf("%s\n", "branch word not found.");
			continue;
		}

		sprintf(tmp_str, "find %s -name 'posting' "
		                 "-exec echo referred_by {} \\; "
		                 "-exec cat {} \\; "
		                 "| tee candy", dir_str, dir_str);
		system(tmp_str);

		sprintf(tmp_str, "%s%s", num_str, bwd_str);
		candy2rank(tmp_str);
	}

	fclose(fbody);
	return 0;
}

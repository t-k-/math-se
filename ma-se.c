#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inter-def.h"

void candy2rank(char *num_str, char *bwd_str)
{
	FILE *fcandy = fopen("candy", "r");
	char tmp_str[STR_BUFLEN];
	char dir_str[STR_BUFLEN];
	unsigned int num;
	char bwd_str2[STR_BUFLEN];
	char *p, *p2;

	if (fcandy == NULL)
		return;
	
	while (fgets(tmp_str, sizeof(tmp_str), fcandy)) {
		if (tmp_str[0] == 'r') {
			sscanf(tmp_str, "referred_by %s\n", dir_str);
			dir_str[strlen(dir_str) - strlen("posting") - 1] = '\0';
		} else {
			sscanf(tmp_str, "%dX%s", &num, bwd_str2);
		}
	}
	printf("[%s]\n", bwd_str2);
	printf("[%s]\n", bwd_str);
	for (p = bwd_str, p2 = bwd_str2;;) {
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
		sprintf(tmp_str, "find %s -name 'posting' "
		                 "-exec echo referred_by {} \\; "
		                 "-exec cat {} \\; "
		                 "| tee candy", dir_str, dir_str);
		system(tmp_str);

		candy2rank(num_str, bwd_str);
	}

	fclose(fbody);
	return 0;
}

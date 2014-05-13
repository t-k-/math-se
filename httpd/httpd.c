#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include "../inter-def.h"

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
	char arg[2048];
	char cmd[2048];
	char line[4096];
	FILE *f_rank = NULL;

	scanf("%s", arg);
	
	replace_plus(arg);
	curl = curl_easy_init();
	unescape = curl_easy_unescape(curl, arg, 0, NULL);

	printf("Content-type: text/html\n\n");
	printf("<html><head><style>"
			"div.container { "
			"height:150px;"
			"position:absolute;"
			"left:50%%;"
			"top:50%%;"
			"margin: 0 0 0 -240px;"
			"}"
			"</style>"
			"</head>"
			"<body>"
			"<div class=\"container\" align=\"center\">");

	sprintf(arg, "%s", unescape);
	arg[0] = ' ';
	arg[1] = ' ';
	free(unescape);

	sprintf(cmd, "./ma-se '%s'", arg);
	system(cmd);
	printf("<b>for query: %s </b><br/>", unescape);

	printf("rank: </br>");
	f_rank = fopen("rank", "r");
	if (f_rank == NULL) {
		printf("no thing we can find :( <br/>");
	}

	while (fscanf(f_rank, "%s\n", line) != EOF) {
		printf("[%s]</br>", line);
	}

	printf( "</div>"
			"</body>"
			"</html>");
	
	fclose(f_rank);
	return 0;
}

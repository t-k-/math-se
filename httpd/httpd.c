#include <stdio.h>
#include <curl/curl.h>

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

	scanf("%s", arg);

	printf("raw: %s <br/>", arg);
	
	replace_plus(arg);
	printf("+ replaced: %s <br/>", arg);

	curl = curl_easy_init();
	unescape = curl_easy_unescape(curl, arg, 0, NULL);
	printf("escaped: %s", unescape);

	system("./ma-se 'a+b'");
	return 0;
}

#include <stdio.h>
#include <sys/types.h>
#include <regex.h>
#include <string.h>

int main()
{
	char *l = "adkjfalkdf20010:myname:100:25ajdkfjalf";
	char list[128];
	list[0] = '\0';
	strcpy(list, l);
	char *regex = "\\([0-9]\\{5\\}\\):\\([^:]\\{1,\\}\\):\\([0-9]\\{3\\}\\):\\([0-9]\\{2\\}\\)";
	regex_t preg;

	int res = regcomp(&preg, regex, 0);
	printf("regex: %s res: %d\n", regex, res);

	size_t nmatch = 5;
	regmatch_t pmatch[5];
	res = regexec(&preg, list, nmatch, pmatch, 0);
	printf("regexec: %s res: %d\n", list, res);

	printf("so: %d eo: %d\n", pmatch[0].rm_so, pmatch[0].rm_eo);
	printf("so: %d eo: %d\n", pmatch[1].rm_so, pmatch[1].rm_eo);
	printf("so: %d eo: %d\n", pmatch[2].rm_so, pmatch[2].rm_eo);
	printf("so: %d eo: %d\n", pmatch[3].rm_so, pmatch[3].rm_eo);
	printf("so: %d eo: %d\n", pmatch[4].rm_so, pmatch[4].rm_eo);

	regfree(&preg);

	return 0;
}

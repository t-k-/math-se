#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <linux/limits.h>
#include <string.h>

#ifndef PATH_MAX
#error "bad"
#endif

int dir_find(DIR *dir, const char *dname)
{
	struct dirent *ent;
	rewinddir(dir);

	while (1) {
		ent = readdir(dir);
		if (!ent)
			break;

		if (ent->d_type & DT_DIR) {
			if (strcmp(dname, "..") == 0 ||
			    strcmp(dname, ".") == 0)
				continue;

			if (0 == strcmp(dname, ent->d_name)) {
				return 1;
			}
		}
	}

	return 0;
}

void dir_merge(char (*path)[PATH_MAX], unsigned long int n)
{
	int i;
	const char *dname;
	struct dirent *ent;
	char (*next_dir)[PATH_MAX]; 
	DIR **dir = calloc(n, sizeof(DIR*)); 

	for (i = 0; i < n; i++) {
		dir[i] = opendir(path[i]);
		if (NULL == dir[i]) {
			printf("NULL open! [%s]\n", path[i]);
			goto exit;
		}
	}

	while(1) {
		ent = readdir(dir[0]);
		if (!ent)
			break;
		
		dname = ent->d_name;
		if (ent->d_type & DT_DIR) {
			if (strcmp(dname, "..") == 0 ||
			 strcmp(dname, "eq_class") == 0 || ////////////asdfasf
			    strcmp(dname, ".") == 0)
				continue;
			    
			for (i = 1; i < n; i++) {
				if (!dir_find(dir[i], dname)) 
					break;
			}

			if (i == n) {
				next_dir = malloc(PATH_MAX * n);
				for (i = 0; i < n; i++) {
					sprintf(next_dir[i], "%s/%s", path[i], dname);
				}
				dir_merge(next_dir, n);
				free(next_dir);
			}
		} else {
			for (i = 0; i < n; i++) {
				char aaa[PATH_MAX];
				sprintf(aaa, "%s/%s", path[i], dname);
				printf("%s\n", aaa);
			}
			printf("\n");
		}
	}

exit:
	for (i = 0; i < n; i++) {
		if (NULL != dir[i])
			closedir(dir[i]);
	}
	
	free(dir);
}

int
main()
{
	//char dirname[][PATH_MAX] = {"./dir0", "./dir1", "./dir2"};
	char dirname[][PATH_MAX] = {
	"/home/tk/math-se/col/var/add", 
	"/home/tk/math-se/col/var/times/add" 
	};

	dir_merge(dirname, sizeof(dirname) / PATH_MAX);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

struct brw_blk {
	unsigned int  frml_id;
	unsigned char factor[25];
	unsigned char idx;
	unsigned char num;
	unsigned char var;
};

unsigned int write(const char *fname)
{
	FILE *fh = fopen(fname, "a");
	struct brw_blk blk;
	int i, delta;
	unsigned int check_sum = 0;
	
	if (fh == NULL)
		return;

	blk.frml_id = 0;
	for (i = 0; i < 25; i++)
		blk.factor[i] = 25 + i;
	blk.idx = 0;
	blk.num = 0;
	blk.var = 0;
	delta = rand() % 300;
	for (i = 0; i < 15000 + delta; i++) {
		blk.frml_id += rand() % 5;
		blk.idx ++;
		blk.num = (blk.num + 1) % 2;
		blk.var = blk.num + blk.idx;
		fwrite(&blk, sizeof(struct brw_blk), 1, fh);
		check_sum += blk.frml_id;
	}
	
	fclose(fh);
	return check_sum;
}

#define BRWBLK_RD_NUM 4096 // 8
#define BRWBLK_UNIT_SZ sizeof(struct brw_blk)
#define BRWBLK_RD_BUF_SZ (BRWBLK_RD_NUM * BRWBLK_UNIT_SZ) 

int read(const char *fname)
{
	FILE *fh = fopen(fname, "r");
	char buff[BRWBLK_RD_BUF_SZ];
	struct brw_blk *blk;
	int j, nread, res = 0;
	
	if (fh == NULL)
		return;
	
	do {
		nread = fread(buff, BRWBLK_UNIT_SZ, BRWBLK_RD_NUM, fh);
		if (nread <= 0)
			break;
		printf("%s: size read = %d\n", fname, nread);
		for (j = 0; j< nread; j++) {
			blk = (struct brw_blk*)buff + j;
			printf("%s: %d\n", fname, blk->frml_id);
			res += blk->frml_id;
		}
	} while (1);
	
	fclose(fh);
	return res;
}

unsigned int read2(char (*fname)[32], int n)
{
	int i;
	FILE **fh = calloc(n, sizeof(FILE*));
	struct brw_blk (*buf)[BRWBLK_RD_NUM];
	unsigned int *idx, *end, nread, min;
	unsigned int check_sum = 0;

	/* initialization */
	for (i = 0; i < n; i++) {
		fh[i] = fopen(fname[i], "r");
		if (fh[i] == NULL) 
			goto exit;
	}
	
	buf = malloc(n * BRWBLK_RD_BUF_SZ);
	idx = malloc(n * sizeof(unsigned int));
	end = malloc(n * sizeof(unsigned int));

	for (i = 0; i < n; i++) {
		idx[i] = BRWBLK_RD_NUM;
		end[i] = BRWBLK_RD_NUM + 1;
	}
	
	do { 
		/* update buffer when needed */
		for (i = 0; i < n; i++) {
			if (idx[i] == end[i]) {
				continue;
			} else if (idx[i] == BRWBLK_RD_NUM) {
				nread = fread(buf[i], BRWBLK_UNIT_SZ, 
				              BRWBLK_RD_NUM, fh[i]);

				printf("...........read %d\n", nread);
				idx[i] = 0;
				if (nread < BRWBLK_RD_NUM)
					end[i] = nread;
			}
		}

		/* find min value to fetch */
		min = ~0;
		for (i = 0; i < n; i++)
			if (idx[i] != end[i])
				if (buf[i][idx[i]].frml_id < min)
					min = buf[i][idx[i]].frml_id;

		/* nothing to fetch for any pass, break */
		if (min == ~0)
			break;

		/* skip all min values for each pass */
		for (i = 0; i < n; i++) {
			while (idx[i] < BRWBLK_RD_NUM && idx[i] < end[i]) {
				if (buf[i][idx[i]].frml_id == min) {
					//printf("%s: %d\n", fname[i], min);
					check_sum += min;
					idx[i] ++;
				} else
					break;
			}
		}
	} while (1);

	free(end);
	free(idx);
	free(buf);

exit:
	for (i = 0; i < n; i++) {
		if (fh[i] != NULL)
			fclose(fh[i]);
	}

	free(fh);

	return check_sum;
}

int
main()
{
	unsigned int check_sum = 0;
	char fname[][32] = {"posting-1.bin", 
	                    "posting-2.bin", 
	                    "posting-3.bin",
	                    "posting-4.bin"
						};
	srand(time(NULL));
	check_sum += write("posting-1.bin");
	check_sum += write("posting-2.bin");
	check_sum += write("posting-3.bin");
	check_sum += write("posting-4.bin");
	printf("check_sum = %d\n", check_sum);

	//read("posting-1.bin");
	//read("posting-2.bin");
	//read("posting-3.bin");

	check_sum = read2(fname, 4);
	
	printf("check_sum = %d\n", check_sum);
	return 0;
}

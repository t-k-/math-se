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

void write(const char *fname)
{
	FILE *fh = fopen(fname, "a");
	struct brw_blk blk;
	int i, delta;
	
	if (fh == NULL)
		return;

	blk.frml_id = 0;
	delta = rand() % 10;
	for (i = 0; i < 30 + delta; i++) {
		blk.frml_id += rand() % 5;
		fwrite(&blk, sizeof(struct brw_blk), 1, fh);
	}
	
	fclose(fh);
}

#define BRWBLK_RD_NUM 8 // 4096
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
	
	printf("%s:", fname);
	do {
		nread = fread(buff, BRWBLK_UNIT_SZ, BRWBLK_RD_NUM, fh);
		if (nread <= 0)
			break;
		printf("\n(size read = %d) ", nread);
		for (j = 0; j< nread; j++) {
			blk = (struct brw_blk*)buff + j;
			printf("%d ", blk->frml_id);
			res += blk->frml_id;
		}
	} while (1);
	
	printf("\n\n");
	fclose(fh);
	return res;
}

static void 
rebuf(void *buf, FILE *fh,
      unsigned int *idx, unsigned int *end)
{
	unsigned int nread;
	nread = fread(buf, BRWBLK_UNIT_SZ, BRWBLK_RD_NUM, fh);
	*idx = 0;
	*end = nread;
	printf("rebuf: read %d\n", nread);
}

static void 
jump(unsigned int target_id, struct brw_blk *buf, FILE *fh,
     unsigned int *idx, unsigned int *end)
{
	unsigned int nread, i, idx_far;
	do {
		idx_far = *end - 1;
		if (buf[idx_far].frml_id >= target_id)
			break;
		rebuf(buf, fh, idx, end);
	} while (*idx < *end);

	if (*idx == *end)
		return;

	for (i = *idx; i < *end; i++) {
		if (buf[i].frml_id >= target_id)
			break;
	}

	*idx = i; 
	printf("jump to (%d)\n", buf[i].frml_id);
}

unsigned int posmerge(char (*fname)[32], int n)
{
	int i, term_cond;
	FILE **fh = calloc(n, sizeof(FILE*));
	struct brw_blk (*buf)[BRWBLK_RD_NUM];
	unsigned int *idx, *end, max, id, diff;

	for (i = 0; i < n; i++) {
		fh[i] = fopen(fname[i], "r");
		if (fh[i] == NULL) {
			printf("open error: %s\n", fname[i]);
			goto exit;
		}
	}
	
	buf = malloc(n * BRWBLK_RD_BUF_SZ);
	idx = malloc(n * sizeof(unsigned int));
	end = malloc(n * sizeof(unsigned int));

	if (buf == NULL || idx == NULL || end == NULL)
		goto exit;

	for (i = 0; i < n; i++)
		rebuf(&buf[i], fh[i], &idx[i], &end[i]);

	do { 
		max = buf[0][idx[0]].frml_id;
		diff = 0;
		for (i = 1; i < n; i++) {
			id = buf[i][idx[i]].frml_id;
			if (id != max) {
				if (id > max) 
					max = id;
				diff = 1;
			}
		}

		printf("for (%d) and (%d): max=%d\n", 
		       buf[0][idx[0]].frml_id, buf[1][idx[1]].frml_id, max);
		
		term_cond = 0;
		if (diff) {
			for (i = 0; i < n; i++) {
				id = buf[i][idx[i]].frml_id;
				if (id != max) {
					printf("(%d) @ pipe %d ", 
					       id, i, max);
					jump(max, (struct brw_blk*)&buf[i], fh[i], 
					     &idx[i], &end[i]);
					if (idx[i] == end[i]) {
						term_cond = 1;
						break;
					}
				}
			}
		} else {
			for (i = 0; i < n; i++) {
				while (idx[i] < end[i]) {
					id = buf[i][idx[i]].frml_id;
					if (id != max)
						break;

					printf("(%d) @ pipe %d calls\n", id, i);
					idx[i] ++;
					if (idx[i] == BRWBLK_RD_NUM) {
						rebuf(&buf[i], fh[i], &idx[i], &end[i]);
					}
				}

				if (idx[i] == end[i]) 
					term_cond = 1;
			}
		}
	} while (!term_cond);

	free(end);
	free(idx);
	free(buf);

exit:
	for (i = 0; i < n; i++) {
		if (fh[i] != NULL)
			fclose(fh[i]);
	}

	free(fh);

	return 0;
}

int
main()
{
	char fname[][32] = {"posting-0.bin", "posting-1.bin"};
	srand(123);

	system("rm -f *.bin");
	write("posting-0.bin");
	write("posting-1.bin");

	read("posting-0.bin");
	read("posting-1.bin");

	posmerge(fname, 2);

	return 0;
}

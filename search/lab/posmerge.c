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
	delta = rand() % 50000;
	for (i = 0; i < 150000 + delta; i++) {
		blk.frml_id += rand() % 500;
		fwrite(&blk, sizeof(struct brw_blk), 1, fh);
	}
	
	fclose(fh);
}

#define SIZE sizeof(struct brw_blk)

void write_case0(const char *fname)
{
	FILE *fh = fopen(fname, "a");
	struct brw_blk blk;
	if (fh == NULL)
		return;
	blk.frml_id = 1; fwrite(&blk, SIZE, 1, fh);
	blk.frml_id = 2; fwrite(&blk, SIZE, 1, fh);
	blk.frml_id = 3; fwrite(&blk, SIZE, 1, fh);
	blk.frml_id = 4; fwrite(&blk, SIZE, 1, fh);

	blk.frml_id = 5; fwrite(&blk, SIZE, 1, fh);
	blk.frml_id = 6; fwrite(&blk, SIZE, 1, fh);
	blk.frml_id = 7; fwrite(&blk, SIZE, 1, fh);
	blk.frml_id = 8; fwrite(&blk, SIZE, 1, fh);
	
	blk.frml_id = 9; fwrite(&blk, SIZE, 1, fh);
	blk.frml_id = 9; fwrite(&blk, SIZE, 1, fh);
	blk.frml_id = 9; fwrite(&blk, SIZE, 1, fh);
	blk.frml_id = 10; fwrite(&blk, SIZE, 1, fh);
	
	blk.frml_id = 12; fwrite(&blk, SIZE, 1, fh);
	blk.frml_id = 15; fwrite(&blk, SIZE, 1, fh);
	
	fclose(fh);
}

void write_case1(const char *fname)
{
	FILE *fh = fopen(fname, "a");
	struct brw_blk blk;
	if (fh == NULL)
		return;
	blk.frml_id = 0; fwrite(&blk, SIZE, 1, fh);
	blk.frml_id = 2; fwrite(&blk, SIZE, 1, fh);
	blk.frml_id = 14; fwrite(&blk, SIZE, 1, fh);
	blk.frml_id = 15; fwrite(&blk, SIZE, 1, fh);
	
	fclose(fh);
}

#define BRWBLK_RD_NUM 4096 //4
#define BRWBLK_UNIT_SZ sizeof(struct brw_blk)
#define BRWBLK_RD_BUF_SZ (BRWBLK_RD_NUM * BRWBLK_UNIT_SZ) 

void tracebuf(struct brw_blk* buff, 
              unsigned int idx, unsigned int end) 
{
	unsigned int j;
	struct brw_blk *blk;
	for (j = 0; j < BRWBLK_RD_NUM; j++) {
		blk = buff + j;
		if (j == idx)
			printf("[%d]", blk->frml_id);
		else if (j >= end)
			printf("<%d>", blk->frml_id);
		else
			printf("(%d)", blk->frml_id);
	}
	printf("\n");
}

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

static unsigned int 
rebuf(void *buf, FILE *fh,
      unsigned int *idx, unsigned int *end)
{
	unsigned int nread, skips = *end - *idx;
	nread = fread(buf, BRWBLK_UNIT_SZ, BRWBLK_RD_NUM, fh);
	*idx = 0;
	*end = nread;

	return skips;
}

static unsigned int
jump(unsigned int target, struct brw_blk *buf, FILE *fh,
     unsigned int *idx, unsigned int *end)
{
	unsigned int idx_far, jumps = 0;
	do {
		idx_far = *end - 1;
		if (buf[idx_far].frml_id >= target)
			break;
		
		jumps += rebuf(buf, fh, idx, end);
		
		if (*end != BRWBLK_RD_NUM)
			return 0; 
	} while (1);

	while (*idx < *end && buf[*idx].frml_id < target) {
		(*idx) ++;
		jumps ++;
	}

	return jumps;
}

unsigned int posmerge(char (*fname)[32], int n)
{
	FILE **fh = calloc(n, sizeof(FILE*));
	struct brw_blk (*buf)[BRWBLK_RD_NUM];
	unsigned int i, *idx, *end, max, id, diff, go_on, jumps = 0;

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
		//printf("%d: (%d) \n", 0, max);
		for (i = 1; i < n; i++) {
			id = buf[i][idx[i]].frml_id;
			//printf("%d: (%d) \n", i, id);
			if (id != max) {
				if (id > max) 
					max = id;
				diff = 1;
			}
		}
		
		go_on = 1;
		if (diff) {
			for (i = 0; i < n; i++) {
				id = buf[i][idx[i]].frml_id;
				if (id != max) {
					go_on = jump(max, (struct brw_blk*)&buf[i], 
				                 fh[i], &idx[i], &end[i]); 
					if (!go_on)
						break;
					else if (go_on > 1) {
						//printf("jumped %d\n", go_on);
						jumps += go_on;
					}
				}
			}
		} else {
			for (i = 0; i < n; i++) {
				while (buf[i][idx[i]].frml_id == max) {
					printf("[%d] @ pipe %d calls\n", 
					       buf[i][idx[i]].frml_id, i);
					idx[i] ++;
					if (idx[i] == end[i]) {
						if (end[i] == BRWBLK_RD_NUM) {
							rebuf(&buf[i], fh[i], &idx[i], &end[i]);
						} else {
							go_on = 0;
							break;
						}
					}
				}
			}
		}
	} while (go_on);

	free(end);
	free(idx);
	free(buf);

exit:
	for (i = 0; i < n; i++) {
		if (fh[i] != NULL)
			fclose(fh[i]);
	}

	free(fh);

	return jumps;
}

int
main()
{
	char fname[][32] = {"posting-0.bin", 
	                    "posting-1.bin",
	                    "posting-2.bin"};
	srand(3909);
	unsigned int i, jumps;

	system("rm -f *.bin");
	write("posting-0.bin");
	write("posting-1.bin");
	write("posting-2.bin");
//	write_case0("posting-0.bin");
//	write_case1("posting-1.bin");

	//read("posting-0.bin");
	//read("posting-1.bin");

	printf("begin.\n");
	for (i = 0; i < 50; i++) {
		jumps = posmerge(fname, 3);
	}

	printf("total jumps: %d \n", jumps);
	return 0;
}

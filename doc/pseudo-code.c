#define T_ID uint
#define T_SCORE float;

struct tex_tr *tex_parse(char *tex);

list<struct dbrw> tex_tr_gen_dbrw(struct tex_tr *tr);

char *tex_tr_print(struct tex_tr *tr);

list<struct tex_tr> tex_tr_merge_points(struct tex_tr *tr);
		
void sort_by_vname(list<struct dbrw>);
		
list<struct search_dir> = gen_search_dir(list<struct dbrw>);

/* size = 28 */
struct brw {
	uchar idx; /* identify the branch in score tree */
	uchar n;
	uchar vname;
	uchar sons[25];
};

struct search_dir {
	char *dir;
	struct dbrw *dbrw_ref[MAX_DBRW_REF];
	(p,q) rlv_stack[FRML_MAX_BRW];
};
struct dbrw {
	char *dir;
	struct brw brw;
	struct search_dir *srch_entry;
};
struct post_rec {
	T_ID        id; /* formula ID */
	struct brw brw;
};
struct score_tr_var {
	uint n_leaf;
	uchar vname;
	T_SCORE score;
};

struct champ_unit {
	T_SCORE max;
	struct check_unit *who;
};
struct check_unit {
	uchar idx;
	uint ck_mark;
};
struct score_tr {
	uint n_var, n_qbrw;
	T_SCORE max, score;	
	struct check_unit *who;
	struct score_tr_var var[n_var];
	struct champ_unit champ_board[n_qbrw][n_var];
	struct check_unit check_board[n_var][n_leaf];
};

void parse(url)
{
	write(url);
	while input(tex) {
		tex_tr = tex_parse(tex);
		li_dbrw = tex_tr_gen_dbrw(tex_tr->root);
		tr_out = tex_tr_print(tex_tr);
		write(tex, tr_out);
		for dbrw in li_dbrw {
			wrtie(dbrw.dir, dbrw.brw);
		}
	}
}

void index()
{
	/* output:
	 * ./col/hdb {id}->{#brw, url, tex, tree}
	 * ./col/brw-path/posting {max, min, records[]}
	 */
}

void search(tex)
{
	cnt = 0;

	tex_tr = tex_parse(tex);
	score_tr = score_tr_new();

	/* generate merge points in depth order */
	li_merges = tex_tr_merge_points(tex_tr);

	for merge in li_merges {
		li_query_dbrw = tex_tr_gen_dbrw(merge);

		/* sort and re-arrange brw idx */
		li_query_dbrw = sort_by_vname(li_query_dbrw);

		/* unique path, each path contains 
		 * multiple source query brw(s) */
		li_search_dir = gen_search_dir(li_query_dbrw);

		if ! merge_search(li_query_dbrw, li_search_dir, 
		                  score_tr, &cnt) 
			break;
	}
	score_tr_dele(score_tr)
}

int merge_search_callbk(struct post_rec matched, instr, srch_entry, 
                         li_query_dbrw, score_tr, &cnt)
{
	if instr == FIRST_CALL {
		if ranking_set_has(matched.id) 
			return SKIP_THIS_ID;
	}  

	if instr != SKIP_THIS_ID {
		leaf = score_tr_insert(score_tr, matched);
		srch_entry.rlv_stack.push(leaf);
		for dbrw in srch_entry.dbrw_ref 
			score_cache_put(matched.brw, dbrw.brw);
	}

	if instr == END_CALL {
		score = score_tr_calc(score_tr, li_query_dbrw);
		score_tr_clear(score_tr);
		nbrw = hdb_get_nbrw(matched.id);
		score = 1000 * score + 1 / nbrw; 
		ranking_set_add(score, matched.id);

		if (++cnt) >= THRESHOLD 
			return ABORT_ALL;
	}
	
	return NEXT;
}

void score_tr_clear()
{
	score_tr.score = 0;
	score_tr.n_var = 0;
	score_tr.n_qbrw = 0;
}

T_SCORE score_tr_calc(score_tr, li_query_dbrw)
{
	k = 0;

	for q_dbrw in li_query_dbrw {
		i = q_bdrw.brw.idx;
		champs = score_tr.champ_board[i];
		for p, q in q_dbrw.srch_entry->rlv_stack {
			leaf = score_tr.check_board[p][q];
			champ = champs[p];
			if leaf.ck_mark == unmark {
				score = score_cache_get(i, leaf.idx);
				if score > champ.max {
					if champ.who 
						champ.who->ck_mark = unmark;
					champ.max = score;
					champ.who = &leaf;
					leaf.ck_mark = mark;
				}
			}
		}
	
		for j = 0; j < score_tr.n_var; j++ {
			score_tr.var[j].score += champs[j].max; 
			if score_tr.var[j].score > score_tr.max {
				score_tr.max = score_tr.var[j].score;
				score_tr.who = &score_tr.var[j];
			}
		}

		stage_flag = 0;
		if last_iteration()  
			stage_flag = 1;
		else if next_vname() != q_brw.vname 
			stage_flag = 1;

		if stage_flag {
			for j = 0; j < score_tr.n_var; j++ {
				test = (score_tr.who == &score_tr.var[j]);
				next_state = test ? cross : unmark;
				for l = k; l <= i; l ++ {
					champ = score_tr.champ_board[l][j];
					if champ.who 
						champ.who.state = next_state;
				}
			}
			memset(score_tr.var);
			memset(score_tr.champ_board);

			score_tr.score += score_tr.max;
			score_tr.max = 0;
			score_tr.who = NULL;

			k = i;
		}
	}

	memset(score_tr.check_board);
	return score_tr.score;
}

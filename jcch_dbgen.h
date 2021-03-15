#pragma once

/**
 * tpch_dbgen.c/h among other things contain the data originating from dists.dss and are therefore subject to the TPCH
 * license (see LICENSE)
 */

#define NATIONS_COUNT 25
#define REGIONS_COUNT 5

typedef struct
{
  long      weight;
  char     *text;
  long      len;
}         set_member;

typedef struct
{
  int      count;
  int      max;
  set_member *list;
  long *permute;
}         distribution;


extern distribution jcch_nations;
extern distribution jcch_regions;
extern distribution jcch_o_priority_set;
extern distribution jcch_l_instruct_set;
extern distribution jcch_l_smode_set;
extern distribution jcch_l_category_set;
extern distribution jcch_l_rflag_set;
extern distribution jcch_c_mseg_set;
extern distribution jcch_colors;
extern distribution jcch_p_types_set;
extern distribution jcch_p_cntr_set;

/* distributions that control text generation */
extern distribution jcch_articles;
extern distribution jcch_nouns;
extern distribution jcch_adjectives;
extern distribution jcch_adverbs;
extern distribution jcch_prepositions;
extern distribution jcch_verbs;
extern distribution jcch_terminators;
extern distribution jcch_auxillaries;
extern distribution jcch_np;
extern distribution jcch_vp;
extern distribution jcch_grammar;

extern char** jcch_asc_date;

void jcch_dbgen_reset_seeds();
void jcch_dbgen_init_scale_factor(float scale_factor);
void jcch_dbgen_cleanup();

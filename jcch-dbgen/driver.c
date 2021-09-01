/*
* $Id: driver.c,v 1.7 2008/09/24 22:35:21 jms Exp $
*
* Revision History
* ===================
* $Log: driver.c,v $
* Revision 1.7  2008/09/24 22:35:21  jms
* remove build number header
*
* Revision 1.6  2008/09/24 22:30:29  jms
* remove build number from default header
*
* Revision 1.5  2008/03/21 17:38:39  jms
* changes for 2.6.3
*
* Revision 1.4  2006/04/26 23:01:10  jms
* address update generation problems
*
* Revision 1.3  2005/10/28 02:54:35  jms
* add release.h changes
*
* Revision 1.2  2005/01/03 20:08:58  jms
* change line terminations
*
* Revision 1.1.1.1  2004/11/24 23:31:46  jms
* re-establish external server
*
* Revision 1.5  2004/04/07 20:17:29  jms
* bug #58 (join fails between order/lineitem)
*
* Revision 1.4  2004/02/18 16:26:49  jms
* 32/64 bit changes for overflow handling needed additional changes when ported back to windows
*
* Revision 1.3  2004/01/22 05:49:29  jms
* AIX porting (AIX 5.1)
*
* Revision 1.2  2004/01/22 03:54:12  jms
* 64 bit support changes for customer address
*
* Revision 1.1.1.1  2003/08/08 21:50:33  jms
* recreation after CVS crash
*
* Revision 1.3  2003/08/08 21:35:26  jms
* first integration of rng64 for o_custkey and l_partkey
*
* Revision 1.2  2003/08/07 17:58:34  jms
* Convery RNG to 64bit space as preparation for new large jcch_scale RNG
*
* Revision 1.1.1.1  2003/04/03 18:54:21  jms
* initial checkin
*
*
*/
/* main driver for dss banchmark */

#define DECLARER				/* EXTERN references get defined here */
#define NO_FUNC (int (*) ()) NULL	/* to clean up jcch_tdefs */
#define NO_LFUNC (long (*) ()) NULL		/* to clean up jcch_tdefs */

#include "config.h"
#include "release.h"
#include <stdlib.h>
#if (defined(_POSIX_)||!defined(WIN32))		/* Change for Windows NT */
#include <unistd.h>
#include <sys/wait.h>
#endif /* WIN32 */
#include <stdio.h>				/* */
#include <limits.h>
#include <math.h>
#include <ctype.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#ifdef HP
#include <strings.h>
#endif
#if (defined(WIN32)&&!defined(_POSIX_))
#include <process.h>
#pragma warning(disable:4201)
#pragma warning(disable:4214)
#pragma warning(disable:4514)
#define WIN32_LEAN_AND_MEAN
#define NOATOM
#define NOGDICAPMASKS
#define NOMETAFILE
#define NOMINMAX
#define NOMSG
#define NOOPENFILE
#define NORASTEROPS
#define NOSCROLL
#define NOSOUND
#define NOSYSMETRICS
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOMCX
#include <windows.h>
#pragma warning(default:4201)
#pragma warning(default:4214)
#endif

#ifdef JCCH_SKEW
#include "skew/phash.h"
int JCCH_skew = 0;
#endif

#include "dss.h"
#include "dsstypes.h"
#include "jcch_dbgen.h"

/*
* Function prototypes
*/
void	jcch_usage (void);
void	kill_load (void);
int		pload (int tbl);
void	jcch_gen_tbl (int tnum, JCCH_DSS_HUGE start, JCCH_DSS_HUGE count, long jcch_upd_num);
int		pr_drange (int tbl, JCCH_DSS_HUGE min, JCCH_DSS_HUGE cnt, long num);
int		jcch_set_files (int t, int pload);
int		jcch_partial (int, int);


extern int optind, opterr;
extern char *optarg;
JCCH_DSS_HUGE jcch_rowcnt = 0, jcch_minrow = 0;
long jcch_upd_num = 0;
double jcch_flt_scale;
#if (defined(WIN32)&&!defined(_POSIX_))
char *spawn_args[25];
#endif
#ifdef RNG_TEST
extern seed_t jcch_Seed[];
#endif
static int bTableSet = 0;


/*
* general jcch_table descriptions. See dss.h for details on structure
* NOTE: tables with no scaling info are scaled according to
* another jcch_table
*
*
* the following is based on the tdef structure defined in dss.h as:
* typedef struct
* {
* char     *name;            -- name of the jcch_table; 
*                               flat file output in <name>.tbl
* long      base;            -- base jcch_scale rowcount of jcch_table; 
*                               0 if derived
* int       (*loader) ();    -- function to present output
* long      (*gen_seed) ();  -- functions to seed the RNG
* int       child;           -- non-zero if there is an associated detail jcch_table
* unsigned long vtotal;      -- "checksum" total 
* }         tdef;
*
*/

/*
* flat file print functions; used with -F(lat) option
*/
int pr_cust (jcch_customer_t * c, int mode);
int pr_line (jcch_order_t * o, int mode);
int pr_order (jcch_order_t * o, int mode);
int pr_part (part_t * p, int mode);
int pr_psupp (part_t * p, int mode);
int pr_supp (supplier_t * s, int mode);
int pr_order_line (jcch_order_t * o, int mode);
int pr_part_psupp (part_t * p, int mode);
int pr_nation (code_t * c, int mode);
int pr_region (code_t * c, int mode);

/*
* seed generation functions; used with '-O s' option
*/
long jcch_sd_cust (int child, JCCH_DSS_HUGE skip_count);
long jcch_sd_line (int child, JCCH_DSS_HUGE skip_count);
long jcch_sd_order (int child, JCCH_DSS_HUGE skip_count);
long jcch_sd_part (int child, JCCH_DSS_HUGE skip_count);
long jcch_sd_psupp (int child, JCCH_DSS_HUGE skip_count);
long jcch_sd_supp (int child, JCCH_DSS_HUGE skip_count);
long jcch_sd_order_line (int child, JCCH_DSS_HUGE skip_count);
long jcch_sd_part_psupp (int child, JCCH_DSS_HUGE skip_count);

tdef jcch_tdefs[] =
{
	{"part.tbl", "part jcch_table", 200000,
		pr_part, jcch_sd_part, JCCH_PSUPP, 0},
	{"partsupp.tbl", "partsupplier jcch_table", 200000,
		pr_psupp, jcch_sd_psupp, NONE, 0},
	{"supplier.tbl", "suppliers jcch_table", 10000,
		pr_supp, jcch_sd_supp, NONE, 0},
	{"customer.tbl", "customers jcch_table", 150000,
		pr_cust, jcch_sd_cust, NONE, 0},
	{"orders.tbl", "order jcch_table", 150000,
		pr_order, jcch_sd_order, JCCH_LINE, 0},
	{"lineitem.tbl", "lineitem jcch_table", 150000,
		pr_line, jcch_sd_line, NONE, 0},
	{"orders.tbl", "orders/lineitem tables", 150000,
		pr_order_line, jcch_sd_order, JCCH_LINE, 0},
	{"part.tbl", "part/partsupplier tables", 200000,
		pr_part_psupp, jcch_sd_part, JCCH_PSUPP, 0},
	{"nation.tbl", "nation jcch_table", NATIONS_MAX,
		pr_nation, NO_LFUNC, NONE, 0},
	{"region.tbl", "region jcch_table", NATIONS_MAX,
		pr_region, NO_LFUNC, NONE, 0},
};

/*
* re-set default output file names 
*/
int
jcch_set_files (int i, int pload)
{
	char line[80], *new_name;
	
	if (jcch_table & (1 << i))
child_table:
	{
		if (pload != -1)
			sprintf (line, "%s.%d", jcch_tdefs[i].name, pload);
		else
		{
			printf ("Enter new destination for %s data: ",
				jcch_tdefs[i].name);
			if (fgets (line, sizeof (line), stdin) == NULL)
				return (-1);;
			if ((new_name = strchr (line, '\n')) != NULL)
				*new_name = '\0';
			if ((int)strlen (line) == 0)
				return (0);
		}
		new_name = (char *) malloc ((int)strlen (line) + 1);
		MALLOC_CHECK (new_name);
		strcpy (new_name, line);
		jcch_tdefs[i].name = new_name;
		if (jcch_tdefs[i].child != NONE)
		{
			i = jcch_tdefs[i].child;
			jcch_tdefs[i].child = NONE;
			goto child_table;
		}
	}
	
	return (0);
}



/*
* read the distributions needed in the benchamrk
*/
void
jcch_load_dists (void)
{
	jcch_read_dist (jcch_env_config (DIST_TAG, DIST_DFLT), "p_cntr", &jcch_p_cntr_set);
	jcch_read_dist (jcch_env_config (DIST_TAG, DIST_DFLT), "jcch_colors", &jcch_colors);
	jcch_read_dist (jcch_env_config (DIST_TAG, DIST_DFLT), "p_types", &jcch_p_types_set);
	jcch_read_dist (jcch_env_config (DIST_TAG, DIST_DFLT), "jcch_nations", &jcch_nations);
	jcch_read_dist (jcch_env_config (DIST_TAG, DIST_DFLT), "jcch_regions", &jcch_regions);
	jcch_read_dist (jcch_env_config (DIST_TAG, DIST_DFLT), "o_oprio",
		&jcch_o_priority_set);
	jcch_read_dist (jcch_env_config (DIST_TAG, DIST_DFLT), "instruct",
		&jcch_l_instruct_set);
	jcch_read_dist (jcch_env_config (DIST_TAG, DIST_DFLT), "smode", &jcch_l_smode_set);
	jcch_read_dist (jcch_env_config (DIST_TAG, DIST_DFLT), "category",
		&jcch_l_category_set);
	jcch_read_dist (jcch_env_config (DIST_TAG, DIST_DFLT), "rflag", &jcch_l_rflag_set);
	jcch_read_dist (jcch_env_config (DIST_TAG, DIST_DFLT), "msegmnt", &jcch_c_mseg_set);

	/* load the distributions that contain text generation */
	jcch_read_dist (jcch_env_config (DIST_TAG, DIST_DFLT), "jcch_nouns", &jcch_nouns);
	jcch_read_dist (jcch_env_config (DIST_TAG, DIST_DFLT), "jcch_verbs", &jcch_verbs);
	jcch_read_dist (jcch_env_config (DIST_TAG, DIST_DFLT), "jcch_adjectives", &jcch_adjectives);
	jcch_read_dist (jcch_env_config (DIST_TAG, DIST_DFLT), "jcch_adverbs", &jcch_adverbs);
	jcch_read_dist (jcch_env_config (DIST_TAG, DIST_DFLT), "jcch_auxillaries", &jcch_auxillaries);
	jcch_read_dist (jcch_env_config (DIST_TAG, DIST_DFLT), "jcch_terminators", &jcch_terminators);
	jcch_read_dist (jcch_env_config (DIST_TAG, DIST_DFLT), "jcch_articles", &jcch_articles);
	jcch_read_dist (jcch_env_config (DIST_TAG, DIST_DFLT), "jcch_prepositions", &jcch_prepositions);
	jcch_read_dist (jcch_env_config (DIST_TAG, DIST_DFLT), "jcch_grammar", &jcch_grammar);
	jcch_read_dist (jcch_env_config (DIST_TAG, DIST_DFLT), "jcch_np", &jcch_np);
	jcch_read_dist (jcch_env_config (DIST_TAG, DIST_DFLT), "jcch_vp", &jcch_vp);
	
}

static jcch_order_t order;
static part_t part;
/*
* generate a particular jcch_table
*/
void
jcch_gen_tbl (int tnum, JCCH_DSS_HUGE start, JCCH_DSS_HUGE count, long jcch_upd_num)
{
	supplier_t supp;
	jcch_customer_t cust;
	code_t code;
	static int completed = 0;
	JCCH_DSS_HUGE i;

	JCCH_DSS_HUGE rows_per_segment=0;
	JCCH_DSS_HUGE rows_this_segment=-1;
	JCCH_DSS_HUGE residual_rows=0;
	
	if (jcch_insert_segments)
		{
		rows_per_segment = count / jcch_insert_segments;
		residual_rows = count - (rows_per_segment * jcch_insert_segments);
		}

	for (i = start; count; count--, i++)
	{
		LIFENOISE (1000, i);
		jcch_row_start(tnum);

		switch (tnum)
		{
		case JCCH_LINE:
		case JCCH_ORDER:
  		case ORDER_LINE: 
			jcch_mk_order (i, &order, jcch_upd_num % 10000);

		  if (jcch_insert_segments  && (jcch_upd_num > 0))
			if((jcch_upd_num / 10000) < residual_rows)
				{
				if((++rows_this_segment) > rows_per_segment) 
					{						
					rows_this_segment=0;
					jcch_upd_num += 10000;					
					}
				}
			else
				{
				if((++rows_this_segment) >= rows_per_segment) 
					{
					rows_this_segment=0;
					jcch_upd_num += 10000;
					}
				}

			if (jcch_set_seeds == 0)
				jcch_tdefs[tnum].loader(&order, jcch_upd_num);
			break;
		case JCCH_SUPP:
			jcch_mk_supp (i, &supp);
			if (jcch_set_seeds == 0)
				jcch_tdefs[tnum].loader(&supp, jcch_upd_num);
			break;
		case JCCH_CUST:
			jcch_mk_cust (i, &cust);
			if (jcch_set_seeds == 0)
				jcch_tdefs[tnum].loader(&cust, jcch_upd_num);
			break;
		case JCCH_PSUPP:
		case JCCH_PART:
  		case PART_PSUPP: 
			jcch_mk_part (i, &part);
			if (jcch_set_seeds == 0)
				jcch_tdefs[tnum].loader(&part, jcch_upd_num);
			break;
		case JCCH_NATION:
			jcch_mk_nation (i, &code);
			if (jcch_set_seeds == 0)
				jcch_tdefs[tnum].loader(&code, 0);
			break;
		case JCCH_REGION:
			jcch_mk_region (i, &code);
			if (jcch_set_seeds == 0)
				jcch_tdefs[tnum].loader(&code, 0);
			break;
		}
		jcch_row_stop(tnum);
		if (jcch_set_seeds && (i % jcch_tdefs[tnum].base) < 2)
		{
			printf("\nSeeds for %s at rowcount %ld\n", jcch_tdefs[tnum].comment, i);
			jcch_dump_seeds(tnum);
		}
	}
	completed |= 1 << tnum;
}

void 
version_copyright(void) 
{
	fprintf (stderr, "%s Population Generator (Version %d.%d.%d build %d)\n",
		NAME, VERSION, RELEASE, PATCH, BUILD);
	fprintf (stderr, "Copyright %s %s\n", TPC, C_DATES);
#ifdef JCCH_SKEW
	fprintf(stderr, "JCC-H: Join Crossing Correlations enabled TPC-H dbgen\n");
#endif
}


void
jcch_usage (void)
{
	fprintf (stderr, 
		"USAGE:\ndbgen [-{vf%s}][-T {pcsoPSOL}]\n\t[-s <jcch_scale>][-C <procs>][-S <jcch_step>]\ndbgen [-v]%s[-O m] [-s <jcch_scale>] [-U <jcch_updates>]\n\n",
#ifdef JCCH_SKEW
	"k", " [-k] ");
#else
	"", " "); 
#endif
	fprintf (stderr, "Basic Options\n==================================\n");
	fprintf (stderr, "-C <n> -- separate data set into <n> chunks (requires -S, default: 1)\n");
	fprintf (stderr, "-f     -- jcch_force. Overwrite existing files\n");
	fprintf (stderr, "-h     -- display this message\n");
	fprintf (stderr, "-q     -- enable QUIET mode\n");
	fprintf (stderr, "-s <n> -- set Scale Factor (SF) to  <n> (default: 1) \n");
	fprintf (stderr, "-S <n> -- build the <n>th jcch_step of the data/update set (used with -C or -U)\n");
	fprintf (stderr, "-U <n> -- generate <n> update sets\n");
	fprintf (stderr, "-v     -- enable VERBOSE mode\n");
	fprintf (stderr, "\nAdvanced Options\n==================================\n");
	fprintf (stderr, "-b <s> -- load distributions for <s> (default: dists.dss)\n");
    fprintf (stderr, "-d <n> -- split deletes between <n> files (requires -U)\n");
    fprintf (stderr, "-i <n> -- split inserts between <n> files (requires -U)\n");
	fprintf (stderr, "-T c   -- generate customers ONLY\n");
	fprintf (stderr, "-T l   -- generate nation/region ONLY\n");
	fprintf (stderr, "-T L   -- generate lineitem ONLY\n");
	fprintf (stderr, "-T n   -- generate nation ONLY\n");
	fprintf (stderr, "-T o   -- generate orders/lineitem ONLY\n");
	fprintf (stderr, "-T O   -- generate orders ONLY\n");
	fprintf (stderr, "-T p   -- generate parts/partsupp ONLY\n");
	fprintf (stderr, "-T P   -- generate parts ONLY\n");
	fprintf (stderr, "-T r   -- generate region ONLY\n");
	fprintf (stderr, "-T s   -- generate suppliers ONLY\n");
	fprintf (stderr, "-T S   -- generate partsupp ONLY\n");
#ifdef JCCH_SKEW
	fprintf (stderr, "\nJoin Crossing Correlations (JCC-H)\n==================================\n");
	fprintf (stderr, "-k     -- introduce correlated join skew\n");
#endif
	fprintf (stderr,
		"\nTo generate the SF=1 (1GB), validation database population, use:\n");
	fprintf (stderr, "\tdbgen -vf -s 1\n");
	fprintf (stderr, "\nTo generate jcch_updates for a SF=1 (1GB), use:\n");
	fprintf (stderr, "\tdbgen -v -U 1 -s 1\n");
}

/*
* int jcch_partial(int tbl, int s) -- generate the s-th part of the named tables data
*/
int
jcch_partial (int tbl, int s)
{
	JCCH_DSS_HUGE jcch_rowcnt;
	JCCH_DSS_HUGE extra;
	
	if (jcch_verbose > 0)
	{
		fprintf (stderr, "\tStarting to load stage %d of %d for %s...",
			s, jcch_children, jcch_tdefs[tbl].comment);
	}
	
	jcch_set_files (tbl, s);
	
	jcch_rowcnt = jcch_set_state(tbl, jcch_scale, jcch_children, s, &extra);

	if (s == jcch_children)
		jcch_gen_tbl (tbl, jcch_rowcnt * (s - 1) + 1, jcch_rowcnt + extra, jcch_upd_num);
	else
		jcch_gen_tbl (tbl, jcch_rowcnt * (s - 1) + 1, jcch_rowcnt, jcch_upd_num);
	
	if (jcch_verbose > 0)
		fprintf (stderr, "done.\n");
	
	return (0);
}

void
jcch_process_options (int count, char **vector)
{
	int option;
	FILE *pF;
	
	while ((option = getopt (count, vector,
		"b:C:d:fi:hO:P:qs:S:T:U:v:k")) != -1)
	switch (option)
	{
		case 'b':				/* load distributions from named file */
			jcch_d_path = (char *)malloc((int)strlen(optarg) + 1);
			MALLOC_CHECK(jcch_d_path);
			strcpy(jcch_d_path, optarg);
			if ((pF = fopen(jcch_d_path, "r")) == NULL)
			{
				fprintf(stderr, "ERROR: Invalid argument to -b");
				exit(-1);
			}
			else
				fclose(pF);

			break;
		case 'C':
			jcch_children = atoi (optarg);
			break;
		case 'd':
			jcch_delete_segments = atoi (optarg);
			break;
		case 'f':				/* blind overwrites; Force */
			jcch_force = 1;
			break;
		case 'i':
			jcch_insert_segments = atoi (optarg);
			break;
		case 'q':				/* all prompts disabled */
			jcch_verbose = -1;
			break;
		case 's':				/* jcch_scale by Percentage of base rowcount */
		case 'P':				/* for backward compatibility */
			jcch_flt_scale = atof (optarg);
			if (jcch_flt_scale < MIN_SCALE)
			{
				int i;
				int int_scale;

				jcch_scale = 1;
				int_scale = (int)(1000 * jcch_flt_scale);
				for (i = JCCH_PART; i < JCCH_REGION; i++)
				{
					jcch_tdefs[i].base = (JCCH_DSS_HUGE)(int_scale * jcch_tdefs[i].base)/1000;
					if (jcch_tdefs[i].base < 1)
						jcch_tdefs[i].base = 1;
				}
			}
			else
				jcch_scale = (long) jcch_flt_scale;
			if (jcch_scale > MAX_SCALE)
			{
				fprintf (stderr, "%s %5.0f %s\n\t%s\n\n",
					"NOTE: Data generation for jcch_scale factors >",
					MAX_SCALE,
					"GB is still in development,",
					"and is not yet supported.\n");
				fprintf (stderr,
					"Your resulting data set MAY NOT BE COMPLIANT!\n");
			}
			break;
		case 'S':				/* generate a particular STEP */
			jcch_step = atoi (optarg);
			break;
		case 'U':				/* generate flat files for update stream */
			jcch_updates = atoi (optarg);
			break;
		case 'v':				/* life noises enabled */
			jcch_verbose = 1;
			break;
#ifdef JCCH_SKEW
		case 'k':				/* life noises enabled */
			JCCH_skew = 1;
			break;
#endif
		case 'T':				/* generate a specifc jcch_table */
			switch (*optarg)
			{
			case 'c':			/* generate customer ONLY */
				jcch_table = 1 << JCCH_CUST;
				bTableSet = 1;
				break;
			case 'L':			/* generate lineitems ONLY */
				jcch_table = 1 << JCCH_LINE;
				bTableSet = 1;
				break;
			case 'l':			/* generate code jcch_table ONLY */
				jcch_table = 1 << JCCH_NATION;
				jcch_table |= 1 << JCCH_REGION;
				bTableSet = 1;
				break;
			case 'n':			/* generate nation jcch_table ONLY */
				jcch_table = 1 << JCCH_NATION;
				bTableSet = 1;
				break;
			case 'O':			/* generate orders ONLY */
				jcch_table = 1 << JCCH_ORDER;
				bTableSet = 1;
				break;
			case 'o':			/* generate orders/lineitems ONLY */
				jcch_table = 1 << ORDER_LINE;
				bTableSet = 1;
				break;
			case 'P':			/* generate part ONLY */
				jcch_table = 1 << JCCH_PART;
				bTableSet = 1;
				break;
			case 'p':			/* generate part/partsupp ONLY */
				jcch_table = 1 << PART_PSUPP;
				bTableSet = 1;
				break;
			case 'r':			/* generate region jcch_table ONLY */
				jcch_table = 1 << JCCH_REGION;
				bTableSet = 1;
				break;
			case 'S':			/* generate partsupp ONLY */
				jcch_table = 1 << JCCH_PSUPP;
				bTableSet = 1;
				break;
			case 's':			/* generate suppliers ONLY */
				jcch_table = 1 << JCCH_SUPP;
				bTableSet = 1;
				break;
			default:
				fprintf (stderr, "Unknown jcch_table name %s\n",
					optarg);
				jcch_usage ();
				exit (1);
			}
			break;
		case 'O':				/* optional actions */
			switch (tolower (*optarg))
			{
			case 's':			/* calibrate the RNG jcch_usage */
				jcch_set_seeds = 1;
				break;
			default:
				fprintf (stderr, "Unknown option name %s\n",
					optarg);
				jcch_usage ();
				exit (1);
			}
			break;
		case 'h':				
		default:				/* something unexpected */
			version_copyright ();
			jcch_usage ();
			exit (1);
	}
 	part.s = (partsupp_t*) malloc(SUPP_PER_PART * sizeof(partsupp_t));
	if (part.s == NULL) { 
		fprintf(stderr, "ERROR Allocating memory for %lld suppliers.\n", SUPP_PER_PART);
		exit(-1);
	}

#ifdef JCCH_SKEW
	init_skew();
	part.s = (partsupp_t*) 
#endif
	(order.l = (line_t*) malloc(MAX_L_PER_O * sizeof(line_t)));
	if (order.l == NULL) { 
		fprintf(stderr, "ERROR Allocating memory for %lld orders.\n", MAX_L_PER_O);
		exit(-1);
	}
	return;
}

void jcch_validate_options(void)
{
	// DBGenOptions, 3.1
	if (jcch_children != 1)
	{
		if (jcch_updates != 0)
		{
			fprintf(stderr, "ERROR: -C is not valid when generating jcch_updates\n");
			exit(-1);
		}
		if (jcch_step == -1)
		{
			fprintf(stderr, "ERROR: -S must be specified when generating data in multiple chunks\n");
			exit(-1);
		}
	}

	// DBGenOptions, 3.3
	if (jcch_updates == 0)
	{
		if ((jcch_insert_segments != 0) || (jcch_delete_segments != 0))
		{
			fprintf(stderr, "ERROR: -d/-i are only valid when generating jcch_updates\n");
			exit(-1);
		}
	}

	// DBGenOptions, 3.9
	if (jcch_step != -1)
	{
		if ((jcch_children == 1) && (jcch_updates == 0))
		{
			fprintf(stderr, "ERROR: -S is only valid when generating data in multiple chunks or generating jcch_updates\n");
			exit(-1);
		}
	}

	// DBGenOptions, 3.10
	if (bTableSet && (jcch_updates != 0))
	{
		fprintf(stderr, "ERROR: -T not valid when generating jcch_updates\n");
		exit(-1);
	}

	return;
}


/*
* MAIN
*
* assumes the existance of getopt() to clean up the command 
* line handling
*/
int
jcch_dbgen_main (int ac, char **av)
{
	JCCH_DSS_HUGE i;
	
	jcch_table = (1 << JCCH_CUST) |
		(1 << JCCH_SUPP) |
		(1 << JCCH_NATION) |
		(1 << JCCH_REGION) |
		(1 << PART_PSUPP) |
		(1 << ORDER_LINE);
	jcch_force = 0;
    jcch_insert_segments=0;
    jcch_delete_segments=0;
    jcch_insert_orders_segment=0;
    jcch_insert_lineitem_segment=0;
    jcch_delete_segment=0;
	jcch_verbose = 0;
	jcch_set_seeds = 0;
	jcch_scale = 1;
	jcch_flt_scale = 1.0;
	jcch_updates = 0;
	jcch_step = -1;
	jcch_tdefs[JCCH_ORDER].base *=
		ORDERS_PER_CUST;			/* have to do this after init */
	jcch_tdefs[JCCH_LINE].base *=
		ORDERS_PER_CUST;			/* have to do this after init */
	jcch_tdefs[ORDER_LINE].base *=
		ORDERS_PER_CUST;			/* have to do this after init */
	jcch_children = 1;
	jcch_d_path = NULL;
	
#ifdef NO_SUPPORT
	signal (SIGINT, exit);
#endif /* NO_SUPPORT */
	jcch_process_options (ac, av);
	jcch_validate_options();
#if (defined(WIN32)&&!defined(_POSIX_))
	for (i = 0; i < ac; i++)
	{
		spawn_args[i] = malloc (((int)strlen (av[i]) + 1) * sizeof (char));
		MALLOC_CHECK (spawn_args[i]);
		strcpy (spawn_args[i], av[i]);
	}
	spawn_args[ac] = NULL;
#endif
	
	if (jcch_verbose >= 0) {
			version_copyright ();
	}
	
	jcch_load_dists ();
#ifdef RNG_TEST
	for (i=0; i <= MAX_STREAM; i++)
		jcch_Seed[i].nCalls = 0;
#endif
#ifdef JCCH_SKEW
	if (JCCH_skew) { 
		printf("=== GENERATING SKEWED DATA ===\n");
	}
#endif
	/* have to do this after init */
	jcch_tdefs[JCCH_NATION].base = jcch_nations.count;
	jcch_tdefs[JCCH_REGION].base = jcch_regions.count;
	
	/* 
	* jcch_updates are never parallelized 
	*/
	if (jcch_updates)
		{
		/* 
		 * set RNG to start generating rows beyond SF=jcch_scale
		 */
		jcch_set_state (JCCH_ORDER, jcch_scale, 100, 101, &i); 
		jcch_rowcnt = (int)(jcch_tdefs[ORDER_LINE].base / 10000 * jcch_scale * UPD_PCT);
		if (jcch_step > 0)
			{
			/* 
			 * adjust RNG for any prior update generation
			 */
	      for (i=1; i < jcch_step; i++)
         {
			jcch_sd_order(0, jcch_rowcnt);
			jcch_sd_line(0, jcch_rowcnt);
         }
			jcch_upd_num = jcch_step - 1;
			}
		else
			jcch_upd_num = 0;

		while (jcch_upd_num < jcch_updates)
			{
			if (jcch_verbose > 0)
				fprintf (stderr,
				"Generating update pair #%d for %s",
				jcch_upd_num + 1, jcch_tdefs[ORDER_LINE].comment);
			jcch_insert_orders_segment=0;
			jcch_insert_lineitem_segment=0;
			jcch_delete_segment=0;
			jcch_minrow = jcch_upd_num * jcch_rowcnt + 1;
			jcch_gen_tbl (ORDER_LINE, jcch_minrow, jcch_rowcnt, jcch_upd_num + 1);
			if (jcch_verbose > 0)
				fprintf (stderr, "done.\n");
			pr_drange (ORDER_LINE, jcch_minrow, jcch_rowcnt, jcch_upd_num + 1);
			jcch_upd_num++;
			}

		exit (0);
		}
	
	/**
	** actual data generation section starts here
	**/

	/*
	* traverse the tables, invoking the appropriate data generation routine for any to be built
	*/
	for (i = JCCH_PART; i <= JCCH_REGION; i++)
		if (jcch_table & (1 << i))
		{
			if (jcch_children > 1 && i < JCCH_NATION)
			{
				jcch_partial ((int)i, jcch_step);
			}
			else
			{
				jcch_minrow = 1;
				if (i < JCCH_NATION)
					jcch_rowcnt = jcch_tdefs[i].base * jcch_scale;
				else
					jcch_rowcnt = jcch_tdefs[i].base;
				if (jcch_verbose > 0)
					fprintf (stderr, "Generating data for %s", jcch_tdefs[i].comment);
				jcch_gen_tbl ((int)i, jcch_minrow, jcch_rowcnt, jcch_upd_num);
				if (jcch_verbose > 0)
					fprintf (stderr, "done.\n");
			}
		}
			
		return (0);
}

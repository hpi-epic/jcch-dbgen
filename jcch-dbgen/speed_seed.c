/*
* $Id: speed_seed.c,v 1.9 2009/06/28 14:01:08 jms Exp $
*
* Revision History
* ===================
* $Log: speed_seed.c,v $
* Revision 1.9  2009/06/28 14:01:08  jms
* bug fix for DOP
*
* Revision 1.8  2006/08/01 04:13:17  jms
* fix parallel generation
*
* Revision 1.7  2006/07/31 17:23:09  jms
* fix to parallelism problem
*
* Revision 1.6  2006/05/16 16:26:51  jms
* remove calls to FAKE_V_STR
*
* Revision 1.5  2006/04/26 23:14:28  jms
* Declaraion cleanup of fakeVStr()
*
* Revision 1.4  2006/04/26 23:01:10  jms
* address update generation problems
*
* Revision 1.3  2005/10/25 17:26:38  jms
* check in integration between microsoft changes and baseline code
*
* Revision 1.2  2005/01/03 20:08:59  jms
* change line terminations
*
* Revision 1.1.1.1  2004/11/24 23:31:47  jms
* re-establish external server
*
* Revision 1.2  2004/01/22 03:54:12  jms
* 64 bit support changes for customer address
*
* Revision 1.1.1.1  2003/08/08 22:37:36  jms
* recreation after CVS crash
*
* Revision 1.3  2003/08/08 22:37:36  jms
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
#include <stdio.h>
#include <stdlib.h>
#include "dss.h"
#include "rng64.h"
#include "dss.h"

/*  _tal long RandSeed = "Random^SeedFromTimestamp" (void); */

#define ADVANCE_STREAM(stream_id, num_calls) \
	jcch_advanceStream(stream_id, num_calls, 0)
#define ADVANCE_STREAM64(stream_id, num_calls) \
	jcch_advanceStream(stream_id, num_calls, 1)
#define MAX_COLOR 92
long jcch_name_bits[MAX_COLOR / BITS_PER_LONG];
extern seed_t jcch_Seed[];
void fakeVStr(int nAvg, long nSeed, JCCH_DSS_HUGE nCount);
void jcch_NthElement (JCCH_DSS_HUGE N, JCCH_DSS_HUGE *StartSeed);


void 
jcch_advanceStream(int nStream, JCCH_DSS_HUGE nCalls, int bUse64Bit)
{
   if (bUse64Bit)
      jcch_Seed[nStream].value = AdvanceRand64(jcch_Seed[nStream].value, nCalls);
   else
      jcch_NthElement(nCalls, &jcch_Seed[nStream].value);

#ifdef RNG_TEST
   jcch_Seed[nStream].nCalls += nCalls;
#endif

	return;
}

/* WARNING!  This routine assumes the existence of 64-bit                 */
/* integers.  The notation used here- "HUGE" is *not* ANSI standard. */
/* Hopefully, you have this extension as well.  If not, use whatever      */
/* nonstandard trick you need to in order to get 64 bit integers.         */
/* The book says that this will work if MAXINT for the type you choose    */
/* is at least 2**46  - 1, so 64 bits is more than you *really* need      */

static JCCH_DSS_HUGE Multiplier = 16807;      /* or whatever nonstandard */
static JCCH_DSS_HUGE Modulus =  2147483647;   /* trick you use to get 64 bit int */

/* Advances value of jcch_Seed after N applications of the random number generator
   with multiplier Mult and given Modulus.
   jcch_NthElement(jcch_Seed[],count);

   Theory:  We are using a generator of the form
        X_n = [Mult * X_(n-1)]  mod Modulus.    It turns out that
        X_n = [(Mult ** n) X_0] mod Modulus.
   This can be computed using a divide-and-conquer technique, see
   the code below.

   In words, this means that if you want the value of the jcch_Seed after n
   applications of the generator,  you multiply the initial value of the
   jcch_Seed by the "super multiplier" which is the basic multiplier raised
   to the nth power, and then take mod Modulus.
*/

/* Nth Element of sequence starting with StartSeed */
void jcch_NthElement (JCCH_DSS_HUGE N, JCCH_DSS_HUGE *StartSeed)
   {
   JCCH_DSS_HUGE Z;
   JCCH_DSS_HUGE Mult;
   static int ln=-1;
   int i;

   if ((jcch_verbose > 0) && ++ln % 1000 == 0)
       {
       i = ln % LN_CNT;
       fprintf(stderr, "%c\b", lnoise[i]);
       }
   Mult = Multiplier;
   Z = (JCCH_DSS_HUGE) *StartSeed;
   while (N > 0 )
      {
      if (N % 2 != 0)    /* testing for oddness, this seems portable */
         Z = (Mult * Z) % Modulus;
      N = N / 2;         /* integer division, truncates */
      Mult = (Mult * Mult) % Modulus;
      }
   *StartSeed = Z;

   return;
   }


/* updates jcch_Seed[column] using the jcch_a_rnd algorithm */
void
jcch_fake_a_rnd(int min, int max, int column)
{
   JCCH_DSS_HUGE len;
   JCCH_DSS_HUGE itcount;

   RANDOM(len, min, max, column);
   if (len % 5L == 0)
      itcount = len/5;
   else 
	   itcount = len/5 + 1L;
   jcch_NthElement(itcount, &jcch_Seed[column].jcch_usage);
#ifdef RNG_TEST
	jcch_Seed[column].nCalls += itcount;
#endif
   return;
}


long 
jcch_sd_part(int child, JCCH_DSS_HUGE skip_count)
{
   int i;
 
   for (i=P_MFG_SD; i<= P_CNTR_SD; i++)
       ADVANCE_STREAM(i, skip_count);
 
   ADVANCE_STREAM(P_CMNT_SD, skip_count * 2);
   ADVANCE_STREAM(P_NAME_SD, skip_count * 92);

   return(0L);
}

long 
jcch_sd_line(int child, JCCH_DSS_HUGE skip_count)
	{
	int i,j;
	
	for (j=0; j < O_LCNT_MAX; j++)
	{
		for (i=L_QTY_SD; i<= L_RFLG_SD; i++)
/*
			if (jcch_scale >= 30000 && i == L_PKEY_SD)
				ADVANCE_STREAM64(i, skip_count);
			else
*/
				ADVANCE_STREAM(i, skip_count);
		ADVANCE_STREAM(L_CMNT_SD, skip_count * 2);
	}
	
	/* need to special case this as the link between master and detail */
	if (child == 1)
	{
		ADVANCE_STREAM(O_ODATE_SD, skip_count);
		ADVANCE_STREAM(O_LCNT_SD, skip_count);
	}
	
	return(0L);
	}

long 
jcch_sd_order(int child, JCCH_DSS_HUGE skip_count)        
{
	ADVANCE_STREAM(O_LCNT_SD, skip_count);
/*
	if (jcch_scale >= 30000)
		ADVANCE_STREAM64(O_CKEY_SD, skip_count);
	else
*/
		ADVANCE_STREAM(O_CKEY_SD, skip_count);
	ADVANCE_STREAM(O_CMNT_SD, skip_count * 2);
	ADVANCE_STREAM(O_SUPP_SD, skip_count);
	ADVANCE_STREAM(O_CLRK_SD, skip_count);
	ADVANCE_STREAM(O_PRIO_SD, skip_count);
	ADVANCE_STREAM(O_ODATE_SD, skip_count);

	return (0L);
}

long
jcch_sd_psupp(int child, JCCH_DSS_HUGE skip_count)
	{
	int j;
	
	for (j=0; j < SUPP_PER_PART; j++)
		{
		ADVANCE_STREAM(PS_QTY_SD, skip_count);
		ADVANCE_STREAM(PS_SCST_SD, skip_count);
		ADVANCE_STREAM(PS_CMNT_SD, skip_count * 2);
		}

	return(0L);
	}

long 
jcch_sd_cust(int child, JCCH_DSS_HUGE skip_count)
{
   
   ADVANCE_STREAM(C_ADDR_SD, skip_count * 9);
   ADVANCE_STREAM(C_CMNT_SD, skip_count * 2);
   ADVANCE_STREAM(C_NTRG_SD, skip_count);
   ADVANCE_STREAM(C_PHNE_SD, 3L * skip_count);
   ADVANCE_STREAM(C_ABAL_SD, skip_count);
   ADVANCE_STREAM(C_MSEG_SD, skip_count);
   return(0L);
}

long
jcch_sd_supp(int child, JCCH_DSS_HUGE skip_count)
{
   ADVANCE_STREAM(S_NTRG_SD, skip_count);
   ADVANCE_STREAM(S_PHNE_SD, 3L * skip_count);
   ADVANCE_STREAM(S_ABAL_SD, skip_count);
   ADVANCE_STREAM(S_ADDR_SD, skip_count * 9);
   ADVANCE_STREAM(S_CMNT_SD, skip_count * 2);
   ADVANCE_STREAM(BBB_CMNT_SD, skip_count);
   ADVANCE_STREAM(BBB_JNK_SD, skip_count);
   ADVANCE_STREAM(BBB_OFFSET_SD, skip_count);
   ADVANCE_STREAM(BBB_TYPE_SD, skip_count);      /* avoid one trudge */
   
   return(0L);
}

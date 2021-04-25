/*
* $Id: rng64.h,v 1.2 2005/01/03 20:08:59 jms Exp $
*
* Revision History
* ===================
* $Log: rng64.h,v $
* Revision 1.2  2005/01/03 20:08:59  jms
* change line terminations
*
* Revision 1.1.1.1  2004/11/24 23:31:47  jms
* re-establish external server
*
* Revision 1.1.1.1  2003/08/08 21:57:34  jms
* recreation after CVS crash
*
* Revision 1.1  2003/08/08 21:57:34  jms
* first integration of rng64 for o_custkey and l_partkey
*
*
*/
JCCH_DSS_HUGE AdvanceRand64( JCCH_DSS_HUGE nSeed, JCCH_DSS_HUGE nCount);
void dss_random64(JCCH_DSS_HUGE *tgt, JCCH_DSS_HUGE nLow, JCCH_DSS_HUGE nHigh, long stream);
JCCH_DSS_HUGE NextRand64(JCCH_DSS_HUGE nSeed);

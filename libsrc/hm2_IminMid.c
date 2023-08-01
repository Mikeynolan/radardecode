/*      %W      %G      */
/*
modification history
--------------------
x.xx 25jun95,pjp .. included documentation
*/
#include        <convLib.h>
/********************************************************************************
* hm2_IminMid - convert hours, minutes to minutes from midnite.
*
* DESCRIPTION
*
* Convert hours and minutes to minutes from midnite. The return value will be
* modulo 24 hours.
*
* RETURNS
* The minutes from midnite modulo 1 day.
*
* SEE ALSO
* IminMid_hm2, convLib.h
*/

int hm2_IminMid
	(
	 int hh,	/* hours*/
	 int mm		/* minutesin hour*/
	)
{
/*
 *      convert minutes from midnite to ihour imin. wrap at 24 hours
*/
	return((hh * 60 + mm)%1440);
}

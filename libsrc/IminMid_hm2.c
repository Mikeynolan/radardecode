/*	%W	%G	*/
/*
modification history
--------------------
x.xx 25jun95,pjp .. included documentation
*/
#include	<convLib.h>
/*******************************************************************************
* IminMid_hm2 - convert mintues from midnite to hours,minutes.
*
* DESCRIPTION
*
* Convert minutes from midnite to hours, minutes. The data is returned modulo
* 24 hours. The hours and minutes from midnite are returned in phour, and pmin 
* respectively.
*
* RETURNS
* The funtion has no return value.
*
* SEE ALSO
* convLib.h
*/

void IminMid_hm2
	(
	 int min,		/* minutes from midnite*/
	 int *phour,		/* return hours from midnite*/
	 int *pmin		/* return minutes in hour here*/
	)
{
/*
 *      convert minutes from midnite to ihour imin. wrap at 24 hours
*/
	min=min%1440;
	*phour=min/60;
	*pmin=min%60;
        return;
}

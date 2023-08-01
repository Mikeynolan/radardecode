/*      %W      %G      */
/*
modification history
--------------------
x.xx 25jun95,pjp .. included documentation
*/
#include	<convLib.h>
/********************************************************************************
* ymd1_ymd3 - convert from packed year,month,day to year,mon,day.
*
* DESCRIPTION
*
* Convert from packed year,month,day (yymmdd) to year, month,day.
*
* RETURNS
* The function returns nothing.
*
* SEE ALSO
* dms3_rad, setSgn, convLib.h
*/

void ymd1_ymd3
	(
	  int yymmdd,	/* packed input*/
	  int *pyy,	/* return year*/
	  int *pmm,	/* return month*/
	  int *pdd	/* return day*/
	)
{
/*
 *	convert yymmdd to iyear imonth iday 
*/
        *pyy=yymmdd/10000;
        *pdd=yymmdd%100;
        *pmm=(yymmdd/100)%100;
        return;
}


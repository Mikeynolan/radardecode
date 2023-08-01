/*      %W      %G      */
/*
modification history
--------------------
x.xx 25jun95,pjp .. included documentation
*/
#include	<convLib.h>
/*******************************************************************************
* dmy1_dmy3 - convert packed  ddmmyy to iday, imonth, iyear.
*
* DESCRIPTION
*
* Convert packed day, month, year (ddmmyy)  to day, month, year. 
* The inputs and  outputs are all integer., minutes, seconds.
*
* RETURNS
* The funtion has no return value.
*
* SEE ALSO
* ymd1_ymd3, convLib.h
*/

void dmy1_dmy3
	(
	 int ddmmyy,	/* packed input day,month,year*/
	 int *pdd,	/* return day*/
	 int *pmm,	/* return month*/
	 int *pyy	/* return year*/
	)
{
/*
 *	convert ddmmyy to iday imonth iyear
*/
        *pdd=ddmmyy/10000;
        *pyy=ddmmyy%100;
        *pmm=(ddmmyy/100)%100;
        return;
}

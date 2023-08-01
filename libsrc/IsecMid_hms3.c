/*      %W      %G      */
/*
modification history
--------------------
x.xx 25jun95,pjp .. included documentation
*/
#include        <convLib.h>
/*******************************************************************************
* IsecMid_hms3 - convert secs from midnite to hours,mins,secs (int).
*
*DESCRIPTION
*
*Convert seconds from midnite to hours, minutes, seconds. The value is returned
*modulo 1 day (hours,minutes,seconds will always be values within 1 day).
*The hours and minutes from midnite are returned in hh, mm, and ss. This 
*routine uses integer inputs and outputs.
*
*RETURNS
*The funtion has no return value.
*
*SEE ALSO
*convLib.h
*/

void IsecMid_hms3
	(
	  int isecs,		/* seconds from midnite input*/
	  int *hh,		/* return hours here */
	  int *mm,		/* return minutes here*/
	  int *iss		/* return seconds here*/
	)
{
/*
 *      convert secs from midnite to hh mm ss
*/
        isecs=isecs%86400;                      /* within 1 day*/
        *hh=isecs/3600;
        *mm= (isecs -((*hh) * 3600))/60;
        *iss=isecs%60;
        return;
}

/*      %W      %G      */
/*
modification history
--------------------
x.xx 25jun95,pjp .. included documentation
*/
#include        <convLib.h>
/*******************************************************************************
* hms3_IsecMid - change from hour min sec(int) to seconds from midnite.
*
* Convert from hours(int), minutes(int), seconds(int) to 
* seconds from midnite.
*
* RETURNS
* Seconds from midnite modulo 1 day.
*
* SEE ALSO
* hms3ToFrac, hms3_rad, rad_hms3, convLib.
*/


int  hms3_IsecMid
	(
	 int hh,	/* hours*/
	 int mm,	/* minutes*/
	 int iss	/* seconds*/
	)
{
/*
 *      convert hours/min/secs to minutes from midnite modulo 1 day
*/
        return((hh*3600+mm*60+iss)%86400);
}

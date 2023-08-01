/*      %W      %G      */
/*
modification history
--------------------
x.xx 25jun95,pjp .. included documentation
*/
#include        <convLib.h>
/*******************************************************************************
* hms3ToFrac  -  change from hour minute sec(double) to a fraction of a day.
*
* Compute the current fraction of a day from the input hour, min , second.
*
* RETURNS
* Fraction of a day for the input time as a double;
* 
* SEE ALSO
* hms3_IsecMid, hms3_rad, rad_hms3, convLib.
*/

double	hms3ToFrac
	(
	 int 	h,		/* hour as a 0-23 */
	 int	m,		/* minute of time*/
	 double s 		/* second of minute*/
	)
{
	return(((h*3600 + m*60 + s)/86400.));
}

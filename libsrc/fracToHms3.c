/*      %W      %G      */
/*
modification history
--------------------
x.xx 25jun95,pjp .. included documentation
*/
#include        <convLib.h>
/*******************************************************************************
* fracToHms3 - change fraction of a day to hms
*
* Convert from fraction of a dat to hours, minutes, seconds.
*/

void fracToHms3
	(
	 double	frac,	/* of a day*/
	 int 	*ph,		/* hour as a 0-23 */
	 int	*pm,		/* minute of time*/
	 double *ps 		/* second of minute*/
	)
{
	secMid_hms3(frac*86400.,ph,pm,ps);
	return;
}

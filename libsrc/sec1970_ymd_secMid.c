/*      %W      %G      */
/*
modification history
--------------------
*/
#include 	<time.h>
#include    <math.h>
#include	<convLib.h>
/******************************************************************************
* sec1970_yyyymmdd - convert secs1970 to yyyymmdd
*
* DESCRIPTION
*
* Convert from unixtime (sec 1970) to yyyymmdd., seconds from midnite.
*The returned time is localtime.
*
* RETURNS
*
* SEE ALSO
*/

void sec1970_ymd_secMid(
	double sec1970,		// to convert
    int *pyyyymmdd,      // return yyyymmdd here,
    double *psecMid     // return seconds midnite here)
	)
{
	time_t  itm1970;
	double  fracSec,dtemp;
    struct  tm       tU;

	fracSec=modf(sec1970,&dtemp);
	itm1970=(long)(dtemp);
	localtime_r(&itm1970,&tU);
    *pyyyymmdd=(tU.tm_year + 1900)*10000 + (tU.tm_mon+1)*100 + tU.tm_mday;
	*psecMid=fracSec + (tU.tm_sec + 60*(tU.tm_min + 60*(tU.tm_hour)));
	return;
}

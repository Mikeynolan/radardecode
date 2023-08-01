/*
modification history
--------------------
x.xx 25jun95,pjp .. included documentation
*/
#include	<mathCon.h>
#include	<convLib.h>
/*******************************************************************************
* dms1_rad - convert ddmmss.sss as a double to radians.
*
* DESCRIPTION
*
* Convert packed degrees, minutes, seconds to radians.
* The input is a single double with ddmmss.ss with dd degrees, mm minutes,
* ss.s seconds. 
*
* RETURNS
* The corresponding value in radians (can be positive or negative).
*
* SEE ALSO
* dms1_dms3,convLib.h
*/

double dms1_rad
	(
	double	dms	/* packed ddmmss.s degrees,minutes,seconds*/
	)			       
{
/*      	convert from ddmmss.ssss... to radians.
*/
	int     d,m;
	double  s;
	int     sign;

/* 
 * 	split into d m s (int,int,double). sign holds sign bit
*/
	dms1_dms3(dms,&sign,&d,&m,&s);
        return((double)sign * C_2PI*(((double)d)/360. +
                     ((double)m)/21600. +   s/1296000.));
}

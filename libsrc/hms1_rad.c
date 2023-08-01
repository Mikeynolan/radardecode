/*      %W      %G      */
/*
modification history
--------------------
x.xx 25jun95,pjp .. included documentation
*/
#include	<mathCon.h>
#include	<convLib.h>
/*******************************************************************************
* hms1_rad - convert hhmmss.sss as a double to radians.
*
* DESCRIPTION
*
* Convert packed hours, minutes, seconds to radians.
* The input is a single double with hhmmss.ss with hh hours, mm minutes,
* ss.s seconds.
*
* RETURNS
* The corresponding value in radians (can be positive or negative).
*
* SEE ALSO
* hms1_hms3,convLib.h
*
* NOTE
* The returned value is modulo 1 day (2pi) since hms1_hms3 is used.
*/

double hms1_rad
	(
	 double	hms	/* input hhmmss.ssss*/
	)			       
{
/*      	convert from hhmmss.sss.. radians.
*/
	int     ih,im;
	double  s;
/* 
 * 	split into h m s (int,int,double). sign holds sign bit
*/
	int	sgn;
	hms1_hms3(hms,&sgn,&ih,&im,&s);
        return(C_2PI*sgn*(((double)ih)/24.  +
                     ((double)im)/1440. +   s/86400.));
}

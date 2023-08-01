/*      %W      %G      */
/*
modification history
--------------------
x.xx 25jun95,pjp .. included documentation
*/
#include	<convLib.h>
/*******************************************************************************
* hms1_hms3 - convert hhmmss.sss as a double to ihours,imin, dsec.
*
* DESCRIPTION
*
* Convert packed hours, minutes, seconds to hours, minutes, seconds.
* The input is a single double with hhmmss.ss with hh hours, mm minutes,
* ss.s seconds. On output the hours and minutes are integer while the
* seconds are double. The returned values are always positive and sgn is
* returned with the correct sign (+/- 1) for the input data.
*
* RETURNS
* The funtion has no return value.
*
* SEE ALSO
* convLib.h
*
* NOTE
* The values are returned modulo 1 day.
*/

void   hms1_hms3
	(
	 double	hms,	/* input value	hhmmss.s*/		       
	 int     *sgn,	/* +/-1 depending on sign of hms */
	 int     *ih, 	/* return hour */
	 int	*im,	/* return min */
	 double  *s 	/* return sec */
	)
{
/*      	convert from hms.sss to h m s and sign
*/
	int     itemp;

	if (hms < 0.) {
	    hms*=-1;
	    *sgn=-1;
	}
	else {
	   *sgn=1;
	}
/*
 *      split into h m s (int,int,double). 
*/
        itemp=(int)(hms);
        *s= (hms - itemp) + (itemp%100);        /* seconds*/
        *ih=itemp/10000;                        /* degrees*/
        *im=(itemp/100)%100;
        if (*s>=60.0){
          *s-=60.;
          *im+=1.;
        }
        if (*im>=60){
           *im -=60;
           *ih +=1;
        }
	*ih %= 24;
        return;
}

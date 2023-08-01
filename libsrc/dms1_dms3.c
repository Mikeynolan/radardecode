/*      %W      %G      */
/*
modification history
--------------------
x.xx 25jun95,pjp .. included documentation
*/
#include	<convLib.h>
/*******************************************************************************
* dms1_dms3 - convert ddmmss.sss as a double to ideg,imin, dsec.
*
* DESCRIPTION
*
* Convert packed degrees, minutes, seconds to degrees, minutes, seconds.
* The input is a single double with ddmmss.ss with dd degrees, mm minutes,
* ss.s seconds. On output  the degrees and minutes are integer while the
* seconds are double. The returned values are always positive and isgn is
* returned with the correct sign (+/- 1) for the input data.
*
* RETURNS
* The funtion has no return value.
*
* SEE ALSO
* dms1_rad, convLib.h
*/
void   dms1_dms3(
	double	dms,	/* input data*/			       
	int	*isgn,	/* returned sign of dms */
	int     *id,	/* return degrees.*/
	int	*im,	/* return minutes*/
	double  *s)	/* return seconds*/
{
/*      	
 *	convert from dddmmss.ssss to id im ds
 *	return sign in isgn, id,im,s will then all be positive
 *	be a little careful with double to int since 68040 does it poorly...
*/
	int itemp;

	if (dms < 0.) {
	   dms*=-1.;
	   *isgn=-1;
	}
	else {
	   *isgn=1;
	}
/* 
 * 	split into d m s (int,int,double). sign holds sign bit
*/
	itemp=(int)(dms);
	*s= (dms - itemp) + (itemp%100);	/* seconds*/
	*id=itemp/10000;			/* degrees*/
	*im=(itemp/100)%100;
	if (*s>=60.0){
	  *s-=60.;
	  *im+=1.;
	}
	if (*im>=60){
	   *im -=60;
	   *id +=1;
	}
	return;
}

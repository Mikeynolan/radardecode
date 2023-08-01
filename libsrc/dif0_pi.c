/*      %W      %G      */
/*
modification history
--------------------
x.xx 25jun95,pjp .. included documentation
*/
#include	<convLib.h>
#include	<math.h>
#include	<mathCon.h>
/*******************************************************************************
* dif0_pi - compute the difference of 2 angles modulo PI radians
*
* DESCRIPTION
*
* Compute the difference a-b in radians. Return the value
* to add to b that gives a. this value will have an absolute value <= C_PI.
*
* RETURNS
* The difference angle
*
* NOTES
* This routine uses fmod.
*/
double  dif0_pi
        (
          double        aRd,           /* angle 1 radians */
          double        bRd            /* angle 2 radians */
        )
{
	double dif;

	dif = fmod(aRd - bRd,C_2PI);
	if (dif > C_PI) {
	    return(dif - C_2PI);
	}
	else if (dif <= -C_PI) {
	    return(dif + C_2PI);
	}
	else{
	    return(dif);
	}
}

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
* to0_2pi - return the corresponding angle from 0 to 2pi
*
* Convert the input angle (radians) to lie from 0 to 2pi.
*
* RETURNS
* The corresponding angle between 0 and 2 pi in radians
*
* NOTE 
* This routine uses fmod.
*/
double  to0_2pi
        (
          double        angle          /* input angle in radians. */
        )
{
	double a;
	a=fmod(angle,C_2PI);
	return( (a<0)?a+C_2PI:a);
}

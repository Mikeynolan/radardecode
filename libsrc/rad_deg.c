/*      %W      %G      */
/*
modification history
--------------------
x.xx 25jun95,pjp .. included documentation
*/
#include	<mathCon.h>
#include	<convLib.h>
/********************************************************************************
* rad_deg - convert from radians to degrees (double).
*
* DESCRIPTION
*
* Convert from radians to degrees.
*
* RETURNS
* The value in degress.
*
* SEE ALSO
* convLib.h
*/

double rad_deg
	(
	 double	rad	/* radians*/
	)			       
{
/*      	convert radians to degrees.
*/
        return(rad*C_RAD_TO_DEG);
}

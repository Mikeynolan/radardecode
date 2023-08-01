/*      %W      %G      */
/*
modification history
--------------------
x.xx 25jun95,pjp .. included documentation
*/
#include        <convLib.h>
/*******************************************************************************
* hms1_IsecMid - change from hhmmss (int) to seconds from midnite.
*
* Convert from hhmmss integer one word to seconds from midnight, modulo 1 day.
*
* RETURNS
* Seconds from midnite modulo 1 day.
*
*/


int  hms1_IsecMid
	(
	 int	hhmmss		/* input by user*/
	)
{
	int	hh,mm,ss;
	hh  =hhmmss/10000;
	mm  =(hhmmss/100) % 100;
	ss  =hhmmss%100;
    return((hh*3600+mm*60+ss)%86400);
}

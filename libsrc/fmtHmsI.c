/*      %W      %G      */
/*
modification history
--------------------
x.xx 25jun95,pjp .. included documentation
*/
#include        <convLib.h>
#include	<stdio.h>
/*******************************************************************************
* fmtHmsI - format hours minutes seconds(int) for output.
*
* DESCRIPTION
* Write hours, minutes, seconds as a formatted string into a character array.
* Place leading zeros in each field, separate fields by ":".
* The string is  returned as :
*
*   [-/ ]hh:mm:ss
*
* Negative numbers have a leading "-" while non negative numbers have a leading
* space.
*
* cformatted string must point to a buffer that has already been allocated and
* can hold at all the requested characters plus a NULL terminator.
*
* The input hours, min, sec should be for a non-negative angle with
* sgn set to +/- 1 depending on whether the initial angle was >=0 or < 0.
*
* RETURN
* The formated string is returned via the pointer cformatted.
*
* SEE ALSO
* fmtHmsD, setSgn, convLib
*/
void    fmtHmsI
        (
	 int    sgn,           /* +/-1 depending on angle >=0,< 0*/
         int    hour,           /* hour*/
         int    min,            /* minutes*/
         int    sec,            /* seconds*/
         char   *cformatted     /* return formatted string here*/
        )
{
        char c;
        c=(sgn<0)?'-':' ';
        sprintf(cformatted,"%c%02d:%02d:%02d",c,hour,min,sec);
        return;
}

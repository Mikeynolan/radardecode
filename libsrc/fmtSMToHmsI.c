/*      %W      %G      */
/*
modification history
--------------------
x.xx 25jun95,pjp .. included documentation
*/
#include        <convLib.h>
#include	<stdio.h>
/*******************************************************************************
* fmtSMToHmsI - format seconds from midnite to hms(int) for output.
*
* DESCRIPTION
* Convert seconds from midnite to hours,minutes, seconds (integer) and then
* write hours, minutes, seconds as a formatted string into a character array.
* Place leading zeros in each field, separate fields by ":".
* The string is  returned as :
*
*   hh:mm:ss
*
* cformatted string must point to a buffer that has already been allocated and
* can hold at all the requested characters plus a NULL terminator.
*
* RETURN
* The formated string is returned via the pointer cformatted.
*
* SEE ALSO
* fmtHmsD,  convLib
*/
void    fmtSMToHmsI
        (
	 int	secMidnite,	/* seconds from midnite*/
         char   *cformatted     /* return formatted string here*/
        )
{
	int	hour,min,sec;

	IsecMid_hms3(secMidnite,&hour,&min,&sec);
        sprintf(cformatted,"%02d:%02d:%02d",hour,min,sec);
        return;
}

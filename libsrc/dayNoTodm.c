/*      %W      %G      */
/*
modification history
--------------------
x.xx 25jun95,pjp .. included documentation
*/
#include	<convLib.h>
/*******************************************************************************
* dayNoTodm -  convert from day number of year to day and month of year.
*
* DESCRIPTION
*
* Pass in the day number of the year and the gregorian year. Return the day and
* month of the year. Jan 1 is dayno 1. The year (which should be 4 digits)
* is needed to determine if the current year is a leap year or not.
*
* RETURNS
* The day and month as integers.
*
* WARNING
* Leap year determination uses the gregorian convention (as opposed to the 
* julian). It will not work for years prior to 1582.
* The day number is forced to be within the day number range of the current 
* year.
*/
void    dayNoTodm 
        (
        int     dayNo,          /* day number of year */
        int     year,           /* gregorian year (should be > 1582) */
        int*    pd,	        /* return day of month */
        int*    pm 	        /* return month */
        )
{
/*
 *      static array holds  days up until start of this month. Double 0 at
 *      start since months start at 1 and c array starts at 0.
 *      1st set is for non leapyeard, second set is for leap years.
*/
        static int dayNoDat[26]=
                {0,31,59,90,120,151,181,212,243,273,304,334,365,
                 0,31,60,91,121,152,182,213,244,274,305,335,366};

	int  daysInYear,mon;
	int* pdat;

	if (isLeapYear(year)){
	   daysInYear=366;
	   pdat= dayNoDat + 13;	 	/* pnt at 2nd half of array*/
	}
	else {
	   daysInYear=365;
	   pdat= dayNoDat;	 	/* pnt at 1st half of array*/
	}
	if (dayNo < 1) dayNo=1;
	if (dayNo > daysInYear) dayNo=daysInYear;
	/*
	*  now loop looking for month. we are sure dayNo is 1 -> daysinyear
 	*/
	for (mon=1;mon<=12;mon++){
	   if (dayNo <= pdat[mon]){		/*this month ? */
	     *pm=mon;				/*store it */
	     *pd=dayNo - pdat[mon - 1];		/*subtract off previous months*/
	      break;
	   }
	}
	return;
}

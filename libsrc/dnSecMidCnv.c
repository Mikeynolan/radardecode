/*      %W      %G      */
/*
modification history
--------------------
x.xx 25jun95,pjp .. included documentation
*/
#include	<convLib.h>
/*******************************************************************************
* dnSecMidCnv - convert daynumber,sec midnite to mon,day,h,m,s.
*
*DESCRIPTION
*Convert dayNumber to month,day and secsFromMidnite to hh,mm,ss.
*
*RETURNS
*mon,day,hour,min,sec.
*/
void    dnSecMidCnv
        (
         int    year,           /* 4 digits*/
         int    dayNo,          /* daynumber*/
         int    secMidnite,     /* seconds from midnite*/
         int    *pmon,          /*month 1..12*/
         int    *pday,          /*day of month 1..31*/
         int    *phour,         /*hour of day*/
         int    *pmin,          /*minute of hour*/
         int    *psec           /*sec of min*/
        )
{
        dayNoTodm(dayNo,year,pday,pmon);
        IsecMid_hms3(secMidnite,phour,pmin,psec);
        return;
}

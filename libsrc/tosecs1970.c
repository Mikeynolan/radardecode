#include 	<time.h>
#include    <math.h>
#include	<convLib.h>
/******************************************************************************
* tosecs1970 - convert yymmdd,hhmmss.s to secs1970 
*
* DESCRIPTION
*
* Convert from yymmdd,hhmmss.s to unixtime (sec 1970).
*The input is local time. 
*If yymmdd is 0, then routine assumes yymmdd is 
*closest 12 hour time to current time,day
*The returned time is secs1970 (utc based)
*
* RETURNS
*
* SEE ALSO
*/

double tosecs1970(
	int    yymmdd,      // year,mon,day .year can be 2 or 4 dig
    double hhmmss      // time of day
	)
{
	int       ihr,imin,isec,sgn;
	int       iyr,imon,iday;
    double    dsec;
    time_t    tmFut,tmNow;
    struct tm tmStr;
    int       halfDaySecs;
	double    dtemp,fracSec;

//   split up  hhmmss.s
	halfDaySecs=86400/2;
    hms1_hms3(hhmmss*1.,&sgn,&ihr,&imin,&dsec);
    fracSec=modf(dsec,&dtemp);
    isec=(int)(dsec);
//  if yymmdd not specfied.. used current time for day,mon,etc
	if (yymmdd == 0) {
	 	//  assume the hhmmss is the same day..
    	tmNow=time(NULL);
	    localtime_r(&tmNow,&tmStr);
   		tmStr.tm_sec=isec;
   		tmStr.tm_min=imin;
   		tmStr.tm_hour=ihr;
//      now get secs 1970 for this time
    	tmFut=mktime(&tmStr);
//      if fut time > 12 hours remove 1 day
   		if ((tmFut-tmNow) >= halfDaySecs){
        	tmFut-=86400;
    	} else if ((tmFut - tmNow) < -halfDaySecs) {
        	tmFut+=86400;
    	}
	} else {
	  localtime_r(&tmNow,&tmStr); // get local time.. we want daylight savings stat..
	  ymd1_ymd3(yymmdd,&iyr,&imon,&iday);
	  if (iyr < 1900) {
		iyr=(iyr > 80)?iyr+1900:iyr+2000;
	  }
   	  tmStr.tm_sec=isec;
      tmStr.tm_min=imin;
      tmStr.tm_hour=ihr;
      tmStr.tm_mday=iday;
      tmStr.tm_mon =imon-1;
      tmStr.tm_year=iyr - 1900;
      tmStr.tm_wday=0 ;				// day of  the week.. hope they don't use this...
	  tmStr.tm_yday=dmToDayNo(iday,imon,iyr);
   	  tmFut=mktime(&tmStr);
	}
	return((double)tmFut + fracSec);
}

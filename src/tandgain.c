/*
 * $Id$
 *
 * $Log$
 */

#include <stdio.h>
#include <math.h>

#define dtor (M_PI / 180.)
#define ZA_MAX 19.69

/*
  Tsys curves from Aug 2000  ("za -14" terms only apply for za > 14)

  polA: Tsys(za) = 26.000 - .00016*za + .078998*(za-14)^2 + .005007*(za-14)^3 
  polB: Tsys(za) = 25.127 - .00738*za + .061695*(za-14)^2 + .009357*(za-14)^3
*/

float tsysc[2][4] = {{26.000, -.00016, .078998, .005007},
		     {25.127, -.00738, .061695, .009357}};

/* gain curves from Aug 2000, Sep-Dec 2001, and Mar 2003 - Feb 2004 */
/* (taken from ~phil/idl/data/gain.datR12 on 2004 Mar 11)           */
/*                                                                  */
/* Gain curve from 2003-4 adjusted downward by a factor of 1.052 on */
/* 2007 Jul 10 to account for lowered-after-the-fact cal values     */
/* -- see /pkg/rsi/local/libao/phil/data/cal.datR12                 */
/*                                                                  */
/* post-Maria gain curve (after 2017 Sep 21) added on 2018 Jul 10   */
/* -- see http://www.naic.edu/~phil/calsbn/sbn.html#gaincurvemay18  */
/*    and /pkg/rsi/local/libao/phil/data/gain.datR12                */

float gc2000Aug[] = { 6.5506e+00, -6.1362e-02, -8.2047e-03,  2.8626e-04,
	              2.2321e-01,  4.3514e-01, 
	              1.1014e-01, -1.6827e-01,
	             -1.6264e-01, -6.6516e-02};

float gc2001Sep[] = { 9.7567e+00, -1.4662e-02, -7.8413e-02,  6.0635e-03,
                     -2.3754e-01, -8.1630e-02,
                     -2.4594e-02, -3.1619e-02,
                     -4.0554e-01, -9.3417e-02};

float gc2003Mar[] = { 9.3024e+00,  2.2544e-02, -3.9145e-02, -2.1812e-03,
                     -2.6398e-01, -7.1019e-02,
                      1.2424e-01, -1.4299e-01,
                      1.3503e-01,  2.8582e-02};

float gc2017Nov[] = { 7.9519e+00, -7.9284e-02, -2.8894e-02,  2.1964e-04,
                      4.4723e-01, -6.3857e-01,
                      1.1302e-02, -4.2917e-02,
                      1.8074e-01, -2.1288e-01};

double S_tsys(double za, double jd, int chan, int give_warning) {

  /* As of 2003 July, the only full-blown fit of OC and SC system temperatures   */
  /* as functions of zenith angle was performed in 2000 August.  However, I have */
  /* visually inspected (using the "tsys" package) Tsys measurements made during */
  /* the period 1999 April through 2003 July, and have crudely estimated the     */
  /* lower envelope (minimum Tsys values for low ZA) for each channel over those */
  /* dates; I have used piecewise constant and linear functions to do this.      */
  /*                                                                             */
  /* S_tsys first computes the system temperature for the desired channel using  */
  /* the August 2000 fit, then scales it by the ratio of minimum Tsys values for */
  /* the desired date vs. August 2000.  In other words, it assumes that the      */
  /* fractional variation of Tsys with zenith angle remains constant in time.    */
  /*                                                                             */
  /* CM 2002 August 12                                                           */
  /*                                                                             */
  /* Modified by CM 2008 Feb 21:                                                 */
  /*      Only scale the constant term, not the full ZA-dependent Tsys function, */
  /*      by the ratio of minimum Tsys values                                    */
  /*                                                                             */
  /* Updated by CM on 2011 Dec 16, incorporating a somewhat smoothed version of  */
  /*      Patrick's Tsys expressions for 2008-2011                               */
  /*                                                                             */
  /* Modified by CM on 2013 Mar 6:                                               */
  /*      Add "give_warning" argument to S_tsys function so that we can limit    */
  /*      how many warnings are displayed if the Tsys values need updating       */

  const double jd1998Jan01=2450814.5, jd1998Aug17=2451042.5, jd1998Dec01=2451148.5,
               jd1999Apr20=2451288.5, jd1999Jun29=2451358.5, jd1999Oct07=2451458.5,
               jd1999Nov16=2451498.5, jd1999Dec26=2451538.5, jd2000Jan11=2451554.5,
               jd2000Aug12=2451768.5, jd2001Aug13=2452134.5, jd2002Feb19=2452324.5,
               jd2002Apr10=2452374.5, jd2002Jul19=2452474.5, jd2002Sep22=2452539.5,
               jd2002Oct07=2452554.5, jd2003Mar11=2452709.5, jd2003Jul09=2452829.5,
               jd2003Oct14=2452927.0, jd2004Jun15=2453171.5, jd2004Nov05=2453314.5,
               jd2004Nov30=2453339.5, jd2005Jan01=2453371.5, jd2005Jan22=2453392.5,
               jd2005Jul03=2453554.5, jd2005Aug27=2453609.5, jd2005Oct20=2453663.5,
               jd2005Nov01=2453675.5, jd2006Jan01=2453736.5, jd2006Apr10=2453835.5,
               jd2006Jul08=2453924.5, jd2006Nov17=2454056.5, jd2007Jan01=2454101.5,
               jd2007Mar01=2454160.5, jd2007Apr25=2454215.5, jd2008Jan31=2454496.5,
               jd2009Mar09=2454899.5, jd2009Jun07=2454989.5, jd2011Jul07=2455749.5,
               jd2011Sep28=2455832.5, jd2012Nov22=2456253.5, jd2013Aug22=2456526.5,
               jd2019May06=2458609.5, jd2019Aug16=2458711.5, jd2019Dec02=2458819.5,
               jd2019Dec12=2458829.5, jd2019Dec26=2458843.5, jd2020Mar04=2458912.5,
               jd2020Mar20=2458928.5;

  /* Note that 1999 April 20 is DOY 110 */

  /* Some of the Tsys values below, starting on 2003 Mar 11 and going up to
     (but not including) 2006 Apr 10, were revised downward after the fact
     by a factor of 1.052 to account for the revised cal values that were
     belatedly put into use in the IDL "tsys" routine on 2006 Apr 10.       */

  const float tsys1998Aug17_min[2] = {31.1, 26.8},
              tsys1998Dec01_min[2] = {33.0, 29.0},
              tsys1999Apr20_min[2] = {30.5, 27.0},
              tsys1999Oct07_min[2] = {28.0, 26.0},
              tsys1999Nov16_min[2] = {30.0, 28.0},
              tsys1999Dec26_min[2] = {32.0, 32.0},
              tsys2000Jan11_min[2] = {29.0, 26.0},
              tsys2000Aug11_min[2] = {26.0, 25.0},
              tsys2000Aug12_min[2] = {25.0, 23.5},
              tsys2001Aug13_min[2] = {25.5, 23.5},
              tsys2002Apr10_min[2] = {26.5, 24.5},
              tsys2002Jul19_min[2] = {26.0, 24.0},
              tsys2002Sep22_min[2] = {29.5, 23.5},
              tsys2002Oct07_min[2] = {27.5, 23.0},
              tsys2003Mar11_min[2] = {25.2, 21.9},
              tsys2003Jul09_min[2] = {25.7, 22.3},
              tsys2003Oct13_min[2] = {25.2, 22.8},
              tsys2003Oct14_min[2] = {22.8, 25.2},  /* switched channels */
              tsys2004Jun15_min[2] = {22.3, 24.7},
              tsys2004Nov30_min[2] = {22.3, 26.6},
              tsys2004Dec31_min[2] = {23.8, 28.0},
              tsys2005Jan01_min[2] = {27.6, 26.1},
              tsys2005Jan22_min[2] = {23.3, 25.2},
              tsys2005Jul02_min[2] = {25.7, 27.6},
              tsys2005Jul03_min[2] = {24.7, 26.6},
              tsys2005Aug27_min[2] = {23.8, 25.7},
              tsys2005Oct20_min[2] = {28.5, 26.1},
              tsys2005Nov01_min[2] = {24.2, 26.1},
              tsys2006Jan01_min[2] = {23.8, 25.7},
              tsys2006Apr10_min[2] = {24.5, 26.0},  /* revised cal values */
              tsys2006Jul07_min[2] = {25.0, 26.5},
              tsys2006Jul08_min[2] = {24.0, 26.0},
              tsys2006Nov17_min[2] = {23.0, 26.0},
              tsys2007Jan01_min[2] = {22.0, 25.5},
              tsys2007Mar01_min[2] = {22.0, 25.5},
              tsys2007Apr25_min[2] = {23.0, 26.5},  /* platform painting starts */
              tsys2007Dec06_min[2] = {23.0, 26.5},  /* platform painting finished */
              tsys2008Jan31_min[2] = {23.0, 28.0},
              tsys2009Mar09_min[2] = {23.0, 28.0},
              tsys2009Jun07_min[2] = {23.0, 29.0},
              tsys2011Jul07_min[2] = {23.0, 29.0},
              tsys2011Sep28_min[2] = {23.5, 29.5},
              tsys2012Nov22_min[2] = {22.0, 23.5},
              tsys2013Aug22_min[2] = {24.0, 24.0},
              tsys2019May06_min[2] = {29.0, 24.0},  /* loosening connection, rcvr compression failed */
              tsys2019Aug16_min[2] = {34.0, 24.0},  /* loosening connection, rcvr compression failed */
              tsys2019Dec02_min[2] = {25.0, 45.0},  /* switched channels */
              tsys2019Dec12_min[2] = {25.0,120.0},
	      tsys2019Dec26_min[2] = {25.0, 65.0},
	      tsys2020Mar04_min[2] = {25.0, 50.0};

  double tsys2000Aug_min, interp, tsys_min, tsys;

  if (chan != 1 && chan != 2) return (0.);

  chan--;

  /*  Estimate the minimum Tsys for the Julian date of interest, using linear
      interpolation when there is a clear trend of change between a pair of
      dates or else constant values when there is no clear trend.  The Tsys
      expression for the most recent data is an extrapolation, with a warning
      output when the extrapolation goes beyond 120 days; the error thus
      introduced should be no worse than 1.0 K, still small relative to
      overall absolute calibration uncertainties of 25% or so.                 */

  if (jd < jd1998Jan01) {
      return(-99.99);   /* haven't collected Tsys values this early */
  } else if (jd < jd1998Aug17) {
      tsys_min = tsys1998Aug17_min[chan];
  } else if (jd < jd1998Dec01) {
      interp = (jd - jd1998Aug17) / (jd1998Dec01 - jd1998Aug17);
      tsys_min = tsys1998Aug17_min[chan]
                 + interp*(tsys1998Dec01_min[chan] - tsys1998Aug17_min[chan]);
  } else if (jd < jd1999Apr20) {
      interp = (jd - jd1998Dec01) / (jd1999Apr20 - jd1998Dec01);
      tsys_min = tsys1998Dec01_min[chan]
                 + interp*(tsys1999Apr20_min[chan] - tsys1998Dec01_min[chan]);
  } else if (jd < jd1999Jun29) {
      tsys_min = tsys1999Apr20_min[chan];
  } else if (jd < jd1999Oct07) {
      tsys_min = tsys1999Oct07_min[chan];
  } else if (jd < jd1999Nov16) {
      interp = (jd - jd1999Oct07) / (jd1999Nov16 - jd1999Oct07);
      tsys_min = tsys1999Oct07_min[chan]
                 + interp*(tsys1999Nov16_min[chan] - tsys1999Oct07_min[chan]);
  } else if (jd < jd1999Dec26) {
      interp = (jd - jd1999Nov16) / (jd1999Dec26 - jd1999Nov16);
      tsys_min = tsys1999Nov16_min[chan]
                 + interp*(tsys1999Dec26_min[chan] - tsys1999Nov16_min[chan]);
  } else if (jd < jd2000Jan11) {
      interp = (jd - jd1999Dec26) / (jd2000Jan11 - jd1999Dec26);
      tsys_min = tsys1999Dec26_min[chan]
                 + interp*(tsys2000Jan11_min[chan] - tsys1999Dec26_min[chan]);
  } else if (jd < jd2000Aug12) {
      interp = (jd - jd2000Jan11) / (jd2000Aug12 - jd2000Jan11);
      tsys_min = tsys2000Jan11_min[chan]
                 + interp*(tsys2000Aug11_min[chan] - tsys2000Jan11_min[chan]);
  } else if (jd < jd2001Aug13) {
      tsys_min = tsys2000Aug12_min[chan];
  } else if (jd < jd2002Feb19) {
      tsys_min = tsys2001Aug13_min[chan];
  } else if (jd < jd2002Apr10) {
      interp = (jd - jd2002Feb19) / (jd2002Apr10 - jd2002Feb19);
      tsys_min = tsys2001Aug13_min[chan]
                 + interp*(tsys2002Apr10_min[chan] - tsys2001Aug13_min[chan]);
  } else if (jd < jd2002Jul19) {
      interp = (jd - jd2002Apr10) / (jd2002Jul19 - jd2002Apr10);
      tsys_min = tsys2002Apr10_min[chan]
                 + interp*(tsys2002Jul19_min[chan] - tsys2002Apr10_min[chan]);
  } else if (jd < jd2002Sep22) {
      tsys_min = tsys2002Jul19_min[chan];
  } else if (jd < jd2002Oct07) {
      tsys_min = tsys2002Sep22_min[chan];
  } else if (jd < jd2003Mar11) {
      tsys_min = tsys2002Oct07_min[chan];
  } else if (jd < jd2003Jul09) {
      interp = (jd - jd2003Mar11) / (jd2003Jul09 - jd2003Mar11);
      tsys_min = tsys2003Mar11_min[chan]
                 + interp*(tsys2003Jul09_min[chan] - tsys2003Mar11_min[chan]);
  } else if (jd < jd2003Oct14) {
      interp = (jd - jd2003Jul09) / (jd2003Oct14 - jd2003Jul09);
      tsys_min = tsys2003Jul09_min[chan]
                 + interp*(tsys2003Oct13_min[chan] - tsys2003Jul09_min[chan]);
  } else if (jd < jd2004Jun15) {
      tsys_min = tsys2003Oct14_min[chan];
  } else if (jd < jd2004Nov05) {
      tsys_min = tsys2004Jun15_min[chan];
  } else if (jd < jd2004Nov30) {
      tsys_min = tsys2004Nov30_min[chan];
  } else if (jd < jd2005Jan01) {
      interp = (jd - jd2004Nov30) / (jd2005Jan01 - jd2004Nov30);
      tsys_min = tsys2004Nov30_min[chan]
                 + interp*(tsys2004Dec31_min[chan] - tsys2004Nov30_min[chan]);
  } else if (jd < jd2005Jan22) {
      tsys_min = tsys2005Jan01_min[chan];
  } else if (jd < jd2005Jul03) {
      interp = (jd - jd2005Jan22) / (jd2005Jul03 - jd2005Jan22);
      tsys_min = tsys2005Jan22_min[chan]
                 + interp*(tsys2005Jul02_min[chan] - tsys2005Jan22_min[chan]);
  } else if (jd < jd2005Aug27) {
      tsys_min = tsys2005Jul03_min[chan];
  } else if (jd < jd2005Oct20) {
      tsys_min = tsys2005Aug27_min[chan];
  } else if (jd < jd2005Nov01) {
      tsys_min = tsys2005Oct20_min[chan];
  } else if (jd < jd2006Jan01) {
      interp = (jd - jd2006Jan01) / (jd2005Nov01 - jd2006Jan01);
      tsys_min = tsys2005Nov01_min[chan]
                 + interp*(tsys2006Jan01_min[chan] - tsys2005Nov01_min[chan]);
  } else if (jd < jd2006Apr10) {
      interp = (jd - jd2006Jan01) / (jd2006Apr10 - jd2006Jan01);
      tsys_min = tsys2006Jan01_min[chan]
                 + interp*(tsys2006Apr10_min[chan] - tsys2006Jan01_min[chan]);
  } else if (jd < jd2006Jul08) {
      interp = (jd - jd2006Apr10) / (jd2006Jul08 - jd2006Apr10);
      tsys_min = tsys2006Apr10_min[chan]
                 + interp*(tsys2006Jul07_min[chan] - tsys2006Apr10_min[chan]);
  } else if (jd < jd2006Nov17) {
      tsys_min = tsys2006Jul08_min[chan];
  } else if (jd < jd2007Jan01) {
      interp = (jd - jd2006Nov17) / (jd2007Jan01 - jd2006Nov17);
      tsys_min = tsys2006Nov17_min[chan]
                 + interp*(tsys2007Jan01_min[chan] - tsys2006Nov17_min[chan]);
  } else if (jd < jd2007Mar01) {
      tsys_min = tsys2007Jan01_min[chan];
  } else if (jd < jd2007Apr25) {
      interp = (jd - jd2007Mar01) / (jd2007Apr25 - jd2007Mar01);
      tsys_min = tsys2007Mar01_min[chan]
                 + interp*(tsys2007Apr25_min[chan] - tsys2007Mar01_min[chan]);
  } else if (jd < jd2008Jan31) {
      tsys_min = tsys2007Dec06_min[chan];
  } else if (jd < jd2009Mar09) {
      tsys_min = tsys2008Jan31_min[chan];
  } else if (jd < jd2009Jun07) {
      interp = (jd - jd2009Mar09) / (jd2009Jun07 - jd2009Mar09);
      tsys_min = tsys2009Mar09_min[chan] 
                 + interp*(tsys2009Jun07_min[chan] - tsys2009Mar09_min[chan]);
  } else if (jd < jd2011Jul07) {
      tsys_min = tsys2009Jun07_min[chan];
  } else if (jd < jd2011Sep28) {
      interp = (jd - jd2011Jul07) / (jd2011Sep28 - jd2011Jul07);
      tsys_min = tsys2011Jul07_min[chan] 
                 + interp*(tsys2011Sep28_min[chan] - tsys2011Jul07_min[chan]);
  } else if (jd < jd2012Nov22) {
      tsys_min = tsys2011Sep28_min[chan];
  } else if (jd < jd2013Aug22) {
      tsys_min = tsys2012Nov22_min[chan];
  } else if (jd < jd2019May06) {
      tsys_min = tsys2013Aug22_min[chan];
  } else if (jd < jd2019Aug16) {
      tsys_min = tsys2019May06_min[chan];
  } else if (jd < jd2019Dec02) {
      tsys_min = tsys2019Aug16_min[chan];
  } else if (jd < jd2019Dec12) {
      tsys_min = tsys2019Dec02_min[chan];
  } else if (jd < jd2019Dec26) {
      tsys_min = tsys2019Dec12_min[chan];
  } else if (jd < jd2020Mar04) {
      tsys_min = tsys2019Dec26_min[chan];
  } else {
      tsys_min = tsys2020Mar04_min[chan];
      if (jd > jd2020Mar20 + 120 && give_warning) {
        printf("*************************************************************************\n");
        printf("***  WARNING: ask CM to update tandgain.c to avoid Tsys extrapolation ***\n");
        printf("*************************************************************************\n");
      }
  }

  /* Get the minimum (ZA = 14 deg) Tsys from the August 2000 fit */

  tsys2000Aug_min = tsysc[chan][0] + tsysc[chan][1]*14.0;

  /* Get Tsys according to the August 2000 fit,
     scaling the constant term by the ratio of minimum Tsys values */

  tsys = tsysc[chan][0] * (tsys_min/tsys2000Aug_min) + tsysc[chan][1] * za;

  za -= 14.;
  if (za > 0.)
    tsys += za*za* (tsysc[chan][2] + tsysc[chan][3] * za);

  return(tsys);
}

double S_gain(double za, double az, double jd, int transmit) {

  /*  The dish surface was adjusted starting in 2000 December and ending on 2001 Jan 13;  */
  /*  improvements were more gradual after this.  In the (probable) absence of better     */
  /*  data, and given that significant east-west asymmetries were measured in early       */
  /*  2001 January, we'll settle for simple linear interpolation, as follows:             */
  /*                                                                                      */ 
  /*  1) Assume that the Aug. 2000 gain curve holds until 2000 Dec 1 and that the         */
  /*     Sep. 2001 curve holds after 2001 Sep 1.                                          */
  /*  2) Graphs on Phil's Web pages show that the dish-averaged gain was 9.33 K/Jy after  */
  /*     2001 Sep. 1 and was something like 8.0 K/Jy, about 15% lower, on 2001 Jan 13.    */
  /*     Hence we'll define the gain on 2001 Jan 13 to be 85% of the Sep. 2001 value.     */
  /*  3) Use two straight lines to connect these three dates.                             */
  /*                                                                                      */
  /*  CM 2002 Jan 9                                                                       */
  /*                                                                                      */
  /*  Modified by CM 2004 Mar 11:                                                         */
  /*      Added gain curve for Mar 2003 - Feb 2004.  Things changed due to shimming       */
  /*      performed prior to Mar 2003, so we'll use the Sep 2001 curve though Feb 2003    */
  /*      and the new curve starting on 2003 Mar 1.                                       */
  /*                                                                                      */
  /*  Modified by CM 2005 Dec 1:                                                          */
  /*      Return zero if target is out of the beam                                        */
  /*                                                                                      */
  /*  Modified by CM 2007 Jul 10:                                                         */
  /*      Add "transmit" parameter to account for a new receive horn (2006 May 4)         */
  /*      and subsequent recalibration based on that new receive horn; the new horn had   */
  /*      4% lower gain than the old one (but 10-15% better Tsys), so the replacement     */
  /*      and recalibration effectively increased the TX gain by 4% relative to RX gain.  */
  /*                                                                                      */
  /*  Modified by CM 2018 Jul 10:                                                         */
  /*      Added gain curve for Nov 2017 - Jan 2018 (post-Hurricane-Maria)                 */

  const double jd2000Dec01=2451879.5, jd2001Jan13=2451922.5, jd2001Sep01=2452153.5,
               jd2003Mar01=2452699.5, jd2006May04=2453859.5, jd2017Nov01=2458058.5;

  double za10, za14, gain2000Aug, gain2001Sep, gain2001Jan13, gain2003Mar, gain2017Nov,
         interp, gain;

  /*  Return zero gain if the target has left the beam  */

  if (za >= ZA_MAX)
    return 0.0;

  /*  The three za-10 terms in the Aug. 2000 fit apply for all zenith angles,  */
  /*  whereas the two za-14 terms in the later fits apply for za > 14 only.    */

  za10 = za - 10.;
  za14 = (za > 14.) ? za - 14. : 0.0;
  az = (az + 180.) * dtor; /* curves are in feed az */

  gain2000Aug = gc2000Aug[0] +
                za10*(gc2000Aug[1] + za10*(gc2000Aug[2] + za10*gc2000Aug[3])) + 
                gc2000Aug[4] * cos(    az) + gc2000Aug[5] * sin(    az) +
                gc2000Aug[6] * cos(2 * az) + gc2000Aug[7] * sin(2 * az) +
                gc2000Aug[8] * cos(3 * az) + gc2000Aug[9] * sin(3 * az);
  gain2001Sep = gc2001Sep[0] +
                za*gc2001Sep[1] + za14*za14*(gc2001Sep[2] + za14*gc2001Sep[3]) + 
                gc2001Sep[4] * cos(    az) + gc2001Sep[5] * sin(    az) +
                gc2001Sep[6] * cos(2 * az) + gc2001Sep[7] * sin(2 * az) +
                gc2001Sep[8] * cos(3 * az) + gc2001Sep[9] * sin(3 * az);
  gain2001Jan13 = 0.85*gain2001Sep;
  gain2003Mar = gc2003Mar[0] +
                za*gc2003Mar[1] + za14*za14*(gc2003Mar[2] + za14*gc2003Mar[3]) + 
                gc2003Mar[4] * cos(    az) + gc2003Mar[5] * sin(    az) +
                gc2003Mar[6] * cos(2 * az) + gc2003Mar[7] * sin(2 * az) +
                gc2003Mar[8] * cos(3 * az) + gc2003Mar[9] * sin(3 * az);
  gain2017Nov = gc2017Nov[0] +
                za*gc2017Nov[1] + za14*za14*(gc2017Nov[2] + za14*gc2017Nov[3]) + 
                gc2017Nov[4] * cos(    az) + gc2017Nov[5] * sin(    az) +
                gc2017Nov[6] * cos(2 * az) + gc2017Nov[7] * sin(2 * az) +
                gc2017Nov[8] * cos(3 * az) + gc2017Nov[9] * sin(3 * az);

  if (jd < jd2000Dec01) {
      gain = gain2000Aug;
  } else if (jd < jd2001Jan13) {
      interp = (jd - jd2000Dec01) / (jd2001Jan13 - jd2000Dec01);
      gain = gain2000Aug + interp*(gain2001Jan13 - gain2000Aug);
  } else if (jd < jd2001Sep01) {
      interp = (jd - jd2001Jan13) / (jd2001Sep01 - jd2001Jan13);
      gain = gain2001Jan13 + interp*(gain2001Sep - gain2001Jan13);
  } else if (jd < jd2003Mar01) {
      gain = gain2001Sep;
  } else if (jd < jd2006May04) {
      gain = gain2003Mar;
  } else if (jd < jd2017Nov01) {
      gain = (transmit)? 1.04*gain2003Mar : gain2003Mar;
  } else {
      gain = (transmit)? 1.04*gain2017Nov : gain2017Nov;
  }

  return(gain);
}

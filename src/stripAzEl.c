#ifdef SWAPEM
#include <byteswap.h>
#define SWAB4(x) bswap_32(x)
#else
#define SWAB4(x) (x)
#endif

/**
   
   Strips azimuth and zenith angle of Gregorian (and other info) from
   data file.  Output to ascii file.
   
**/

/*
   PT, 2012 Dec:
       -- Changed tmpnam to mkstemp (via tempoutfile[L_tmpnam] to 
           tempoutfile[6] and tempoutfile = "XXXXXX")

   MN, 2011 Dec:
       -- Made several small changes to allow for Linux compilation:
           Byteswap numerical values to little endian using the Linux
            definition in byteswap.h
           Use -DSWAPEM during compilation to enable byteswap
	   Use -DBCOPY during compilation to avoid a problem with the
	    definition of memcpy in the vx*.h headers (bcopy is
            backwards)
           Use fgetpos after opening file rather than setting 
            saveDatafilePos to zero (fpos_t is an opaque type)

   CM, 2013 Mar:
       -- Add "give_warning" argument to S_tsys function so we get just
          one warning displayed if the Tsys values need to be updated
       -- Allow more than 100 TX power values (i.e., more than 100 scans),
          and check to see if the maximum is exceeded

   CM, 2008 Sep:
       -- Add "-d" option to adjust hop bandwidth, first hop frequency,
          and transmit offset for the average Doppler shift and write the
          adjusted values to the header; this option is needed if the
          'doptrksmp' setup parameter was left out (or set to zero)
          during the observations

   CM, 2007 Jul:
       -- Add "zerofill" parameter to the control file, thus permitting
          zero-filled spectra (generally when good frequency resolution
          requires more fft points than data samples)
       -- Add "transmit" argument to S_gain routine to account for new
          (2006 May 4) transmitter horn that raised transmit gain (but
          not receive gain) by 4%
       -- Bug fix (pointed out by MCN): When backing up and then reading
          forward to get to the start of a new scan, read forward using
          the *preceding* scan's header length and record length
       -- Bug fix related to the 2007 Feb bug fix regarding record-end
          times: for one-second records, the record-end time in the header
          is sometimes truly the record-end time but is sometimes the
          record-START time, depending on whether or not the datataking
          program is running a few microseconds slow.  This can change
          within a single scan; for example, the times for a scan that
          started 35899 seconds after midnight might read 35899, 35900,
          35902 (jump from start to end time), 35903, 35904, 35905, etc.
          Hence the code has to provide a second of "slack" for deciding
          whether or not we've reached the end of a dwell.

   CM, 2007 Feb:
       -- Eliminate calls to fseek (which doesn't work for large files)
          and use fgetpos / fsetpos instead.
       -- Read data records on the fly rather than storing the entire
          datafile before starting to produce output.  For a given scan we
          can't know how many dwells should be written until the entire scan
          has been read in (due to skipped records, truncation to an integer
          number of ffts, etc.), so write on the fly to a temporary file and
          then copy the correct number of dwells to the real output file.
       -- Correct a bug: the range of record-end times (integer-TRUNCATED
          seconds after midnight) of data records that contribute to a given
          dwell was being taken to include both the dwell's start time and its
          end time, whereas in fact it should include the start time but
          should include only the FIRST record whose record-end time is equal
          to the dwell's end time.  For example, a 10-sec dwell might get
          contributions from data records whose record-end times run from
          t = 4822 through t = 4831 and from the first record with t = 4832.

   CM, 2006 Aug:
       -- Add -E argument for getting delay and Doppler corrections
       -- Read ephemeris records on the fly rather than storing the entire
          ephemeris file(s) before starting to look at the datafile

   CM, 2005 Dec:
       -- Don't use a dwell if the target is out of the beam at any point
          during that dwell

   CM, 2005 Oct:
       -- Modify the method for reading in TX power values, to be consistent
          with the new writecal program

   CM, 2005 Jun:
       -- Write hop bandwidth and starting hop frequency with extra precision

   CM, 2004 Feb:
       -- Give a warning rather than exiting if the specified input scan(s)
          conflict with the fsuf value given in the control file -- say, if
          fsuf = 2 but the first and last scans specified are "3 3" or "2 4".
          For example, if on a given night for a given NEA target there are
          numerous datafiles containing one scan apiece, you can specify scans
          "1 1" for each datafile but keep the output files separate by using
          different control files with a different fsuf value for each.

   CM, 2003 Jul:
       -- Fix error in computing date, only relevant when a scan starts
          just before midnight and you skip enough records to go into
          the start of the next date

   CM, 2003 Mar:
       -- Fix error in computing time per record (dt).  Until now this was set
          equal to [(start time of last record) - (start time of 1st record)]
          divided by the # of records, when in fact it should have been
          divided by [(# of records) - 1].  It's now computed using parameters
          like record length, sampling rate, etc., to avoid problems with
          integer time truncation.

   CM, 2002 Jul-Aug:
       -- Fix treatment of 12-bit data (2 bytes per real sample, not 1.5)
       -- Adjust Tsys function in tandgain.c so that for each polarization
          channel, the August 2000 fit is scaled by the ratio of the minimum
          Tsys at a given date to that in August 2000.  This now parallels
          the treatment of gain, except that I've retained the option of
          specifying your own (linear or two-part linear) Tsys curves.

   CM, 2002 Mar:
       -- Add RA and dec to end of each output record

   CM, 2002 Feb:
       -- Major change: Now read the control-parameter file for drv_cw.sc so
          as to ensure that for each scan, the number of records output is
          the same as the number of power spectra output by drv_cw.sc.  No
          more assumptions as to number of bits per sample, range of records
          used, etc.  Additionally, the data truncation carried out by
          drv_cw.sc (more specifically, by zerofill, fftfilter, and avgdata)
          is now explicitly accounted for.  Before now it was possible to get
          the hops out of synch when processing more than one scan at a time.
       -- As a side-effect of this change, no longer need the "-d datafile"
          or "-s skip" command-line arguments, just "-i infile"
       -- Eliminated command-line argument "-o outfile": now construct the
          output filename using the same stem and suffix that drv_cw.sc uses
       -- Eliminated the "old" output mode, leaving just the default and "-z"

   CM, 2002 Jan:
       -- Fixed bug (?) which confused program when write times went backwards
          by a few seconds at a time on 2001 Mar 28.  Such scans must still
          be reduced without the -e (ephemeris) option of stripAzEl, but at
          least the program gives sensible output.  (NOTE: Still need to check
          that it didn't skip any hops in the output.)
       -- Compute the Julian date at the start of the first scan, so that
          this value can be used as an argument to S_gain (to determine which
          gain curve to use).

   CM, 2001 Oct:
       -- Now checks each scan for header length and record length, in case
          datafile has different kinds of scans (e.g., cw scans and
          closed-loop tests) lumped together.
       -- Implemented -e <ephemeris file> option
       -- Fixed incorrect handling of avoidance intervals which all are
          are entirely after midnight in midnight-crossing data
       -- Better handling of mean azimuth for dwells which cross 360/0 deg

   CM, 2001 Sep:
       -- New string entry function which allows spaces in target name
       -- Get date from datafile header instead of entering by hand
       -- Turned macros into procedures or else ordinary statements

   CM, 2001 Aug 20: Increased the maximum header length to avoid having
   the target name (and who knows what else) overwritten with junk; the
   program now halts rather than issuing a mild warning if this maximum
   is exceeded.

   GB, 2001 Jan 17: Cleaned up the azimuth issue (IMHO). Now the feed
   azimuth from ri header is immediately converted into sky azimuth,
   which is solely used internally and is output.  S_gain() expects to
   be passed this sky azimuth.  Not sure if any further processing
   using the azimuth anyway, but not it is more easily compared to an
   ephemeris which likely reports sky position.

   Some changes by CM on 2001 Jan 12:

   Fixed incorrect receive-azimuth argument in call to S_gain

   Explicitly ask whether or not to use hard-coded Tsys fits
   (can be expanded later to allow different fits [8/00, 1/01, etc.])

   Ask for number of scans in data file so that TX power can be input
   for each scan separately

   Read and write round-trip time in seconds rather than minutes, since
   this is what's seen in the ephemeris  

   Write hop1 ("color" [0, 1, 2, 3] of first dwell) into "HOP" header line,
   not just into the line for each dwell; this makes it easier to modify
   existing IDL routines so that they can process the output of stripAzEl

*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <aohdrLib.h>
/* #include <string.h> */
#include <malloc.h>

/* expected sizes. data buffer is dynamically allocated, so matching
   isn't critical, but hdr should be similar (extra few bytes seem to
   occur occasionally but that is ok as long as most parameters line
   up in aohdr structure */
#define MAX_HDR_LEN 1024
#define MAX_EPH_LEN 255

#define ARECIBO_LAT 18.34350
#define SBAND_FREQ 2.380e9
#define ZA_MAX 19.69
#define ZA_KNEE 16.0
#define dtor (M_PI/180.0)
#define FALSE 0
#define TRUE 1
#define MAX_NSCANS_USE 1000

/* new measured values */
double S_tsys(double za, double jd, int chan, int give_warning);
double S_gain(double za, double az, double jd, int transmit);

#define       gfactor 2.18462981  /* converts K/Jy to real gain*1e-6,
                                     gfactor = (8*pi*k/lambda^2)*1e20  */

/* targets I use often - GB */
int n_known_targets = 6;
char *known_target[]    = {"Io",   "Europa", "Ganymede", "Callisto", "Titan", "Iapetus"};
double known_diameter[] = {3630.0, 3138.0,   5262.0,     4800.0,     5150.0,  1460.0}; /* km */
double known_period[]   = {1.77,   3.55,     7.16,       16.7,       15.95,   79.33 }; /* day */
double known_jd0[]      = {-1,     -1,       -1,         -1,         2451844.57291667, 2451833.1250}; /* jd of phase=0 */

/*==================================================================*/

/** Returns Sband system temperature given ZA (degrees), Tsys1 = temp at
    zenith angle ZA1, Tsys2 = temp at zenith angle ZA2.  If ZA2 > ZAknee,
    assume change is due to dish.  If ZA2 < Zaknee, assume another source
    for the change and make a straight line between ZA1 and ZA2 **/

double S_tsys_user(double ZA, double Tsys1, double Tsys2, double ZA1, double ZA2)
{
  if (ZA2 > ZA_KNEE) {
      if (ZA > ZA_KNEE)
        return Tsys1 + (Tsys2 - Tsys1)*(ZA - ZA_KNEE)/(ZA2 - ZA_KNEE);
      else
        return Tsys1;
  } else {
       return Tsys1 + (Tsys2 - Tsys1)*(ZA - ZA1)/(ZA2 - ZA1);
  }
}

/*==================================================================*/

/* Display a prompt and then (safely) read a string from stdin     */
/*                                                                 */
/* If not_all_white is true (1), askstring keeps reading until it  */
/* gets a string with at least one character that's not whitespace */

void askstring(char *query, char *s, int size, int not_all_white)
{
  int i, savesize;
  char c;

  printf(query);
  savesize = size;
  do {
      size = savesize;
      i = 0;
      while (--size > 0) {
        c = getchar();
        if (c == EOF || c == '\n')
          size = 0;
        else
          s[i++] = c;
      }
      s[i] = '\0';
  } while (not_all_white && (i == 0 || i == strspn(s, " \t\f\n")));
}

/*==================================================================*/

/* Convert the day of the year (1-366) to month and day of the month */

void doyToCal(int year, int doy, int *month, int *day)
{

  /* Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec */

  static int normal_year[] =
  {    0,  31,  59,  90, 120, 151, 181, 212, 243, 273, 304, 334 };
  static int leap_year[]   =
  {    0,  31,  60,  91, 121, 152, 182, 213, 244, 274, 305, 335 };

  int i, doylist[12], maxdoy, mon;

  /* Is it a leap year? */

  if ( ((year%400) == 0) || ( ((year%100)!=0) && ((year%100)%4)==0 ) ) {
      for (i=0; i<12; i++)
        doylist[i] = leap_year[i];
      maxdoy = 366;
  } else {
      for (i=0; i<12; i++)
        doylist[i] = normal_year[i];
      maxdoy = 365;
  }

  /* Check for legal year and day of year */

  if (year < 1600 || year > 2100) {
      printf("ERROR: doyToCal only handles years 1600-2100\n");
      *month = 0; 
      *day = 0; 
      return;
  } else if (doy < 1 || doy > maxdoy) {
      printf("ERROR in doytoCal: Need 1 <= doy <= %d\n", maxdoy);
      *month = 0;
      *day = 0;
      return;
  }

  /* Figure out the month and the day of the month */

  mon = 12;
  while (doy <= doylist[mon-1])
    mon--;
  *month = mon;
  *day = doy - doylist[mon-1];

  return;

}

/*==================================================================*/

/* Convert AST year, day of the year (1-366) and hour to */
/* UTC year, month, day of the month, and hour           */

void ASTtoUTC(ASTyear, ASTdoy, ASThour, year, month, day, hour)

int ASTyear, ASTdoy, ASThour, *year, *month, *day, *hour;

{

  /* Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec */

  static int normal_year[] =
  {    0,  31,  59,  90, 120, 151, 181, 212, 243, 273, 304, 334 };
  static int leap_year[]   =
  {    0,  31,  60,  91, 121, 152, 182, 213, 244, 274, 305, 335 };

  int i, doylist[12], maxdoy, doy, yr, mon, hr;

  /* Is the AST year a leap year? */

  if ( ((ASTyear%400) == 0) || ( ((ASTyear%100)!=0) && ((ASTyear%100)%4)==0 ) ) {
      for (i=0; i<12; i++)
        doylist[i] = leap_year[i];
      maxdoy = 366;
  } else {
      for (i=0; i<12; i++)
        doylist[i] = normal_year[i];
      maxdoy = 365;
  }

  /* Check for legal year and day of year */

  if (ASTyear < 1600 || ASTyear > 2100) {
      printf("ERROR: ASTtoUTC only handles years 1600-2100\n");
      *year = 0;
      *month = 0; 
      *day = 0; 
      return;
  } else if (ASTdoy < 1 || ASTdoy > maxdoy) {
      printf("ERROR in ASTtoUTC: Need 1 <= ASTdoy <= %d\n", maxdoy);
      *year = 0;
      *month = 0;
      *day = 0;
      return;
  }

  /* Add four hours to go from AST to UTC hour / day of year / year */

  yr = ASTyear;
  doy = ASTdoy;
  hr = ASThour + 4;
  if (hr >= 24) {
    doy++;
    hr -= 24;
  }
  if (doy > maxdoy) {
    yr++;
    doy -= maxdoy;
  }

  /* Figure out the UT month and day of the month */

  mon = 12;
  while (doy <= doylist[mon-1])
    mon--;
  *year = yr;
  *month = mon;
  *day = doy - doylist[mon-1];
  *hour = hr;

}

/*==================================================================*/

/*  Turn a calendar date and time into a Julian date                       */
/*                                                                         */
/*  Note that month is in the range [1-12] and that sec is floating-point  */

double JulianDate(year, month, day, hr, min, sec)

int year, month, day, hr, min;
double sec;

{
  int doy, year00;
  int subCentury = year % 100;
  long dayOfCentury;

  /*                            1600     1700     1800     1900     2000     2100 */
  static int JDcentury[] = { 2305446, 2341971, 2378495, 2415019, 2451543, 2488068 };

  /* Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec */
  static int normal_year[] =
  {    0,  31,  59,  90, 120, 151, 181, 212, 243, 273, 304, 334 };
  static int leap_year[]   =
  {    0,  31,  60,  91, 121, 152, 182, 213, 244, 274, 305, 335 };

  if (year < 1600 || year > 2100)
    return 0.0;

  /* Is it a leap year? */

  if ( (year % 400 == 0) || ( (subCentury != 0) && (subCentury % 4 == 0) ) )
    doy = leap_year[month-1] + day;
  else
    doy = normal_year[month-1] + day;

  /* Was the first year of the century a leap year, and does it matter? */

  year00 = year - subCentury;
  if (year00 % 400 == 0)
    dayOfCentury = 365*subCentury + (subCentury + 3)/4 + doy;
  else
    dayOfCentury = 365*subCentury + (subCentury - 1)/4 + doy;
 
  return JDcentury[year/100 - 16] + dayOfCentury + 0.5
         + (hr + min/60.0 + sec/3600.0)/24.0;

}

/*==================================================================*/

/*  Get string input from a file without overflowing the buffer  */

int GetLine(fp, line, maxSize)

FILE *fp;
char line[];
int maxSize;

{
  int n, limit;
  char c;
    
  n = 0;
  limit = maxSize;

  while (--limit > 0)
    {
    c = getc(fp);
    if (c == EOF || c == '\n')
        limit = 0;
    else
        line[n++] = c;
    }
  line[n] = '\0';

  fflush(fp);
        
  return n;
}

/*==================================================================*/

/*  Get a substring from string, length bytes long, starting with   */
/*  byte start (>= 1).                                              */
/*                                                                  */
/*  If appendNull == TRUE, terminate this substring with a null;    */
/*  the substring is then (length+1) bytes long.                    */
/*                                                                  */
/*  Adapted from Keith Rosema's "substring" function                */

void Substring(substring, string, start, length, appendNull)

char *substring, *string;
int start, length, appendNull;

{
  char *s_ptr, *n_ptr;

  s_ptr = string + (start - 1);
  strncpy(substring, s_ptr, length);
  if (appendNull) {
    n_ptr = substring + length;
    *n_ptr = '\0';
  }

}

/*==================================================================*/

void usage (char *prog)
{
  printf("Usage: %s -i infile [-d] [-e ephfile [-E eph2file]] [-m #] [-z]\n", prog);
  printf("       Outputs info needed by dehop or psd2rdf to calibrate cw scans and set rdf tags.\n");
  printf("       -i       infile = same control-parameter file used as input to drv_cw.sc.\n");
  printf("       -d       apply average Doppler shift to frequency hops and tx offsets\n");
  printf("                    (do NOT use this option if 'doptrksmp = 1' in observing log)\n");
  printf("       -e       ephemeris file to use.  Otherwise ask for round-trip time.\n");
  printf("       -E       updated ephemeris file for generating delay/Doppler corrections.\n");
  printf("       -m #     default setup number (none=ask, 1=Galilean moons & Titan, 2=Iapetus)\n");
  printf("       -z       calculates az/el/parallactic angle instead of standard output.\n");
  exit(0);
}

/*==================================================================*/

int main (int argc, char **argv)
{

  const char *monthName[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                               "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

  FILE       *fpin, *fpdata, *fptempout, *fpout, *fpeph, *fpeph2;
  HDRAO_STD  *aohdr;
  fpos_t      saveDatafilePos, tempoutStartPos;
  char       *infile, *ephfile, *eph2file, *prog, *s, *buf;
  char        hdr[MAX_HDR_LEN], ephLine[MAX_EPH_LEN], dateHrMin_str[18],
              next_dateHrMin_str[18], dateHrMin2_str[18], next_dateHrMin2_str[18],
              ephseconds_str[3], ephrtt_str[16], ephdop_str[17], ephString[18],
              target[80], outstem[256], outsuf[256], datafile[256], inputLine[256],
              firstPiece[256], outfile[256], rahour[3], ramin[3], rasec[6],
              decsign[2], decdeg[3], decmin[3], decsec[5], pwrLine[256],
              promptline[256], tempoutfile[6], outLine[256], commandLine[256];
  int         nhops, hop1, nadd, nskip, nfftave, ignorefirst, setup, ihop,
              doy, year, month, day, avoid_t1[10], avoid_t2[10],
              i, n, mode, ia, j, no_use, hdr_len, rec_len, previous_hdr_len,
              previous_rec_len, nbits, npols, inputLen, TsysMethod, firstScan,
              lastScan, iscan, nscans, ephseconds, next_ephseconds, eph2seconds,
              next_eph2seconds, hr, min, sec, year_UTC, month_UTC, day_UTC, hr_UTC,
              nffts, ndwells, ndwells_out, outsuf_int, recsToSkip,
              recsInScan, minRecsInDwell, ndummydwells, recsUsedInScan, zerofill,
              recsUsedInFile, ndwells_tot, outlen, apply_doppler, nscans_use;
  long        scan, cur_scan, previousScan, secsAfterMidnight, fftlen, firstrec,
              lastrec, nrecs;
  double      az, za, pa, za_tx, az_tx, el, az2, az1_min, az1_max, az1_range,
              az2_min, az2_max, az2_range, az1_mean, az2_mean, az_mean, za_mean, ha,
              decl, cosel, sinel, cosaz, sinaz, tanlat, coslat, sinlat, tsys_mean[2],
              gain_mean, pwr_mean, rtt_mean, pwr[MAX_NSCANS_USE], rtt, dop, rtt1, tsys1[2],
              tsys2[2], za1, za2, diameter, period, frstep, freq1, srate, lambda,
              dwell, txoff, t, t_offset, t_start, t_end, dt, phase0, jd, jd0,
              interpolate, ephrtt, ephdop, ephra, ephdec, next_ephrtt, next_ephdop,
              next_ephra, next_ephdec, ra, dec, eph2rtt, eph2dop, next_eph2rtt,
              next_eph2dop, delcorr, dopcorr, delta_ra, ra1_min, ra1_mean, ra1_max,
              ra2_min, ra2_mean, ra2_max, dec_mean, ra1_range, ra2_range, ra_mean,
              ra2, delcorr_mean, dopcorr_mean, bytesPerRealSample, previous_t,
              nsamples_double, samplesPerRecord_double, dop_mean, dopfactor;

  /* Initialize values */

  TsysMethod = 1;   /* Ask for real value later */

  setup = -1;
  mode = 2;
  apply_doppler = 0;
  nadd = 0;
  nskip = 0;
  nbits = -1;
  npols = -1;
  fftlen = -1;
  nfftave = -1;
  outsuf_int = -1;
  firstrec = 1;
  lastrec = -999;
  ignorefirst = 0;
  zerofill = 0;
  strncpy((char *) datafile,"",255);
  strncpy((char *) outstem,"",255);
  strncpy((char *) outsuf,"",255);
  strncpy((char *) tempoutfile,"X",5);  
  infile = ephfile = eph2file = (char *) NULL;
  fpeph = fpeph2 = fptempout = fpout = NULL;
  /*  saveDatafilePos = 0; */
  prog = argv[0];

  t_offset = 0;
  doy = 0;
  year = 0;
  jd = 0;
  strncpy(dateHrMin_str,"",17);
  strncpy(next_dateHrMin_str,"",17);
  strncpy(dateHrMin2_str,"",17);
  strncpy(next_dateHrMin2_str,"",17);
  ephseconds = next_ephseconds = 0.0;
  ephrtt = next_ephrtt = 0.0;
  ephdop = next_ephdop = 0.0;
  ephra = next_ephra = 0.0;
  ephdec = next_ephdec = 0.0;
  eph2seconds = next_eph2seconds = 0.0;
  eph2rtt = next_eph2rtt = 0.0;
  eph2dop = next_eph2dop = 0.0;

  jd0 = -999.9;
  previous_t = t = -999.9;
  t_start = t_end = -999.9;
  delcorr = dopcorr = -999.9;
  ndwells = ndwells_tot = 0;
  recsInScan = recsUsedInFile = 0;

  dop = 0;
  az1_mean = az2_mean = za_mean = tsys_mean[0] = tsys_mean[1] 
           = gain_mean = pwr_mean = rtt_mean = dop_mean
           = ra1_mean = ra2_mean = dec_mean
           = delcorr_mean = dopcorr_mean = 0.0;
  az1_min = az2_min = ra1_min = ra2_min = 999.9;
  az1_max = az2_max = ra1_max = ra2_max = -999.9;
  no_use = 0;

  tanlat = tan (ARECIBO_LAT * dtor);

  if (argc <= 1) 
    usage(prog);

  for (;--argc>0;) {
    argv++;
    if (!strncmp(*argv, "-i",2))
      infile = *(++argv), argc--;
    else if (!strcmp(*argv, "-d"))
      apply_doppler = 1;
    else if (!strncmp(*argv, "-e",2))
      ephfile = *(++argv), argc--;
    else if (!strncmp(*argv, "-E",2))
      eph2file = *(++argv), argc--;
    else if (!strcmp(*argv, "-m"))
      setup = atoi(*(++argv)), argc--;
    else if (!strcmp(*argv, "-z"))
      mode = 1;
    else
      usage(prog);
  }

  if (!infile || (!ephfile && eph2file))
    usage(prog);

  /* Open the input file (drvcw....) and read the relevant parameters */
  /*                                                                  */
  /* (Actually, ignorefirst is irrelevant to stripAzEl, as it can't   */
  /* affect the *number* of power spectra output per scan)            */

  if ((fpin = fopen(infile,"r")) == NULL) {
    printf("Can't open %s for input.\n", infile);
    exit(0);
  }
     
  while (!feof(fpin)) {
    inputLen = GetLine(fpin, inputLine, 255);
    if (inputLen > 0) {
      s = strtok(inputLine," \t\n");
      if (s) {
        strcpy(firstPiece,s);
        s = strtok(NULL, " \t\n");
        if (strcmp(firstPiece,"fbase") == 0)
          strcpy(outstem,s);
        else if (strcmp(firstPiece,"fsuf") == 0)
          strcpy(outsuf,s);
        else if (strcmp(firstPiece,"inpfile") == 0)
          strcpy(datafile,s);
        else if (strcmp(firstPiece,"bits") == 0)
          nbits = atoi(s);
        else if (strcmp(firstPiece,"fftlen") == 0)
          fftlen = atol(s);
        else if (strcmp(firstPiece,"numpol") == 0)
          npols = atoi(s);
        else if (strcmp(firstPiece,"firstrec") == 0)
          firstrec = atol(s);
        else if (strcmp(firstPiece,"lastrec") == 0)
          lastrec = atol(s);
        else if (strcmp(firstPiece,"ignorefirst") == 0)
          ignorefirst = (strcmp(s,"yes") == 0) || (strcmp(s,"true") == 0)
                                               || (strcmp(s,"1") == 0);
        else if (strcmp(firstPiece,"nfftave") == 0)
          nfftave = atoi(s);
        else if (strcmp(firstPiece,"numskip") == 0)
          nskip = atoi(s);
        else if (strcmp(firstPiece,"samplestoadd") == 0)
          nadd = atoi(s);
        else if (strcmp(firstPiece,"zerofill") == 0)
          zerofill = (strcmp(s,"yes") == 0) || (strcmp(s,"true") == 0)
                                            || (strcmp(s,"1") == 0);
      }
    }
  }
  fclose(fpin);

  if (nbits != 1 && nbits != 2 && nbits != 4 && nbits != 8 && nbits != 12) {
    printf("ERROR: Need bits = 1, 2, 4, 8, or 12 in input file\n");
    exit(0);
  }

  if (npols != 1 && npols != 2) {
    printf("ERROR: Need numpol = 1 or 2 in input file\n");
    exit(0);
  }

  if (fftlen <= 0 || nfftave <= 0) {
    printf("ERROR: Need fftlen, nfftave > 0 in input file\n");
    exit(0);
  }

  /* Open the datafile */

  if ((fpdata = fopen(datafile,"r")) == NULL) {
    printf("Can't open datafile %s for input.\n", datafile);
    exit(0);
  }

  /* Note the initial position at the beginning of the datafile*/

    fgetpos(fpdata, &saveDatafilePos);  /* save position at start of scan */

  /* Get a name for a temporary output file and construct the name
     of the real output file, but don't open these two files yet:
     wait for as long as possible in case there are input problems */ 

  mkstemp(tempoutfile);
  if (strcmp(outsuf,"") == 0)
    sprintf(outfile, "%s_%ld.azel", outstem, fftlen);
  else {
    sprintf(outfile, "%s_%ld.azel.%s", outstem, fftlen, outsuf);
    outsuf_int = atoi(outsuf);
  }

  /* Compute how many bytes each real sample takes up in the datafile */

  bytesPerRealSample = (nbits == 12) ? 2.0 : nbits/8.0;

  /* Open the ephemeris file(s) */
  
  if (ephfile) {
    if ((fpeph = fopen(ephfile,"r")) == NULL) {
      printf("Can't open %s for ephemeris input.\n", ephfile);
      fclose(fpdata);
      exit(0);
    }
  }
  if (eph2file) {
    if ((fpeph2 = fopen(eph2file,"r")) == NULL) {
      printf("Can't open %s for ephemeris input.\n", eph2file);
      fclose(fpeph);
      fclose(fpdata);
      exit(0);
    }
  }

  /* Check that the datafile contains data and that the records */
  /* are structured as we expected                              */

  aohdr = (HDRAO_STD *) hdr;
  hdr_len = MAX_HDR_LEN;

  if (fread(hdr, 1, hdr_len, fpdata) <= 0) {
    printf("\nERROR: No data in file %s\n", datafile);
    fclose(fpdata);
    if (ephfile)
      fclose(fpeph);
    if (eph2file)
      fclose(fpeph2);
    exit(0);
  }

  /* Initialize a buffer which will hold the data portion of each record */

  rec_len = 60000;                /* typical, but anything > 0 will do here */
  buf = (char *) malloc(rec_len);

  /* Get user input */

  if (mode == 2) {
    askstring("Target name (spaces OK): ", target, sizeof(target), 1);
    printf("\n");
    for (i=0; i<n_known_targets; i++) {
      if (!strncasecmp(target,known_target[i],strlen(target))) {
	diameter = known_diameter[i];
	period = known_period[i]*24.0;
	jd0 = known_jd0[i];
	phase0 = 0.0;
	break;
      }
    }
    if (i == n_known_targets) { /* no match */
      printf("target diameter (km): ");
      scanf("%lf", &diameter);
      printf("\ntarget rotation period (hr): ");
      scanf("%lf", &period);
      printf("\nDefine rotation phase by giving a phase and the corresponding Julian date: ");
      scanf("%lf %lf", &phase0, &jd0);
      printf("\n");
    }
    if (jd0 < 1.0)
      jd0 = -1.0;
    
    /* some configs I use frequently. -GB */
    switch (setup) {
    case 1:
      /* Galilean moons, Titan */
      srate = 62500.0;
      lambda = 0.1259632;
      nhops = 4;
      hop1 = 0;
      dwell = 10;
      frstep = 10000.0;
      freq1 = -15000.0;
      txoff = 0.0;
      break;
      
    case 2:
      /* Iapetus */
      srate = 62500.0;
      lambda = 0.1259632;
      nhops = 4;
      hop1 = 0;
      dwell = 10;
      frstep = 2000.0;
      freq1 = -3000.0;
      txoff = 0.0;
      break;
      
    default:
      printf("sample rate (Hz): ");
      scanf("%lf", &srate);
      printf("\nwavelength (m): ");
      scanf("%lf", &lambda);
      printf("\nnhops, first hop: ");
      scanf("%d %d", &nhops, &hop1);
      printf("\nhop dwell (sec): ");
      scanf("%lf", &dwell);
      printf("\nTX offset (Hz): ");
      scanf("%lf", &txoff);
      printf("\nfreq step, first freq offset (Hz): ");
      scanf("%lf %lf", &frstep, &freq1);
      printf("\n");
      break;
    }

    printf("First, last scan to process (numbered from 1): ");
    scanf("%d %d", &firstScan, &lastScan);
    nscans_use = lastScan - firstScan + 1;
    if (nscans_use > MAX_NSCANS_USE) {
      printf("ERROR: Too many scans, must increase MAX_NSCANS_USE in stripAzEl.c\n");
      exit(0);
    }
    if (strcmp(outsuf,"") != 0) {
      if (firstScan == lastScan && firstScan != outsuf_int) {
          printf("\nWARNING: output file suffix = %s even though input scan = %d\n",
                 outsuf, firstScan);
      } else if (firstScan != lastScan) {
          printf("\nWARNING: output file suffix = %s despite multiple input scans\n",
                 outsuf);
      }
    }
    for (iscan = firstScan-1; iscan < lastScan; iscan++) {
      sprintf(promptline, "\nScan %d TX power (kW): ", iscan + 1);
      askstring(promptline, pwrLine, sizeof(pwrLine), 1);
      sscanf(pwrLine, "%lf", &pwr[iscan-firstScan+1]);
    }

    printf("\nSpecify Tsys function here (0) "
              "or use Aug. 2000 fit, scaled for other dates (1)? ");
    scanf("%d", &TsysMethod);
    if (TsysMethod == 0) {
      printf("\nEnter two zenith angles (deg) for system temperature\n"
               "(ZA #2 <= %4.1f --> a single linear Tsys function for all ZA;\n"
               " ZA #2 >  %4.1f --> constant Tsys below %4.1f, then a linear rise): ",
             ZA_KNEE, ZA_KNEE, ZA_KNEE);
      scanf("%lf %lf", &za1, &za2);
      if (za1 < 0) {
        za1 = 0;
        printf("Resetting ZA #1 to 0 deg\n");
      }
      if (za2 > ZA_MAX) {
        za2 = ZA_MAX;
        printf("Resetting ZA #2 to %5.2f deg\n", ZA_MAX);
      }
      printf("\nCh 1 Tsys at both zenith angles (K): ");
      scanf("%lf %lf", &tsys1[0], &tsys1[1]);
      printf("\nCh 2 Tsys at both zenith angles (K): ");
      scanf("%lf %lf", &tsys2[0], &tsys2[1]);
    }
    
    printf("\nnumber of intervals to avoid: ");
    scanf("%d", &ia);

    if (ia > 0)
      printf("\n     AST, start and end in hh:mm:ss");
    for (i=0; i<ia; i++) {
      int hr1, hr2, min1, min2, sec1, sec2;
      printf("\n     interval %d: ",i+1);
      scanf("%d:%d:%d %d:%d:%d",&hr1,&min1,&sec1,&hr2,&min2,&sec2);
      avoid_t1[i] = hr1*3600+min1*60+sec1;
      avoid_t2[i] = hr2*3600+min2*60+sec2;

      /* Put avoidance intervals which straddle midnight into the range     */
      /* [43200,129600) sec (noon to noon); as a first guess, put all other */
      /* intervals into the range [0,86400) sec (midnight to midnight)      */

      avoid_t1[i] = avoid_t1[i] - 86400*floor(avoid_t1[i]/86400.0);
      avoid_t2[i] = avoid_t2[i] - 86400*floor(avoid_t2[i]/86400.0);
      if (avoid_t2[i] < avoid_t1[i])
        avoid_t2[i] += 86400;
    }

    if (!ephfile) {
      printf("\nround trip light time (sec): ");
      scanf("%lf", &rtt1);
    }

    printf("\n");

  }

  /* Loop through the scans */

  printf("\n");
  nscans = 0;     /* total # of scans within data file */
  iscan = -1;     /* processed scan # within data file (0, 1, 2, ...) */

  do {

    /* Check that the header and data are as expected */

    previous_hdr_len = hdr_len;
    hdr_len = SWAB4(aohdr->hdrlen);
    if (hdr_len > MAX_HDR_LEN) {
      printf("Header length = %d, expected no more than %d.\n",
              hdr_len, MAX_HDR_LEN);
      printf("--> Must increase MAX_HDR_LEN in stripAzEl.c\n\n");
      printf("stripAzEl terminated\n\n");
      fclose(fpdata);
      if (ephfile)
        fclose(fpeph);
      if (eph2file)
        fclose(fpeph2);
      free(buf);
      exit(0);
    }

    previous_rec_len = rec_len;
    rec_len = SWAB4(aohdr->reclen) - SWAB4(aohdr->hdrlen);
    if (rec_len != previous_rec_len) {
      free(buf);
      buf = (char *) malloc(rec_len);
    }

    /* Go back to the last saved datafile position, reread data records
       to get back to the start of this scan, update the file position
       pointer to this starting position, and read the first header
       (use fsetpos for going backwards, since large files break fseek) */

    cur_scan = SWAB4(aohdr->scanNumber);
    if (fsetpos(fpdata, &saveDatafilePos) != 0) {      /* back up to last saved position */
      printf("ERROR trying to move backwards to the start of scan %ld.\n", cur_scan);
      printf("stripAzEl terminated\n\n");
      fclose(fpdata);
      if (ephfile)
        fclose(fpeph);
      if (eph2file)
        fclose(fpeph2);
      if (fptempout) {
        fclose(fptempout);
        remove(tempoutfile);
      }
      if (fpout)
        fclose(fpout);
      free(buf);
      exit(0);
    }
    for (i=0; i<(recsInScan % 100); i++) {             /* read forward to start of scan */
      fread(hdr, 1, previous_hdr_len, fpdata);
      fread(buf, 1, previous_rec_len, fpdata);
    }
    fgetpos(fpdata, &saveDatafilePos);                 /* save position of start of scan */
    fread(hdr, 1, hdr_len, fpdata);                    /* read header of scan's first record */
    cur_scan = SWAB4(aohdr->scanNumber);
    nscans++;
    recsInScan = 0;

    /* Decide whether to skip this scan or process it */

    if (nscans < firstScan) {

        /* Skip to the end of the scan */

        printf("Skipping scan %9ld  (record length = %d + %d)\n",
               cur_scan, hdr_len, rec_len);
        previousScan = cur_scan;
        do {
            fread(buf, 1, rec_len, fpdata);
            if (++recsInScan % 100 == 0)
              fgetpos(fpdata, &saveDatafilePos);
        } while (fread(hdr, 1, hdr_len, fpdata) > 0 && SWAB4(aohdr->scanNumber) == previousScan);

    } else {

        /* Process scan */

        printf("Reading  scan %9ld  (record length = %d + %d)\n",
               cur_scan, hdr_len, rec_len);

        /* Skip records if requested                                          */
        /*                                                                    */
        /* Keep separate track of records skipped via firstrec vs. numskip:   */
        /* The former are skipped before zerofill (called by drv_cw.sc)       */
        /* truncates the data to an integer number of ffts, the latter after  */
        /*                                                                    */
        /* Don't try to adjust azel info for fractional records skipped, but  */
        /* later we'll use the exact number of samples skipped to work out    */
        /* how many power spectra were output by drv_cw.sc                    */

        recsToSkip = (firstrec - 1) + (int) (nskip*(npols*bytesPerRealSample*2)/rec_len);
        if (recsToSkip > 0) {
          printf("Skipping %d records.\n", recsToSkip);
          for (i=0; i<recsToSkip; i++) {
            fread(buf, 1, rec_len, fpdata);
            if (++recsInScan % 100 == 0)
              fgetpos(fpdata, &saveDatafilePos);
            fread(hdr, 1, hdr_len, fpdata);
          }
          if (SWAB4(aohdr->scanNumber) != cur_scan) {
            printf("ERROR: skipped more data records/samples than exist for scan %ld.\n", cur_scan);
            printf("stripAzEl terminated\n\n");
            fclose(fpdata);
            if (ephfile)
              fclose(fpeph);
            if (eph2file)
              fclose(fpeph2);
            if (fptempout) {
              fclose(fptempout);
              remove(tempoutfile);
            }
            if (fpout)
              fclose(fpout);
            free(buf);
            exit(0);
          }
        }
        recsInScan = recsToSkip;

        /* If this is the first scan processed, open the temporary output file
           and the real output file and write the header to the real output file */

        if (nscans == firstScan) {

          /* Get date info for the output file header */

          year = SWAB4(aohdr->date) / 1000;
          doy = SWAB4(aohdr->date) - year*1000;
          doyToCal(year, doy, &month, &day);
          secsAfterMidnight = SWAB4(aohdr->time);
          hr = secsAfterMidnight/3600;
          min = secsAfterMidnight/60 - 60*hr;
          sec = secsAfterMidnight - 3600*hr - 60*min;
          jd = JulianDate(year, month, day, hr+4, min, 1.0*sec);
          if (TsysMethod == 1 && S_tsys(0.0, jd, 1, 1) < 0) {
            ASTtoUTC(year, doy, hr, &year_UTC, &month_UTC, &day_UTC, &hr_UTC);
            printf("\nUT date = %4d %s %02d but Tsys fit doesn\'t go back that far.\n",
                    year_UTC, monthName[month_UTC-1], day_UTC);
            printf("--> Must specify Tsys function here or extend S_tsys function in tandgain.c\n\n");
            printf("stripAzEl terminated\n\n");
            fclose(fpdata);
            if (ephfile)
              fclose(fpeph);
            if (eph2file)
              fclose(fpeph2);
            free(buf);
            exit(0);
          }
          if (jd0 <= 0)
            jd0 = jd;

          /* Open the temporary output file and the real output file,
             and mark the position at the start of both output files   */

          sprintf(commandLine, "touch %s", tempoutfile);
          system(commandLine);
          if ((fptempout = fopen(tempoutfile,"r+")) == NULL) {
            printf("Can't open %s for temporary output.\n", tempoutfile);
            fclose(fpdata);
            if (ephfile)
              fclose(fpeph);
            if (eph2file)
              fclose(fpeph2);
            exit(0);
          }
          fgetpos(fptempout, &tempoutStartPos);

          if ((fpout = fopen(outfile,"w")) == NULL) {
            printf("Can't open %s for output.\n", outfile);
            fclose(fpdata);
            if (ephfile)
              fclose(fpeph);
            if (eph2file)
              fclose(fpeph2);
            fclose(fptempout);
            remove(tempoutfile);
            exit(0);
          }

          /* Write the output header (to the real output file) */

          if (mode == 1) {
      
              /* output includes parallactic angle */

	    fprintf(fpout, "# ID: %8.8s\n# VERS: %4.4s\n", aohdr->id, aohdr->version);
	    fprintf(fpout, "# DATE: %d %d\n", year, doy);
	    fprintf(fpout, "# REC   TIME  AZIMUTH   ZENITH  PARALLA\n");

          } else {
      
              /* output to be read by psd2rdf or dehop.pro */

              fprintf(fpout, "DATE    %d %d %d\n", year, month, day);
              fprintf(fpout, "TARGET  %s %.3f %.6f %.5f %.2f\n", target, diameter, period, jd0, phase0);
              fprintf(fpout, "HOP     %d %d %.3f %.3f %.1f %.3f\n", nhops, hop1, frstep, freq1, dwell, txoff);
              fprintf(fpout, "SRATE   %.1f\n", srate);
              fprintf(fpout, "WVLNGTH %.7f\n", lambda);
              if (eph2file)
                fprintf(fpout, "EPHEM   %s corrected to %s\n", ephfile, eph2file);
          }
        }

        /* Initialize various quantities for this scan */

        iscan++;
        ihop = hop1;                                      /* hop color (0, 1, 2, ...) for this dwell */
        dt = rec_len/(npols*bytesPerRealSample*2*srate);  /* time interval per data record */
        minRecsInDwell = (int) dwell/dt;                  /* number of records per dwell   */
        recsUsedInScan = 0;                               /* number of records used so far in this scan */
        ndwells_out = 0;                                  /* number of dwells for which output has been written so far */
        n = 0;                                            /* number of records so far in this dwell */

        /* Go through each record in this scan                            */
        /* (even if lastrec gives a cutoff -- we'll deal with that later) */

        previousScan = cur_scan;

        do {

          /* Get scan #, azimuth, zenith angle, and time from the record header */

          fread(buf, 1, rec_len, fpdata);
          recsInScan++;
          recsUsedInScan++;
          recsUsedInFile++;
          scan = SWAB4(aohdr->scanNumber);
          az = SWAB4(aohdr->azTTD) / 10000.0 - 180.0; /* header has feed az, want sky az */
          az = az - 360*floor(az/360);         /* put in range [0,360)            */
          za = SWAB4(aohdr->grTTD) / 10000.0;
          previous_t = t;
          t = SWAB4(aohdr->time) + t_offset;          /* record-end time (seconds after midnight) */

          /* If time seems to be decreasing rather than increasing, either
             we've crossed midnight or else there was a datataking error   */

          if (t < previous_t) {
              if (previous_t - t > 43200) {

                /* Crossed midnight: Use time range [43200,129600) sec (noon to noon) */

                t_offset += 86400;
                t += t_offset;
                for (i=0; i<ia; i++) {
                  avoid_t1[i] = avoid_t1[i] - 86400*floor(avoid_t1[i]/86400.0 - 0.5);
                  avoid_t2[i] = avoid_t2[i] - 86400*floor(avoid_t2[i]/86400.0 - 0.5);
                }

            } else {

                /* On 2001 Mar 28 there were some write time irregularities, with
                   times going backwards by a few seconds.  This is such a case.  */

                printf("     WARNING: Write times go backwards at %ld secs after midnight AST\n",
                       (long)(previous_t+0.5));
            }
          }

          /* If this is the first record that we'll use in this scan, initialize the
             time at which the dwell starts to the time at the start of this record  */

          if (recsUsedInScan == 1) {
            t_start = t;
            t_end = t_start + dwell;
          }

          /* Get ephemeris information for this data record:
             RA, dec, RTT, and (if requested) delay correction and Doppler correction */

          if (!ephfile) {

              /* no ephemeris file: use same user-specified RTT for all records
                                    and assign dummy values for RA and dec      */

              rtt = rtt1;
              ra = 99.99999;
              dec = 999.9999;

          } else {

              /* First get the UTC date and time of the start of the scan */

              hr = t/3600;
              min = t/60 - 60*hr;
              sec = t - 3600*hr - 60*min;
              ASTtoUTC(year, doy, hr, &year_UTC, &month_UTC, &day_UTC, &hr_UTC);

              /* Find the corresponding line in the ephemeris file         */
              /*                                                           */
              /* Make a search string including date, hour, minutes;       */
              /* don't include seconds, since at least one ephemeris file  */
              /* (for 253 Mathilde) has entries NOT at even 1-minute ticks */ 

              sprintf(ephString,"%4d-%02d-%02d %02d:%02d:", year_UTC, month_UTC, day_UTC, hr_UTC, min);

              /* Read through the ephemeris file until the correct line is reached */
              /*                                                                   */
              /* Keep track of two lines at a time (for linear interpolation)      */
            
              while (strcmp(ephString, dateHrMin_str) != 0 && !feof(fpeph)) {
                strcpy(dateHrMin_str, next_dateHrMin_str);
                ephseconds = next_ephseconds;
                ephrtt = next_ephrtt;
                ephdop = next_ephdop;
                ephra = next_ephra;
                ephdec = next_ephdec;

                /* Read another line and extract date/time, RA, dec, RTT, and Doppler */

                GetLine(fpeph, ephLine, MAX_EPH_LEN);
                if (ephLine[0] != '#') {
                  Substring(next_dateHrMin_str, ephLine, 1, 17, TRUE);
                  Substring(ephseconds_str, ephLine, 18, 2, TRUE);
                  next_ephseconds = atoi(ephseconds_str);
                  Substring(rahour, ephLine, 33, 2, TRUE);
                  Substring(ramin, ephLine, 35, 2, TRUE);
                  Substring(rasec, ephLine, 37, 5, TRUE);
                  next_ephra = atoi(rahour) + atoi(ramin)/60. + atof(rasec)/3600.;
                  Substring(decsign, ephLine, 43, 1, TRUE);
                  Substring(decdeg, ephLine, 44, 2, TRUE);
                  Substring(decmin, ephLine, 46, 2, TRUE);
                  Substring(decsec, ephLine, 48, 4, TRUE);
                  next_ephdec = atoi(decdeg) + atoi(decmin)/60. + atof(decsec)/3600.;
                  if (strcmp(decsign,"-") == 0)
                    next_ephdec = -next_ephdec;
                  Substring(ephrtt_str, ephLine, 53, 15, TRUE);
                  next_ephrtt = atof(ephrtt_str);
                  Substring(ephdop_str, ephLine, 69, 16, TRUE);
                  next_ephdop = atof(ephdop_str);
                }
              }
               
              /* Let user know if this ephemeris file doesn't contain that starting date and time */

              if (strcmp(ephString, dateHrMin_str) != 0 || ephLine[0] == '#') {
                if (strcmp(ephString, dateHrMin_str) != 0)
                  printf("\nERROR: Starting date/time %s for scan %ld is not included in file %s\n",
                         ephString, scan, ephfile);
                else
                  printf("\nERROR: Record after starting date/time %s for scan %ld is not included in file %s\n",
                         ephString, scan, ephfile);
                fclose(fpdata);
                fclose(fpeph);
                if (eph2file)
                  fclose(fpeph2);
                fclose(fptempout);
                remove(tempoutfile);
                fclose(fpout);
                free(buf);
                exit(0);
              }

              /* Since the ephemeris predictions are given at 1-minute ticks,  */
              /* use linear interpolation to get parameters for this date/time */

              interpolate = (sec - ephseconds)/60.0;
              rtt = ephrtt + interpolate*(next_ephrtt - ephrtt);
              dop = ephdop + interpolate*(next_ephdop - ephdop);
              delta_ra = next_ephra - ephra;
              delta_ra = delta_ra - 24*floor(delta_ra/24.0 + 0.5);   /* range [-12,12) */
              ra = ephra + interpolate*delta_ra;
              ra = ra - 24*floor(ra/24.0);                           /* range [0,24)   */
              dec = ephdec + interpolate*(next_ephdec - ephdec);

              /* Use the updated ephemeris file, if specified, */
              /* to get delay and Doppler corrections          */

              if (!eph2file) {
                  delcorr = 0.0;
                  dopcorr = 0.0;
              } else {
                  while (strcmp(ephString, dateHrMin2_str) != 0 && !feof(fpeph2)) {
                    strcpy(dateHrMin2_str, next_dateHrMin2_str);
                    eph2seconds = next_eph2seconds;
                    eph2rtt = next_eph2rtt;
                    eph2dop = next_eph2dop;
                    GetLine(fpeph2, ephLine, MAX_EPH_LEN);
                    if (ephLine[0] != '#') {
                      Substring(next_dateHrMin2_str, ephLine, 1, 17, TRUE);
                      Substring(ephseconds_str, ephLine, 18, 2, TRUE);
                      next_eph2seconds = atoi(ephseconds_str);
                      Substring(ephrtt_str, ephLine, 53, 15, TRUE);
                      next_eph2rtt = atof(ephrtt_str);
                      Substring(ephdop_str, ephLine, 69, 16, TRUE);
                      next_eph2dop = atof(ephdop_str);
                    }
                  }
                  if (strcmp(ephString, dateHrMin2_str) != 0 || ephLine[0] == '#') {
                    if (strcmp(ephString, dateHrMin2_str) != 0)
                      printf("\nERROR: Starting date/time %s for scan %ld is not included in file %s\n",
                             ephString, scan, eph2file);
                    else
                      printf("\nERROR: Record after starting date/time %s for scan %ld is not included in file %s\n",
                             ephString, scan, eph2file);
                    fclose(fpdata);
                    fclose(fpeph);
                    fclose(fpeph2);
                    fclose(fptempout);
                    remove(tempoutfile);
                    fclose(fpout);
                    free(buf);
                    exit(0);
                  }
                  interpolate = (sec - eph2seconds)/60.0;
                  delcorr = eph2rtt + interpolate*(next_eph2rtt - eph2rtt) - rtt;
                  dopcorr = eph2dop + interpolate*(next_eph2dop - eph2dop) - dop;
                  dop += dopcorr;
              }
          }      /* end getting ephemeris information for this data record */

          /* Calculate transmit zenith angle and azimuth for this data record */

          el = 90.0 - za;
          cosel = cos (el * dtor);
          sinel = sin (el * dtor);
          cosaz = cos (az * dtor);
          sinaz = sin (az * dtor);
          coslat = cos (ARECIBO_LAT * dtor);
          sinlat = sin (ARECIBO_LAT * dtor);
          ha = atan2 (-cosel * sinaz, coslat * sinel - sinlat * cosel * cosaz);
          decl = asin (sinlat * sinel + coslat * cosel * cosaz);
          ha = ha - (15.0 * rtt / 3600.0) * dtor;
          el = asin (sinlat * sin (decl) + coslat * cos (decl) * cos (ha)) / dtor;
          sinaz = -cos(decl) * sin(ha) / cos(el * dtor);
          cosaz = (sin(decl) * coslat - cos(decl) * sinlat * cos(ha)) / cos(el * dtor);
          za_tx = 90.0 - el;      
          az_tx = atan2(sinaz, cosaz) / dtor;

          /* Write output, if any, for this data record */

          if (mode == 1) {

              /* output for every data record; includes parallactic angle */

              if (lastrec <= 0 || recsInScan <= lastrec) {
                pa = atan2(sin(az*dtor), cos(az*dtor)*cos(za*dtor) - tanlat*sin(za*dtor)) / dtor;
                fprintf(fpout, "%6d  %7.1f  %7.3f  %7.3f  %7.3f\n", recsUsedInScan-1, t, az, za, pa);
              }

          } else {

              /* output to be read by psd2rdf or dehop.pro */

              /* Compute this data record's contribution to the appropriate dwell */

              if (t >= t_start && t <= t_end) {
    
                /*  Don't use this dwell if any record in it lies within
                    an avoidance interval, or if the target is out of the
                    beam at any time during the dwell                      */

                for (j=0; j<ia; j++)
                  if (avoid_t1[j] <= t && t <= avoid_t2[j])
                    no_use = 1;
                if (za >= ZA_MAX || za_tx >= ZA_MAX)
                  no_use = 1;

                /* Build up min/mean/max quantities for this dwell */

                az1_min = (az1_min < az) ? az1_min : az;
                az1_max = (az1_max > az) ? az1_max : az;
                az1_mean += az;
                az2 = az - 360*floor(az/360.0 - 0.5);  /* range [180,540) */
                az2_min = (az2_min < az2) ? az2_min : az2;
                az2_max = (az2_max > az2) ? az2_max : az2;
                az2_mean += az2;
                za_mean += za;
                pwr_mean += pwr[iscan];
                rtt_mean += rtt;
                dop_mean += dop;
                ra1_min = (ra1_min < ra) ? ra1_min : ra;
                ra1_max = (ra1_max > ra) ? ra1_max : ra;
                ra1_mean += ra;
                ra2 = ra - 24*floor(ra/24.0 - 0.5);    /* range [12,36) */
                ra2_min = (ra2_min < ra2) ? ra2_min : ra2;
                ra2_max = (ra2_max > ra2) ? ra2_max : ra2;
                ra2_mean += ra2;
                dec_mean += dec;
                delcorr_mean += delcorr;
                dopcorr_mean += dopcorr;

                if (TsysMethod == 0) {
                    /* use user input values */
                    tsys_mean[0] += S_tsys_user(za, tsys1[0], tsys1[1], za1, za2);
                    tsys_mean[1] += S_tsys_user(za, tsys2[0], tsys2[1], za1, za2);
                } else {
                    /* use August 2000 mapped and hardcoded values */
                    tsys_mean[0] += S_tsys(za, jd, 1, 0);
                    tsys_mean[1] += S_tsys(za, jd, 2, 0); 
                }

                /*  S_gain expects source azimuth
                    Fourth argument is 1 for transmit gain, 0 for receive gain  */

                gain_mean += S_gain(za, az, jd, 0) * S_gain(za_tx, az_tx, jd, 1);

                n++;
              }

              /* If we've gone beyond the end of this dwell, write an output record.
                 The test below uses "t >= t_end-1" rather than "t >= t_end" because
                 we can't be too picky: for one-second records, the record-end time
                 in the header is sometimes in fact the record-START time.           */

              if (n >= minRecsInDwell && t >= t_end-1) {

                /* Write a record for this dwell to the temporary output file
                   if the dwell is complete (not truncated by the end of the scan) */

                if (no_use) /* bad spectrum */
                  pwr_mean = 0.001*n;  /* negligible but not 0 */
                az1_range = az1_max - az1_min;
                az2_range = az2_max - az2_min;
                if (az2_range < az1_range)
                  az_mean = (az2_mean/n) - 360*floor( (az2_mean/n) /360.0 );
                else
                  az_mean = az1_mean/n;
                ra1_range = ra1_max - ra1_min;
                ra2_range = ra2_max - ra2_min;
                if (ra2_range < ra1_range)
                  ra_mean = (ra2_mean/n) - 24*floor( (ra2_mean/n) /24.0 );
                else
                  ra_mean = ra1_mean/n;
                fprintf(fptempout, "%6d %7.1f %3d  %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %8.2f %8.5f %8.4f",
                        ndwells_out++, t_start, ihop, az_mean, za_mean/n,
                        tsys_mean[0]/n, tsys_mean[1]/n, 
                        gain_mean*gfactor*gfactor/n,
                        pwr_mean/n, rtt_mean/n, ra_mean, dec_mean/n);
                if (eph2file)
                  fprintf(fptempout, " %11.3f %11.5f", 1.0e6*delcorr_mean/n, dopcorr_mean/n);
                fprintf(fptempout, "\n");
                fflush(fptempout);
                t_start += dwell;
                t_end += dwell;
                ihop = (ihop + 1) % nhops;
                az1_mean = az2_mean = za_mean = tsys_mean[0] = tsys_mean[1] 
                         = gain_mean = pwr_mean = rtt_mean
                         = ra1_mean = ra2_mean = dec_mean
                         = delcorr_mean = dopcorr_mean = 0.0;
                az1_min = az2_min = ra1_min = ra2_min = 999.9;
                az1_max = az2_max = ra1_max = ra2_max = -999.9;
                no_use = 0;
                n = 0;
              }
          }         /* finished with output for this data record */

          /* Every 100th data record, update the file position pointer
             to the current position (doing it every record is too slow) */

          if (recsInScan % 100 == 0)
            fgetpos(fpdata, &saveDatafilePos);

          /* Read next data record and loop back */

        } while (fread(hdr, 1, hdr_len, fpdata) > 0 && SWAB4(aohdr->scanNumber) == previousScan);

        /* Finished processing all data records in this scan */

        /* If we're writing one output record per dwell then we have to check how many
           dwells drv_cw.sc would have written for this scan, add dummy records if
           needed so that we have one output record per dwell, and copy the correct
           number of records from the temporary output file to the real output file    */

        if (mode == 2) {

          /* Write the last dwell in the scan to the temporary output file
             if the dwell is complete (not truncated by the end of the scan) */

          if (n >= minRecsInDwell) {
            if (no_use) /* bad spectrum */
              pwr_mean = 0.001*n;  /* negligible but not 0 */
            az1_range = az1_max - az1_min;
            az2_range = az2_max - az2_min;
            if (az2_range < az1_range)
              az_mean = (az2_mean/n) - 360*floor( (az2_mean/n) /360.0 );
            else
              az_mean = az1_mean/n;
            ra1_range = ra1_max - ra1_min;
            ra2_range = ra2_max - ra2_min;
            if (ra2_range < ra1_range)
              ra_mean = (ra2_mean/n) - 24*floor( (ra2_mean/n) /24.0 );
            else
              ra_mean = ra1_mean/n;
            fprintf(fptempout, "%6d %7.1f %3d  %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %8.2f %8.5f %8.4f",
                    ndwells_out++, t_start, ihop, az_mean, za_mean/n,
                    tsys_mean[0]/n, tsys_mean[1]/n, 
                    gain_mean*gfactor*gfactor/n,
                    pwr_mean/n, rtt_mean/n, ra_mean, dec_mean/n);
            if (eph2file)
              fprintf(fptempout, " %11.3f %11.5f", 1.0e6*delcorr_mean/n, dopcorr_mean/n);
            fprintf(fptempout, "\n");
            fflush(fptempout);
            t_start += dwell;
            ihop = (ihop + 1) % nhops;
          }
          az1_mean = az2_mean = za_mean = tsys_mean[0] = tsys_mean[1] 
                   = gain_mean = pwr_mean = rtt_mean
                   = ra1_mean = ra2_mean = dec_mean
                   = delcorr_mean = dopcorr_mean = 0.0;
          az1_min = az2_min = ra1_min = ra2_min = 999.9;
          az1_max = az2_max = ra1_max = ra2_max = -999.9;
          no_use = 0;
          n = 0;

          /* Work out the number of power spectra (one spectrum per dwell) that
             drv_cw.sc would have output for this scan; use double-precision
             rather than integer math to get the number of samples in case this
             number exceeds the maximum long-integer value (LONG_MAX).          */
        
          if (lastrec <= 0 || recsInScan < lastrec)
            nrecs = recsInScan - (firstrec - 1);
          else
            nrecs = lastrec - (firstrec - 1);
          samplesPerRecord_double = rec_len/(npols*bytesPerRealSample*2);
          nsamples_double = nadd + floor(nrecs*samplesPerRecord_double + 0.5);
          if (zerofill)
            nsamples_double = (1.0*fftlen) * ceil(nsamples_double/fftlen);      /* zerofill    */
          else
            nsamples_double = (1.0*fftlen) * ((long) (nsamples_double/fftlen)); /* zerofill -n */
          nffts = (long) ((nsamples_double - nskip)/fftlen);                    /* fftfilter   */
          ndwells = nffts/nfftave;                                              /* avgdata     */

          /* Write dummy record(s) for the last dwell(s) of this scan to the
             temporary output file if needed to keep in synch with the output of drv_cw.sc   */

          if (ndwells_out < ndwells)
            ndummydwells = ndwells - ndwells_out;
          else
            ndummydwells = 0;
          for (i=0; i<ndummydwells; i++) {
            fprintf(fptempout, "%6d %7.1f %3d  %7.3f %7.3f %7.3f %7.3f %7.3f %7.3f %8.2f %8.5f %8.4f",
                    ndwells_out++, t_start, ihop, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
            if (eph2file)
              fprintf(fptempout, " %11.3f %11.5f", 0.0, 0.0);
            fprintf(fptempout, "\n");
            fflush(fptempout);
            t_start += dwell;
            ihop = (ihop + 1) % nhops;
          } 

          /* Copy the correct number of output records (dwells) from the temporary
             output file to the real output file, then rewind the temporary output file */

          if (fsetpos(fptempout, &tempoutStartPos) != 0) {
            printf("ERROR trying to rewind temporary output file %s.\n", tempoutfile);
            printf("stripAzEl terminated\n\n");
            fclose(fpdata);
            if (ephfile)
              fclose(fpeph);
            if (eph2file)
              fclose(fpeph2);
            fclose(fptempout);
            remove(tempoutfile);
            fclose(fpout);
            free(buf);
            exit(0);
          }
          for (i=0; i<ndwells; i++) {
            GetLine(fptempout, outLine, 255);
            fprintf(fpout, "%s\n", outLine);
          }
          fsetpos(fptempout, &tempoutStartPos);

          /* Let the user know that the scan has been completely processed */

          if (ndummydwells > 0) {
              printf("Wrote records for %d dwells in scan %9ld  (last %d are dummy records)\n",
                     ndwells, cur_scan, ndummydwells);
          } else {
              printf("Wrote records for %d dwells in scan %9ld\n", ndwells, cur_scan);
          }
          ndwells_tot += ndwells;

        }   /* if mode == 2 */

    }  /* finished skipping or processing this scan */

    /* Loop back and read the next scan if we're not done */

  } while (!feof(fpdata) && nscans < lastScan);

  /*  Close all files and delete the temporary output file  */

  fclose(fpdata);
  if (ephfile)
    fclose(fpeph);
  if (eph2file)
    fclose(fpeph2);
  fclose(fptempout);
  remove(tempoutfile);
  fclose(fpout);
  free(buf);
  
  /*  Print an error message if no scans were processed  */

  if (nscans < firstScan) {
    printf("ERROR: There are only %d scans in file %s, so none were processed\n",
           nscans, datafile); 
    exit(0);
  }

  /*  If the "-d" argument was given, adjust the hop bandwidth, first hop frequency,
      and transmit offset for Doppler shift, averaged over all records for all
      processed scans; move the real output file to the temporary output file, then
      copy it (with the "HOP" header line adjusted) back to the real output file.     */

  if (apply_doppler && mode == 2) {

    sprintf(commandLine, "\\mv %s %s", outfile, tempoutfile);
    system(commandLine);

    if ((fptempout = fopen(tempoutfile,"r")) == NULL) {
      printf("Can't open %s for final copy operation.\n", tempoutfile);
      exit(0);
    }

    if ((fpout = fopen(outfile,"w")) == NULL) {
      printf("Can't open %s for final copy operation.\n", outfile);
      fclose(fptempout);
      remove(tempoutfile);
      exit(0);
    }

    dop_mean /= recsUsedInFile;
    dopfactor = 1 + dop_mean/SBAND_FREQ;

    outlen = (eph2file) ? 6 + ndwells_tot : 5 + ndwells_tot;
    for (i=0; i<2; i++) {
      GetLine(fptempout, outLine, 255);
      fprintf(fpout, "%s\n", outLine);
    }
    GetLine(fptempout, outLine, 255);
    fprintf(fpout, "HOP     %d %d %.3f %.3f %.1f %.3f\n",
            nhops, hop1, frstep*dopfactor, freq1*dopfactor, dwell, txoff*dopfactor);
    for (i=3; i<outlen; i++) {
      GetLine(fptempout, outLine, 255);
      fprintf(fpout, "%s\n", outLine);
    }

    fclose(fptempout);
    remove(tempoutfile);
    fclose(fpout);

  }

  printf("\n");
  exit(0);

}

/* printhdrV - print asci version of vme data taking hdrs               */
/*
modification history:
--------------------
xx.x 21jan93  copied from printhdr
xx.x 10jun94  changed to hdrSection format, added riProg section 
xx.x 11jun94  added  spsBg section
xx.x 05sep99  recompile to add  pnt section
*/
/*
*DESCRIPTION
*Filter to read in Vme headers, format them, and then output them to a file
*descriptor. It will output the standard header and all header sections that
*it knows about. If an unknown header section is found, a one line msg
*is also output to the file ptr.
* the routine expects the binary headers in the big endian format.
*(it swaps the data before printing).
*
*The call is:
*
*       printhdrV  [-h] [-s]
*
*       input is from standard input, output is to standard out
*       -h   help
*       -s   just output standard header
*/

/*      includes        */
#include        <stdio.h>
#include        <stdlib.h>
#include        <string.h>
#include        <memory.h>
#include        <byteswap.h>
#include        <hdrLib.h>
#include        <unistd.h>
#include		<aovmehdrLib.h>
#include        <philLib.h>
#include		<unistd.h>


/*      defines         */

#define    TRUE 1
#define    FALSE 0
#define PRGNAME "printhdrv"

/*      globals         */

char    *secNames[]= { "inp","out","proc","time","pnt","misc"};

/*      prototypes      */

void    processargs(int argc,char **argv,int *pstdHdrOnly); 

int main
        (
          int   argc,
          char  **argv
        )
{
        char       hdr[HDR_MAX_LEN];            /* read into here*/     
        HDRAO_STD  *phdrStd;
        char    *psec;                  /* ptr to sections*/
        int     inbytes;

        int     hdrlenStd,hdrlen;                       /* for header*/
        int     bytesLeftHdr;   /* prg specific portion of header*/
        int     stdHdrOnly;     /* only output std header*/     
        
        stdHdrOnly=FALSE;
        processargs(argc,argv,&stdHdrOnly);

        hdrlenStd=sizeof(HDRAO_STD);
        phdrStd=(HDRAO_STD*)hdr;
        for (;;){
           /*
            * get standard hdr. this will also tell us the complete hdr len
           */
           inbytes=fread(hdr,sizeof(char),hdrlenStd,stdin);
           if (inbytes < hdrlenStd) break;
           if (!hdrIsHdr((char*)hdr)){
              fprintf(stderr,"%s: bad header. marker: %4s\n",
               PRGNAME,phdrStd->hdrMarker);
              goto errout;
           }
           /* 
            * read rest of header
           */
		   hdrlen=bswap_32(phdrStd->hdrlen);
           bytesLeftHdr=hdrlen - hdrlenStd;
           if (bytesLeftHdr > 0) {
                psec=hdr + hdrlenStd;
                if (bytesLeftHdr <= (HDR_MAX_LEN - hdrlenStd)){
                   inbytes=fread(psec,sizeof(char),bytesLeftHdr,stdin);
                   if (inbytes < bytesLeftHdr) break;
                }
                else {
                   fprintf(stderr,
                        "%s: prog specific hdrlen too long:%d. maxBuf:%d\n",
                    PRGNAME,bytesLeftHdr,HDR_MAX_LEN-hdrlenStd);
                    goto errout;
                }
           }
           /*
            * output the  hdr
           */
		   swaphdr(phdrStd);
           hdrPrAll(stdout,phdrStd);
           fprintf(stdout,"\n");
                                
        }
        exit(0);
errout: exit(-1);
        /*NOTREACHED*/
}
/******************************************************************************/
/*    process args                                                            */
/******************************************************************************/
void    processargs
        (
         int    argc,
         char   **argv,
         int    *pstdHdrOnly    /* only output std header*/
        ) 
{
/*
        function to process a programs input command line.
        This is a template that can be customized for individual programs
        To use it you should:

        - pass in the parameters that may be changed.
        - edit the case statement below to correspond to what you want.
        - stdio.h must be added for this routine to work

        Don't forget the ** on the arguments coming in (since you want to 
        pass back data.
*/
        int getopt();                   /* c lib function returns next opt*/ 
        extern int opterr;              /* if 0, getopt won't output err mesg*/

        int c;                          /* Option letter returned by getopt*/
        char  *myoptions = "hs";        /* options to search for. :--> needs
                                            an argument*/
        char *USAGE = "Usage: printhdrV [-h] [-s]< >";

        opterr = 0;                             /* turn off there message*/
/* 
        loop over all the options in list
*/
        while ((c = getopt(argc,argv,myoptions)) != -1){ 
          switch (c) {
          case 'h':
                  fprintf(stderr,"%s\n",USAGE);
                  exit(0);
          case 's':
                  *pstdHdrOnly=TRUE;
                  break;
          case '?':                     /*if c not in myoptions, getopt rets ?*/
             goto errout;
             break;
          }
        }
        return;
/*
        here if illegal option or argument
*/
errout: fprintf(stderr,"%s\n",USAGE);
        exit(1);
}

/*hdrRi.h - include file for riProg portion of headers
*  %W%    %G%
*/
/*modification history:
----------------------
1.1,19aug94 moved under sccs
1.2,19aug94 outstanding
    23nov94 changed itemp[3] to ipp,gw,startOn...
    18jan00 added 1 word at end to make it a multiple of 8 bytes.
*/
#ifndef INChdrRih
#define INChdrRih
#include        <hdrLib.h>
/*
 *      Ri config info for headers
*/
#define RIHDR_IPPNOGWCNT        -1
#define RIHDR_CONT               0
#define RIHDR_IPPANDGWCNT        1

#define RIHDR_USEMUX             1 
#define RIHDR_CYCLEMODE         10 
/*
 *      macros to access header locatoins
*/
#define RIHDR_SUBCYCLEON(a)  ( (((a)/10)%10)==1)
#define RIHDR_USEMUXON(a)      ( ( (a)%10    )==1)
/*
 *      pass in start of header, return ptr to start of riPortion of header
*/
#define RIHDR_GET(phdr)  ( \
         (HDRAO_RI*) ( \
              ((char*)(phdr)) + \
              HDR_S_GET_OFF(((HDRAO_STD *)(phdr))->sec.inp)\
             ))
typedef struct {
/*
 *      here's the ri specific junk
 *      extra space at bottom makes multiple of 8 bytes and can be used
 *      for additions later on
*/
/*      need to add:
        char    id[4];                  "RIPR" no null termination*;
        char    ver[4];                 "xx.x" version...
*/
        int     extTiming;              /* FALSE --> use rtg timing*/
        int     smpMode;                /* -1 ipp no gwcount,0=continuous,
                                           1 ipp with gwCnt*/
        int     packing;                /* 1,2,4,8,12*/
        int     muxAndSubCycle;         /*  0--> none,1 mux,10cyclemode,11both*/
        int     fifoNum;                /* 1,2,12*/
        int     smpPairIpp;             /* smp 1 pair/ipp*/
        int     ippsPerBuf;             /* ipps in 1 buf */
        int     ippNumStartBuf; /* varies each rec.1st ipp of buf. cnt from 1*/ 
        float   ipp;                    /* ipp in usecs .1 usec accuracy*/
        float   gw;                     /* gw in usecs .1 usec accuracy*/
        int     startOn;                /* 0- immediate
                                           1 - 1second tick
                                           10- 10sec tick
                                           99- external tick
                                        */
		int 	free; 			/* added 18jan00*/
        }       HDRAO_RI;
#endif /*INChdrRih*/

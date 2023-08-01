/*hdrSpsBg.h - include file for spsBufGen headers
*	%W%	%G%
*/
/*
current length 3nov94: 53 words or 212bytes
modification history:
--------------------
1.1,19aug94,	put under sccs control
1.2,19aug94,	outstanding
    23aug94,    version 1.2  
	        added baudLen,bwCodeMhz,codeLenUsec
	        added id, added  version
    26sep94,    left old hdr typedefs in .h file
    02feb95,    changed to version 2.0
*/
#ifndef INChdrSpsBgh
#define INChdrSpsBgh
#include	"hdrLib.h"
// #include	"spsBg.h"

#define HDRAO_SPSBG_ID			"SPBG"
#define HDRAO_SPSBG_VERSION 		" 2.0"
#define HDRAO_SPSBG_MAX_RCV_WIN           5
#define HDRAO_SPSBG_MAX_MP_SEQ           20
#define HDRAO_SPSBG_MAX_LEN_CODE_NAME    20

/*
 *      pass in start of header, return ptr to start of spsBg Portion of header
*/
#define HDRAO_SPSBG_GET(phdr)  ( \
         (HDRAO_SPSBG*) ( \
              ((char*)(phdr)) + \
              HDR_S_GET_OFF(((HDRAO_STD *)(phdr))->sec.time)\
             ))

/*
 *      see if it's a SPSBG header section. pass in ptr to spsBg header
*/

#define HDRAO_SPSBG_ISSPSBG(pspsBgHdr)  (strncmp(pspsBgHdr->id,HDRAO_SPSBG_ID,4)== 0)


/*
 *	aeronomy header specification  this is an extension to the 
 *	standard header
*/
typedef struct {
	float	startUsec;	/* usecs since start of rf pulse*/
	int	numSamples;	/* in window*/
	int	numSamplesCal;  /* num cal samples in window*/
	} HDRAO_SPSBG_RCV_WIN;

typedef struct {
	char	id[4];		/* "SPBG" no null terminator*/
	char    ver[4];		/* "xx.x" no null terminator*/
	float 	ipp;		/* ipp in usecs*/
	float 	gw;		/* gw  in usecs*/
	float   baudLen;	/* baudlen in usecs or 0 if no baud*/
	float   bwCodeMhz;	/* 1/baud or bw if no baud*/
	float	codeLenUsec;	/* code length in usecs*/
	float   txIppToRfOn;	/* txipp start to 1st rfOn*/
	float   rfLen;		/* rf on to last rfOff in usecs*/
	int	numRfPulses;    /* num rfPulses*/
	float   mpUnit;		/* multipulse unit in usecs(codeLenUsecs)*/
	int     mpSeq[HDRAO_SPSBG_MAX_MP_SEQ];/* multipulse sequence*/
	char	codeName[HDRAO_SPSBG_MAX_LEN_CODE_NAME];
	int	smpInTxPulse;	/* samples in txPulse */
	int     numRcvWin;	/* number of rcv windows used*/
	HDRAO_SPSBG_RCV_WIN rcvWin[HDRAO_SPSBG_MAX_RCV_WIN];
	} HDRAO_SPSBG;

#define	HDRAO_SPSBG_LEN  sizeof(HDRAO_SPSBG)
#endif /*INChdrSpsBgh*/

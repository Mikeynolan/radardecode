/*
 *	include for aoHdr  conversion routines.
*/
#ifndef INCaovmehdrLibh
#define INCaovmehdrLibh
#include	"phlibExtras.h"
#include	<stdio.h>
#include    "hdrLib.h"
#include    "hdrCor.h"
#include	"hdrDop.h"
#include 	"hdrIfLo.h"
#include	"hdrLib.h"
#include    "hdrPnt.h"
#include    "hdrProc.h"
#include    "hdrRi.h"
#include    "hdrSpsBg.h"

STATUS  hdrIsHdr(char *phdr);
void    hdrPrAll(FILE *fptr,HDRAO_STD *phdr);
void    hdrPrCor(FILE *fp,void *phdr);
void    hdrPrDop(FILE *fptr,void *phdr);
void    hdrPrIfLo(FILE *fptr,void *phdr);
void    hdrPrPnt(FILE *fp,void *phdr);
void    hdrPrProc(FILE *fp,void *phdr);
void    hdrPrRi(FILE *fp,void *phdr);
void    hdrPrSpsBg(FILE *fptr,HDRAO_SPSBG *phdr);
void    hdrPrStd(FILE *fptr,HDRAO_STD *phdr);


void    swaphdr(HDRAO_STD *phdr);
void    swapcorhdr(HDRAO_STD *phdr);
void    swapatmrawhdr(HDRAO_STD *phdr);
void    swapsbrdrhdr(HDRAO_STD *phdr);
void    swaphcor(HDRAO_COR *phdr);
void    swaphdop(HDRAO_DOP *phdr);
void    swaphiflo(HDRAO_IFLO *phdr); 
void    swaphpnt(HDRAO_PNT *phdr);
void    swaphproc(HDRAO_PROC *phdr);
void    swaphri(HDRAO_RI *pri);
void    swaphsps(HDRAO_SPSBG *phdr);
void    swaphstd(HDRAO_STD *phdr);
#endif  /* INCaoHdrLib */

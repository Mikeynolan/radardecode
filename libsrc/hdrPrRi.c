#include	<stdio.h>
#include    <aovmehdrLib.h>
/******************************************************************************/
/*	hdrPrRi 							      */
/******************************************************************************/
void	hdrPrRi(FILE *fptr,void  *phdr)
{
/*
 *	print ri config portion of header to fptr
*/
	char  *p1,*p2,*p3,*p4;
	HDRAO_RI	*phri;
	phri=(HDRAO_RI*)phdr;
/*
smpMode :ippNoGwCnt--extTiming:--xxx---SubCycle:---xxx--useMux:xxx
ippNum  :--xxxxxxxx---ipps/buf:xxxxx-smpPairIpp:xxxxxx Packing:-xx--fifo:xx
ipp     :ffffffff.f----startOn:---xx---------gw:ffff.f
*/
	switch (phri->smpMode){
	    case (RIHDR_IPPNOGWCNT):
		p1="ippNoGwCnt";
		break;
	    case (RIHDR_IPPANDGWCNT):
		p1="ippGwCnt";
		break;
	    case (RIHDR_CONT):
		p1="continuous";
		break;
	    default:
	        p1="???";
	        break;
	}
	p2=(phri->extTiming)?"yes":" no";
	p3=(RIHDR_SUBCYCLEON(phri->muxAndSubCycle))?" on":"off";
	p4=(RIHDR_USEMUXON(phri->muxAndSubCycle))?"yes":"no";

	fprintf(fptr,
"smpMode :%10s  extTiming:  %3s   SubCycle:   %3s  useMux:%3s\n",
		p1,p2,p3,p4);
	fprintf(fptr,
"ippNum  :  %8d   ipps/buf:%5d smpPairIpp:%6d Packing: %2d  fifo:%d\n",
phri->ippNumStartBuf,phri->ippsPerBuf,phri->smpPairIpp,phri->packing,
phri->fifoNum);
	fprintf(fptr,
"ipp     :%10.1f    startOn:%5d         gw:%6.1f\n",
		phri->ipp,phri->startOn,phri->gw);
	return;
}

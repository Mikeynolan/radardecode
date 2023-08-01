#include	<stdio.h>
#include    <aovmehdrLib.h>
/******************************************************************************
*hdrPrSpsBg - print spsBg header to file descriptor
*/ 
void	hdrPrSpsBg
	(
	 FILE 	     *fptr,		/* file ptr to output to */
	 HDRAO_SPSBG *phdr		/* hdr to output*/
	)
{
	int	i,j;
/*
 *	print spsBg portion of header to fptr
*/
/*
spsId   :ssss-----------spsVer:ssss
spsIpp  :xxxxxxxx.x--IppToRfOn:xxxxx.x-numRfPul:--ddd---rfUnit:xxxxx.x
spsGw   :xxxxxxxx.x---TxSmples:ddddd---rfLenTot:xxxxx.x-
baudLen :xxxxxxxx.x--bwCodeMhz:x.xx---codLnUsec:xxxxx.x
mpSeq   :ddd ddd (20 max)
numRcvWn:dddxxxxxxxxxxcodeName:ssssssssssssssssssss 
rw1start:xxxxxxxx.x --rw1Tsmpl:ddddd   rw1Csmpl:ddddd  
rw2start:xxxxxxxx.x --rw2Tsmpl:ddddd   rw2Csmpl:ddddd  
rw3start:xxxxxxxx.x --rw3Tsmpl:ddddd   rw3Csmpl:ddddd  
rw4start:xxxxxxxx.x --rw4Tsmpl:ddddd   rw4Csmpl:ddddd  
rw5start:xxxxxxxx.x --rw5Tsmpl:ddddd   rw5Csmpl:ddddd  
*/

	fprintf(fptr,
"spsId   :%4.4s           spsVer:%4.4s\n",
   	    phdr->id,phdr->ver);
	fprintf(fptr,
"spsIpp  :%10.1f  IppToRfOn:%7.1f numRfPul:  %3d   rfUnit:%7.1f\n",
   	    phdr->ipp,phdr->txIppToRfOn,phdr->numRfPulses,phdr->mpUnit);

	fprintf(fptr,
"spsGw   :%10.1f   TxSmples:%5d   rfLenTot:%7.1f\n",
   	    phdr->gw,phdr->smpInTxPulse,phdr->rfLen);

	fprintf(fptr,
"baudLen :%10.1f  bwCodeMhz:%7.1f   codLnUsec:%7.1f\n",
   	    phdr->baudLen,phdr->bwCodeMhz,phdr->codeLenUsec);

	fprintf(fptr,
"mpSeq   :");
	for (i=0;(i<phdr->numRfPulses) && (i < HDRAO_SPSBG_MAX_MP_SEQ);i++){
	    fprintf(fptr,"%3d ",phdr->mpSeq[i]);
	    if ((i==16) && (phdr->numRfPulses>17))fprintf(fptr,"\n         ");
	}
	fprintf(fptr,"\n");

	fprintf(fptr,
"numRcvWn:%3d          codeName:%s\n",
  	    phdr->numRcvWin,phdr->codeName);

	for (i=0;(i<phdr->numRcvWin)&& (i<HDRAO_SPSBG_MAX_RCV_WIN);i++){
	   j=i+1;
	   fprintf(fptr,
"rw%1dstart:%10.1f   rw%1dTsmpl:%5d   rw%1dCsmpl:%5d\n",
		j,phdr->rcvWin[i].startUsec,j,phdr->rcvWin[i].numSamples,
		j,phdr->rcvWin[i].numSamplesCal);
	}
	return;
}

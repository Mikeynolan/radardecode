#include	<stdio.h>
#include    <aovmehdrLib.h>
/******************************************************************************
*hdrPrIfLo	- output if/lo header to file ptr						      
*DESCRIPTION
*Output if/lo header to supplied file ptr. A ptr to the start of the
*if/lo header is input.
*
* history:
* 24jun99 - started
* 09dec99 - lo2hsd now a 4 bit number
* 17oct07 - now 8 synths
rfNum :      dd if1Num:       d  xfr1N:       0  xfr2N:       1     id:xxxx
hybIn :0        hbPhLo:     ddd  hbPhS:     ddd useFib:       1    ver:xx.x
rfAttn:   dd dd ifAttn:   dd dd  if2Gn: -xx -xx zmAttn: 1.5 1.5 zmNDOn:   0
lbFb  : xxx xxx lo1hSd:       1 lo2hSd: 1 1 1 1 zmNDLv:    11.1 zmNDTA:   0
rcvCal:      xx calTyp:xxxx     calSrc:       1 ac1Pwr:    xxxx ac2Pwr: xxx
sbShCl:       0 syn1Rf:       1 syn2Rf:       1 vis30M:	     xx pwrMFp:xxxx  
if2Inp:	      x vlbaFr:xxxx     blk430:       1 if2Nse:       x 2pol30:   1
synDes: x x x x mxcfr :1 1 1 1  bampSr: 1 1 1 1 extAmp:xx xx xx xx
pwrM    : -dd.dd -dd.dd ddddd -dd.dd -dd.dd ddddd               lbwPol:xxx
rfFreq  : 11111.111111 lo1: 11111.111111 
lo2     : 11111.111111      11111.111111       11111.111111 11111.111111
        : 11111.111111      11111.111111       11111.111111 11111.111111

rfFreq

*/
void	hdrPrIfLo
	(
	 FILE *fptr,
	 void *phdr			/* ptr to start of iflo portion of header*/
	)
{
	HDRAO_IFLO	*piflo;
	HDRAO_IF1	*pif1;
	HDRAO_IF2	*pif2;

	piflo=(HDRAO_IFLO*)phdr; 
	pif1  =(HDRAO_IF1*)&piflo->if1;
	pif2  =(HDRAO_IF2*)&piflo->if2;

	fprintf(fptr,
"rfNum :      %2u if1Num:       %1u   xfr1:       %1u   xfr2:       %1u     id:%4.4s\n",
	pif1->st1.rfNum,pif1->st1.ifNum,pif1->st2.zmNormal,pif2->st1.xferNormal,
	piflo->id);

	fprintf(fptr,
"hybIn :%1d        hbPhLo:     %3d  hbPhS:     %3d useFib:       %1d    ver:%4.4s\n",
	pif1->st1.hybridIn,pif1->hybLoPh,pif1->hybSigPh,pif1->st1.useFiber,
	piflo->ver);

	fprintf(fptr,
"rfAttn:   %2d %2d ifAttn:   %2d %2d  if2Gn: %3d %3d zmAttn: %3.1f %3.1f zmNDOn:   %1d\n",
	pif1->attn.rf[0],pif1->attn.rf[1],pif1->attn.ifA[0],pif1->attn.ifA[1],
	pif2->gain[0],pif2->gain[1],pif1->attn.zm[0]*.1,pif1->attn.zm[1]*.1,
    pif1->st2.zmDiodeOn);

	fprintf(fptr,
"lbFb  : %3x %3x lo1hSd:       %1d lo2hSd: %1d %1d %1d %1d zmNDLv:    %4.1f zmNDTA:   %1d\n",
	pif1->st1.lbFbA,pif1->st1.lbFbB,pif1->st1.lo1Hsd,
	pif1->st2.lo2Hsd & 1, 
	(pif1->st2.lo2Hsd >> 1) & 1, 
	(pif1->st2.lo2Hsd >> 2) & 1, 
	(pif1->st2.lo2Hsd >> 3) & 1,
	pif1->attn.zmNDLev*.1,pif1->st2.zmDiodeToA);

	fprintf(fptr,
"rcvCal:      %2d calTyp:%4d     calSrc:       %1d ac1Pwr:    %4x ac2Pwr: %3x\n",
	pif1->st2.calRcvMux,pif1->st2.calType,pif2->st1.calttlSrc,
	pif1->st2.ac1PwrSw,pif1->st2.ac2PwrSw);

	fprintf(fptr,
"sbShCl:       %1d syn1Rf:       %1d syn2Rf:       %1d vis30M:	     %2s pwrMFp:%2s\n",
	pif1->st2.sbShClosed,pif1->st1.syn1RfOn,pif1->st1.syn2RfOn,
	(pif2->st1.vis30Mhz)?"gr":"ch",(pif2->st1.pwrMToIf)?"if":"fp");

	fprintf(fptr,
"if2Inp:	   %4d vlbaFr:%4d     blk430:       %1d sigSrc:      %2d 2pol30:   %1d\n",
	(pif2->st1.ifInpFreq==0)? 0: (pif2->st1.ifInpFreq==1)? 300:
	(pif2->st1.ifInpFreq==2)? 750:1500,
	(pif2->st1.vlbaInpFrq)?1500:750,pif2->st1.sbDopTrack,pif2->st1.sigSrc,
	pif2->st1.dualPol30);

	fprintf(fptr,
"synDes: %1d %1d %1d %1d mxcfr :%1d %1d %1d %1d bampSrc: %1d %1d %1d %1d extAmp:%2x %2x %2x %2x\n",
	pif2->st4[0].synDest,pif2->st4[1].synDest,pif2->st4[2].synDest,
	pif2->st4[3].synDest,
	pif2->st4[0].mixerCfr,pif2->st4[1].mixerCfr,pif2->st4[2].mixerCfr,
	pif2->st4[3].mixerCfr,
	pif2->st4[0].ampInpSrc,pif2->st4[1].ampInpSrc,pif2->st4[2].ampInpSrc,
	pif2->st4[3].ampInpSrc,
	pif2->st4[0].ampExtMsk,pif2->st4[1].ampExtMsk,pif2->st4[2].ampExtMsk,
	pif2->st4[3].ampExtMsk);
	
	fprintf(fptr,
"pwrM    : %5.2f %5.2f %5d %5.2f %5.2f %5d               lbwPol:%3.3s\n",
	pif1->pwrDbm[0],pif1->pwrDbm[1],pif1->pwrTmStamp,
	pif2->pwrDbm[0],pif2->pwrDbm[1],pif2->pwrTmStamp,
	(pif1->st1.lbwLinPol)?"lin":"cir");

	fprintf(fptr,
"rfFreq  : %12.6f lo1: %12.6f\n",
	pif1->rfFrq*1e-6,pif1->lo1  *1e-6);

	fprintf(fptr,
"lo2     : %12.6f      %12.6f       %12.6f %12.6f\n",
	pif2->synFreq[0]*1e-6,pif2->synFreq[1]*1e-6,pif2->synFreq[2]*1e-6,
	pif2->synFreq[3]*1e-6);

	return;
}

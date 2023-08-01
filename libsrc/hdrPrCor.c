#include	<stdio.h>
#include	<hdrLib.h>
#include    <aovmehdrLib.h>
#include	<cor.h>
/******************************************************************************/
/*	hdrPrCor							      */
/******************************************************************************/
void	hdrPrCor(FILE *fp,void *phdr)
{
/*
 *	print cor config portion of header to fptr
 *mod history:
 *06dec99 - got rid of rfCfr from cor header
*/
/*
mclkPerN:dddddddd     dmpLenMC:dddddddd  nDumps:dddddddd    id:aaaa   ver:aaaa
lagSbcI :dddddddd      lagSbcO:dddddddd   bwNum:dddddddd nsbcI:dddd nsbcO:dddd
lagConfg:dddddddd  chipTstMode:dddddddd  stokes:dddddddd  dnyQ:dddd level:dddd
dataForm:dddddddd  blnkEnabled:dddddddd totCntI:dddddddd start:dddd rlBSh:dddd
boardId :dddddddd  numBrdsUsed:dddddddd PwrCntI:dddddddd
pwrCnt1 :fffffff.  pwrCnt2    :fffffff.                  attn1:dddd attn2:dddd
pwrRat1 :fff.ffff  pwrRat2    :fff.ffff 
*/
	HDRAO_CORV1	*phdr1;
	HDRAO_CORV2	*phdr2;
    int     itempAr[5];
    char    *pc,*pc2,ctemp[20];


	phdr1=(HDRAO_CORV1*)phdr;
	/**************************************************************************
     * version 1
    */
    if (strncmp(phdr1->ver," 1",2)==0){
	fprintf(fp,
"mclkPerN:%8d  dmpIntLenMC:%8d  nDumps:%8d    id:%4.4s   ver:%4.4s\n",
	phdr1->masterClkPeriod,phdr1->dumpLen,phdr1->dumpsPerInteg,phdr1->id,
	phdr1->ver);

	fprintf(fp,
"lagSbcI :%8d      lagSbcO:%8d   bwNum:%8d nsbcI:%4d nsbcO:%4d\n",
	phdr1->lagSbcIn,phdr1->lagSbcOut,phdr1->bwNum,phdr1->numSbcIn,
	phdr1->numSbcOut);

	fprintf(fp,
"lagConfg:%8d  chipTstMode:%8d  stokes:%8d  dnyQ:%4d level:%4d\n",
	phdr1->lagConfig,phdr1->chipTestMode,phdr1->stokes,phdr1->doubleNyquist,
	phdr1->levels);

	fprintf(fp,
"dataForm:%8d  blnkEnabled:%8d totCntI:%8d start:%4d relBSh:%4d\n",
	phdr1->dataFormat,phdr1->blankingEnabled,phdr1->totCntsInc,phdr1->startOn,
	phdr1->bitsToUse);

	fprintf(fp,
"boardId :%8d  numBrdsUsed:%8d PwrCntI:%8d\n",
	phdr1->boardId,phdr1->numBrdsUsed,phdr1->pwrCntInc);

	fprintf(fp,
"pwrCnt1 :%8.0f  pwrCnt2    :%8.0f                  attn1:%4d attn2:%4d\n",
	    phdr1->pwrCnt[0],phdr1->pwrCnt[1],phdr1->attnDb[0],phdr1->attnDb[1]);

	fprintf(fp,
"pwrRat1 :%8.4f  pwrRat2    :%8.4f \n",
	    phdr1->lag0PwrRatio[0],phdr1->lag0PwrRatio[1]);
	}
	/**************************************************************************
    * version 2
   */
/*
mclkPerN:dddddddd     dmpLenMC:dddddddd  nDumps:dddddddd    id:aaaa   ver:aaaa
lagSbcI :dddddddd      lagSbcO:dddddddd   bwNum:dddddddd nsbcI:dddd nsbcO:dddd
lagConfg:dddddddd  chipTstMode:dddddddd  stokes:ddddzzzd  dnyQ:zzzd level:zzzd
dataForm:ssssssss  blnkEnabled:zzzddddd totCntI:zzzddddd start:zzzd reBSh:dddd
boardId :dddddddd  numBrdsUsed:dddddddd PwrCntI:dddddzzz frqNm:dddd cycln:dddd
pwrCnt1 :fffffff.  pwrCnt2    :fffffff.  calCyc:dddddddd attn1:dddd cohAd:dddd
pwrRat1 :fff.ffff  pwrRat2    :fff.ffff  frqCyc:dddddddd attn2:dddd codeD:dddd
calOn 1 :1.ffffff   calOn2    :f.ffffff  state1:xxxxxxxx baudL:ddddd flip:d
calOff1 :1.ffffff  calOff2    :f.ffffff  state2:xxxxxxxx codeL:ddddd
*/
   else if (strncmp(phdr1->ver," 2",2)==0){
		phdr2=(HDRAO_CORV2*)phdr;
		    fprintf(fp,
"mclkPerN:%8d  dmpIntLenMC:%8d  nDumps:%8d    id:%4.4s   ver:%4.4s\n",
    phdr2->masterClkPeriod,phdr2->dumpLen,phdr2->dumpsPerInteg,phdr2->id,phdr2->ver)
;
 
    fprintf(fp,
"lagSbcI :%8d      lagSbcO:%8d   bwNum:%8d nsbcI:%4d nsbcO:%4d\n",
    phdr2->lagSbcIn,phdr2->lagSbcOut,phdr2->bwNum,phdr2->numSbcIn,phdr2->numSbcOut);
 
    itempAr[0]=(phdr2->state & COR_ST_CHIPTESTMODE)?1:0;
    itempAr[1]=(phdr2->state & COR_STC_STOKES)?1:0;
    itempAr[2]=(phdr2->state & COR_ST_DOUBLENYQUIST)?1:0;
    itempAr[3]=(phdr2->state & COR_STC_9LEVEL)      ?9:3;
    fprintf(fp,
"lagConfg:%8d  chipTstMode:%8d  stokes:%8d  dnyQ:%4d level:%4d\n",
    phdr2->lagConfig,itempAr[0],itempAr[1],itempAr[2],itempAr[3]);
 
    switch (phdr2->state & COR_ST_FMT_MASK){
        case COR_ST_FMT_RAWACF:
            pc="rawAcf";
            break;
        case COR_ST_FMT_CMBACF:
            pc="cmbAcf";
            break;
        case COR_ST_FMT_SPECTRA:
            pc="spectra";
            break;
        case COR_ST_FMT_PACK:
            pc="packed";
            break;
        default:
            pc="??????";
    }
    itempAr[0]=(phdr2->state & COR_ST_BLANKENABLED)?1:0;
    itempAr[1]=(phdr2->state & COR_ST_TOTCNTSINC)?1:0;
    itempAr[2]=(phdr2->state & COR_ST_RELBITSHIFT)>>4;
    if (phdr2->state & COR_ST_RELBITSHIFTSGN) itempAr[2]*=-1;

    switch (phdr2->state & COR_ST_START_MASK){
        case COR_ST_START_IMD_SW:
            pc2="imdS";
            break;
        case COR_ST_START_IMD_HW:
            pc2="imdH";
            break;
        case COR_ST_START_TICK1:
            pc2="1sec";
            break;
        case COR_ST_START_TICK10:
            pc2="10s ";
            break;
        default:
            pc2="????";
    }
    fprintf(fp,
"dataForm:%8s  blnkEnabled:%8d totCntI:%8d start:%4s reBSh:%4d\n",
    pc,itempAr[0],itempAr[1],pc2,itempAr[2]);
 
    ctemp[0]=0;
    if (phdr2->state & COR_ST_PWRCNTINC) {
       if (phdr2->state & COR_STC_PWRCNTI) strcat(ctemp,"I");
       if (phdr2->state & COR_STC_PWRCNTQ) strcat(ctemp,"Q");
    }

    fprintf(fp,
"boardId :%8d  numBrdsUsed:%8d PwrCntI:%8s frqNm:%4d cycLn:%4d\n",
    phdr2->boardId,phdr2->numBrdsUsed,ctemp,phdr2->frqBufThisRec,
	phdr2->cycleLen);
 
    fprintf(fp,
"pwrCnt1 :%8.0f  pwrCnt2    :%8.0f  calCyc:%8.8s attn1:%4d cohAd:%4d\n",
       phdr2->pwrCnt[0],phdr2->pwrCnt[1],phdr2->calCyc,phdr2->attnDb[0],
	   phdr2->cohAdd);
 
    fprintf(fp,
"pwrRat1 :%8.4f  pwrRat2    :%8.4f  frqCyc:%8.8s attn2:%4d codeD:%4d\n",
        phdr2->lag0PwrRatio[0],phdr2->lag0PwrRatio[1],phdr2->frqCyc,
		phdr2->attnDb[1],phdr2->codeDelay);
  
    fprintf(fp,
"calOn 1 :%8.6f   calOn2    :%8.6f  state1:%08x baudL:%4d flip:%1d\n",
        phdr2->calOn[0],phdr2->calOn[1],phdr2->state,phdr2->baudLen,
		(phdr2->state&COR_ST_SPECTRA_FLIPPED)?1:0);
 
    fprintf(fp,
"calOff1 :%8.6f  calOff2    :%8.6f  state2:%08x codeL:%4d\n",
        phdr2->calOff[0],phdr2->calOff[1],phdr2->state2,phdr2->codeLen);
	}
	else {
		fprintf(stdout,"Unknown cor version number..:%4.4s...%p\n",
				phdr1->ver,phdr1->ver);
		fflush(stdout);
	}
	return;
}

#include	<stdio.h>
#include	<math.h>
#include	<mathCon.h>
#include    <aovmehdrLib.h>
#include	<convLib.h>

LOCAL void hdrPntFmtCrd(float c1,float c2,int cs,char *pc1F,char	*pc2F,
	 			   char *pcsF);

/******************************************************************************
* hdrPrPnt - print pntHdr to file pointer.							      
*/
void hdrPrPnt(FILE *fp,void *phdrI)
{
/*
 *	print pntportion of header to fptr
*/
/*

... measured 
  measaz: ddd.dddd          gr: xx.xxxx      ch: dd.dddd    tm: sssss.sss 
   azErA: 99999.9       zaErrA: 99999.d totErrA: 99999.9  
   lrAvg: 1256.xxx ddddd  lr12: 1256.xxx    lr4: xxxx.xxx  lr8: xxxx.xxx
   lrTmp: xx.xx            tur: ddd.ddd ddddd.d
    td12:  dd.ddd  ddddd.d td4:  dd.ddd ddddd.d            td8: dd.ddd ddddd.d

... requested

  secAst: ddddd  yrDayAst: yyyyddd    mjd: dddddddd    ut1Frac: 0.999999  
   azCor: -d.ddd   zacor: -d.ddd   modLoc: 333.3333 20.0000

     Pos:  xx:xx:xx.x ddd:mm:ss.s J   Off:  xx:xx:xx.x ddd:mm:ss.s J 
     rat:  xx:xx:xx.x dddddd.dddd J stDay:  ddd           stTm: ssssss
  CumPos:  xx:xx:xx.x ddd:mm:ss.s J  geoV: -0.000000000   HELV: -0.000000000

... stat
   grMst: 1 useMod: 1 trking: 1 crdOk: 1 allOk:     ver: xxxx id: xxxx 
scrmtdOk: 1  turOk: 1  terOk: 1  lrOk: 1 pntOk: 1 agcOk: 1 

*/
	HDRAO_PNT	*phdr;
	double d1,d2,sinza;
	char ctmp1[20];
	char ctmp2[20];
	char ctmp3[20];
	char ctmp4[20];
	char ctmp5[20];
	char ctmp6[20];


	phdr=(HDRAO_PNT*)phdrI;
	/**************************************************************************
     * 
    */
	fprintf(fp,
"  measaz: %8.4f          gr: %7.4f      ch: %7.4f    tm: %9.3f\n", 
	phdr->m.azTTD*.0001,phdr->m.grTTD*.0001, phdr->m.chTTD*.0001,
	phdr->m.agcTmStamp*.001);

	sinza=(phdr->stat.grMaster)?sin(phdr->m.grTTD*.0001*C_DEG_TO_RAD) 
	                           :sin(phdr->m.chTTD*.0001*C_DEG_TO_RAD);
	d1=phdr->errAzRd*sinza*C_RAD_TO_ASEC;
	d2=phdr->errZaRd*C_RAD_TO_ASEC;
	fprintf(fp,
"   azErA: %7.1f       zaErrA: %7.1f totErrA: %7.1f\n", 
		d1,d2,sqrt(d1*d1+d2*d2));

   fprintf(fp,
"   lrAvg: %8.3f %5d  lr12: %8.3f    lr4: %8.3f  lr8: %8.3f\n",
	(phdr->m.platformHgt[0]+phdr->m.platformHgt[1]+phdr->m.platformHgt[2])/3.,
	phdr->m.platformTmStamp,
	phdr->m.platformHgt[0],phdr->m.platformHgt[1],phdr->m.platformHgt[2]);

	fprintf(fp,
"   lrTmp: %5.2f            tur: %7.3f %7.1f\n",
	phdr->m.platformTemp, phdr->m.turPosRd*C_RAD_TO_DEG,
	phdr->m.turTmStamp*.001);

	fprintf(fp,
"    td12:  %6.3f  %7.1f td4:  %6.3f %7.1f            td8: %6.3f %7.1f\n",
	phdr->m.tdPosIn[0],phdr->m.tdTmStamp[0]*.001,
	phdr->m.tdPosIn[1],phdr->m.tdTmStamp[1]*.001,
	phdr->m.tdPosIn[2],phdr->m.tdTmStamp[2]*.001);

	fprintf(fp,
"  secAst: %5d  yrDayAst: %7d    mjd: %8d     ut1Frac: %8.6f\n", 
	phdr->r.secMid,phdr->r.yearDaynum,phdr->r.mjd,phdr->r.ut1Frac);
	
	fprintf(fp,
"   azCor: %6.3f   zacor: %6.3f   modLoc: %8.4f %7.4f\n",
	phdr->r.posCorrectionRd[0]*C_RAD_TO_DEG,
	phdr->r.posCorrectionRd[1]*C_RAD_TO_DEG,
	phdr->r.modelLocDeg[0],phdr->r.modelLocDeg[1]);
	/*
	 * position,offset 
	*/
	hdrPntFmtCrd(phdr->r.reqPosRd[0],phdr->r.reqPosRd[1],
				 phdr->stat.coordSysPos,ctmp1,ctmp2,ctmp3);
	hdrPntFmtCrd(phdr->r.reqOffRd[0],phdr->r.reqOffRd[1],
				 phdr->stat.coordSysOff,ctmp4,ctmp5,ctmp6);
	 fprintf(fp,
"     Pos:%12.12s %11.11s %1.1s   Off:%12.12s %11.11s %1.1s\n",
		ctmp1,ctmp2,ctmp3,ctmp4,ctmp5,ctmp6);
	/*
     * rate
	*/
	hdrPntFmtCrd(phdr->r.reqRateRdSec[0],phdr->r.reqRateRdSec[1],
				 phdr->stat.coordSysRate,ctmp1,ctmp2,ctmp3);
	 fprintf(fp,
"     rat:%12.12s %11.11s %1.1s stDay:  %3d           stTm: %5d\n",
		ctmp1,ctmp2,ctmp3,(int)phdr->r.dayNumAstRateStart,
		(int)((phdr->r.dayNumAstRateStart-((int)phdr->r.dayNumAstRateStart))*
			86400.+.5));
	/*
     * cum positions
	*/
	hdrPntFmtCrd(phdr->r.raJCumRd,phdr->r.decJCumRd,PNTHDR_CRD_J2000,
				 ctmp1,ctmp2,ctmp3);
	fprintf(fp,
"  CumPos:%12.12s %11.11s %1.1s  geoV: %12.9f   HELV: %12.9f\n",
			ctmp1,ctmp2,ctmp3,phdr->r.geoVelProj,phdr->r.helioVelProj);
	/*
 	 * status word 
	*/
	fprintf(fp,
"   grMst: %1d useMod: %1d trking: %1d crdOk: %1d allOk:%1d   ver: %4.4s id: %4.4s\n",
		phdr->stat.grMaster,phdr->stat.modelCor,phdr->stat.tracking,
		phdr->stat.coordValid,phdr->stat.allOk,phdr->ver,phdr->id);
	fprintf(fp,
"scrmtdOk: %1d  turOk: %1d  terOk: %1d  lrOk: %1d pntOk: %1d agcOk: %1d\n",
		phdr->stat.tdOk,phdr->stat.turOk,phdr->stat.terOk,phdr->stat.lrOk,
		phdr->stat.pntOk,phdr->stat.agcOk);
		
	return;
}
/******************************************************************************
* hdrPntFmtCrd - format a single coordinate.
*/
LOCAL void hdrPntFmtCrd
	(
	 float	c1,		/* first coord*/
	 float	c2,		/* 2nd coord*/
	 int    cs,		/* coordinate sys code*/
	 char   *pc1F,	/* return coord 1*/
	 char	*pc2F,
	 char	*pcsF 
	)
{
	  double maxVal = 2*C_2PI;
		
	  strcpy(pc1F," ");
	  strcpy(pc2F," ");
	  strcpy(pcsF," ");
	  if ((c1 > maxVal) || (c1 < -maxVal)){
			strcpy(pc1F,"???");
	  }
	  if ((c2 > maxVal) || (c1 < -maxVal)){
			strcpy(pc2F,"???");
	  }
	  if (cs==PNTHDR_CRD_NOT_IN_USE){
			return;
	  }
      /*
       * convert from radians to normal coordinates
      */
      switch (cs) {
         case PNTHDR_CRD_B1950:
				strcpy(pcsF,"B");
				goto cont1;
         case PNTHDR_CRD_J2000:
				strcpy(pcsF,"J");
				goto cont1;
         case PNTHDR_CRD_HA_DEC:
				strcpy(pcsF,"H");
				goto cont1;
         case PNTHDR_CRD_CURRENT:
				strcpy(pcsF,"C");
				goto cont1;
cont1:     	if (pc1F[0]==' ')fmtRdToHmsD(c1,1,pc1F);
         	if (pc2F[0]==' ')fmtRdToDms(c2,1,pc2F);
         	break;
         case PNTHDR_CRD_AZZA_FEED:
				strcpy(pcsF,"F");
				goto cont2;
         case PNTHDR_CRD_AZZA_SRC:
				strcpy(pcsF,"A");
				goto cont2;
         case PNTHDR_CRD_AZZA_NOMOD:
				strcpy(pcsF,"X");
				goto cont2;
         case PNTHDR_CRD_AZZA_GC:
				strcpy(pcsF,"S");
				goto cont2;
         case PNTHDR_CRD_GAL:
				strcpy(pcsF,"G");
				goto cont2;
         case PNTHDR_CRD_BECLIPTIC:
				strcpy(pcsF,"w");
				goto cont2;
         case PNTHDR_CRD_JECLIPTIC:
				strcpy(pcsF,"E");
				goto cont2;
cont2:         
	       if (pc1F[0] == ' ') sprintf(pc1F,"%12.4f",c1*C_RAD_TO_DEG);
           if (pc2F[0] == ' ') sprintf(pc2F,"%11.4f",c2*C_RAD_TO_DEG);
              break;
         default:
              sprintf(pc1F,"cs ??%7d",cs);
      }
	  return;
}

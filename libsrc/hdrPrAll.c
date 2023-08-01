//#include 	<phillibExtras.h>
#include	<stdio.h>
#include	<hdrLib.h>
#include    <aovmehdrLib.h>

/******************************************************************************
hdrPrAll - try to print out complete hdr header
05jul03 - added test for aeronomy hdr without the hdrid
15jan08 - converted to work on linux.. only output headers we know about.
*/ 
void	hdrPrAll
	(
	 FILE 	     *fptr,		/* file ptr to output to */
	 HDRAO_STD *phdr		/* hdr to output*/
	)
{
/*
 *	print std header  and all the sections that we recognize
*/
	int		inpCode;
	int		procCode;
	int		timeCode;
	int		miscCode;
	hdrPrStd(fptr,phdr);
	/*
	 * iflo
	*/
	if (HDR_S_GET_CODE(phdr->sec.iflo) == HDR_S_IFLO){
		fprintf(fptr,"___IFLOSECTION\n");
		hdrPrIfLo(fptr,IFLOHDR_GET(phdr));
	}
	/*
	 *	pnt section 
	*/
	if (HDR_S_GET_CODE(phdr->sec.pnt) == HDR_S_PNT){
		fprintf(fptr,"___PNTSECTION\n");
		hdrPrPnt(fptr,PNTHDR_GET(phdr));
	}
	/*
	 * input section
	*/
	inpCode=HDR_S_GET_CODE(phdr->sec.inp);
	if ( (inpCode == 0) && ( HDR_S_GET_LEN(phdr->sec.inp) == sizeof(HDRAO_RI))){
		inpCode=HDR_S_RI;
	}
	switch (inpCode){
		case HDR_S_RI: 
		   fprintf(fptr,"___RISECTION\n");
	       hdrPrRi(fptr,RIHDR_GET(phdr));
		   break;
		case HDR_S_COR: 
		   fprintf(fptr,"___CORSECTION\n");
	       hdrPrCor(fptr,CORHDR_GET(phdr));
		   break;
		default:
		   break;
	}
/*
 *	processing section
*/
	procCode=HDR_S_GET_CODE(phdr->sec.proc);
/*
 * check for atm files without procid
*/
/*
    if (procCode == 0) {
        len= HDR_S_GET_LEN(phdr->sec.proc);
        if (len != 0) {
            if ( len == sizeof(HDRAO_MRACF)) {
                 procCode=HDR_S_MRACF;
            } else if (strncmp(phdr->id,"pwr",3) == 0 ) {
                procCode=HDR_S_PWR;
            } else if (strncmp(phdr->id,"clp",3) == 0 ) {
                procCode=HDR_S_CLP;
            }
        }
	}
*/
   switch (procCode){
        case  HDR_S_PWR:
//            hdrPrPwr(fptr,HDRAO_PWR_GET(phdr));
            break;
        case  HDR_S_CLP:
//            hdrPrClp(fptr,HDRAO_CLP_GET(phdr));
            break;
        case  HDR_S_MRACF:
//            hdrPrMracf(fptr,HDRAO_MRACF_GET(phdr));
            break;
        case  HDR_S_DOP:
            fprintf(fptr,"___DOPSECTION\n");
            hdrPrDop(fptr,HDRAO_DOP_GET(phdr));
            break;
        default:
            break;
    }

/*
 *	misc section
*/
	miscCode=HDR_S_GET_CODE(phdr->sec.misc);
	switch (miscCode){
		case  HDR_S_PROC:
		    fprintf(fptr,"___PROCSECTION\n");
	    	hdrPrProc(fptr,HDRAO_PROC_GET(phdr));
			break;
		default:
			break;
	}

/*
 *	time section 
*/
	timeCode=HDR_S_GET_CODE(phdr->sec.time);
	if ( (timeCode == 0) && ( HDR_S_GET_LEN(phdr->sec.time) == 
			sizeof(HDRAO_SPSBG))){
		timeCode=HDR_S_SPSBG;
	}
	switch (timeCode){
		case  HDR_S_SPSBG:
		    fprintf(fptr,"___SPSBG\n");
	    	hdrPrSpsBg(fptr,HDRAO_SPSBG_GET(phdr));
			break;
		default:
		break;
	}
	
	return;
}

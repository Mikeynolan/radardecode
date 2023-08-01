#include	<stdio.h>
#include    <aovmehdrLib.h>
/******************************************************************************/
/*	hdrPrStd							      
* history:
* 24nov94 - removed spare1, added stScanTm
* 24jun99 - sec.out -> sec.iflo
*/
void	hdrPrStd(FILE *fptr,HDRAO_STD *phdr)
{
/*
 *	print stdhdr portion of header to fptr
*/
/*
Marker:xxxx------PrgId:xxxxxxxx--Vers:xxxx--HdrLen:xxx-------RecLen:xxxxxx 
Date  :yyyyddd----Time:xxxxx---ExpNum:xxxxxx--Scan:xxxxxxxxx-RecNum:xxxxxxxx
grpNum:xxxxxx--grpRTot:xxxxx--grpRCur:xxxx----Data:xxx-----stScanTm:xxxxx
posTm :xxxxxxxx  azPos:xxxxxxx  grPos:xxxxxx chPos:xxxxxxx
InputSec:xxxxxxxx-------offset:-xxxx-----length:--xxxx
 ifloSec:xxxxxxxx-------offset:-xxxx-----length:--xxxx
 ProcSec:xxxxxxxx-------offset:-xxxx-----length:--xxxx
  PntSec:xxxxxxxx-------offset:-xxxx-----length:--xxxx
 TimeSec:xxxxxxxx-------offset:-xxxx-----length:--xxxx
 MiscSec:xxxxxxxx-------offset:-xxxx-----length:--xxxx
*/
	fprintf(fptr,
"Marker:%4.4s      PrgId:%8.8s  Vers:%4.4s  HdrLen:%3d       RecLen:%6d\n",
phdr->hdrMarker,phdr->id,phdr->version,phdr->hdrlen,phdr->reclen); 

	fprintf(fptr,
"  Date:%7d    Time:%5d   ExpNum:%6d  Scan:%9d RecNum:%6d\n",
phdr->date,phdr->time,phdr->expNumber,phdr->scanNumber,phdr->recNumber); 

	fprintf(fptr,
"grpNum:%6d  grpRTot:%5d  grpRCur:%4d    Data:%3.3s     stScanTm:%5d\n",
phdr->grpNum,phdr->grpTotRecs,phdr->grpCurRec,phdr->dataType,
phdr->stScanTime); 
	
	fprintf(fptr,
"posTm :%8d  azPos:%7d  grPos:%6d chPos:%6d\n",
	phdr->posTmMs,phdr->azTTD,phdr->grTTD,phdr->chTTD);

	if (phdr->sec.inp != 0){
	   fprintf(fptr, 
"%8s:%8x       offset: %4d     length:  %4d\n",
		"InputSec",
		 HDR_S_GET_CODE(phdr->sec.inp),HDR_S_GET_OFF(phdr->sec.inp),
		 HDR_S_GET_LEN(phdr->sec.inp));
	}

	if (phdr->sec.iflo != 0){
	   fprintf(fptr, "%8s:%8x       offset: %4d     length:  %4d\n",
		"    iflo",
		HDR_S_GET_CODE(phdr->sec.iflo),HDR_S_GET_OFF(phdr->sec.iflo),
		HDR_S_GET_LEN(phdr->sec.iflo));
	}

	if (phdr->sec.proc != 0){
	   fprintf(fptr, "%8s:%8x       offset: %4d     length:  %4d\n",
		" ProcSec",
		HDR_S_GET_CODE(phdr->sec.proc),HDR_S_GET_OFF(phdr->sec.proc),
		HDR_S_GET_LEN(phdr->sec.proc));
	}

	if (phdr->sec.pnt != 0){
	   fprintf(fptr, "%8s:%8x       offset: %4d     length:  %4d\n",
		"  PntSec",
		HDR_S_GET_CODE(phdr->sec.pnt),HDR_S_GET_OFF(phdr->sec.pnt),
		HDR_S_GET_LEN(phdr->sec.pnt));
	}

	if (phdr->sec.time != 0){
	   fprintf(fptr, "%8s:%8x       offset: %4d     length:  %4d\n",
		" TimeSec",
		HDR_S_GET_CODE(phdr->sec.time),HDR_S_GET_OFF(phdr->sec.time),
		HDR_S_GET_LEN(phdr->sec.time));
	}

	if (phdr->sec.misc != 0){
	   fprintf(fptr, "%8s:%8x       offset: %4d     length:  %4d\n",
		" MiscSec",
		HDR_S_GET_CODE(phdr->sec.misc),HDR_S_GET_OFF(phdr->sec.misc),
		  HDR_S_GET_LEN(phdr->sec.misc));
	}

	return;
}

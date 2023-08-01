#include	<stdio.h>
#include    <aovmehdrLib.h>
/******************************************************************************
*hdrPrProc	- output proc (tcl procedure)header to file ptr						      
*DESCRIPTION
*Output doppler header to supplied file ptr. A ptr to the start of the
*dop header is input.
*
* history:
* 08dec99 - started
procNm: xxxxxxxxxxxx procVer: xxxx srcNm: xxxxxxxxxxxxxxxx id:xxxx ver:xxxx

*/
void	hdrPrProc
	(
	 FILE *fptr,
	 void *phdr			/* ptr to start of iflo portion of header*/
	)
{
	HDRAO_PROC	*pproc;

	pproc=(HDRAO_PROC*)phdr; 
	fprintf(fptr,
"procNm: %12.12s procVer: %4.4s srcNm: %16.16s id:%4.4s ver:%4.4s\n",
	pproc->procName,pproc->procVer,pproc->srcName,pproc->id,pproc->ver);

    fprintf(fptr,
"dar1- 5: %12.4f %12.4f %12.4f %12.4f %12.4f\n",
        pproc->dar[0],pproc->dar[1],pproc->dar[2],pproc->dar[3],pproc->dar[4]);
 
    fprintf(fptr,
"dar6-10: %12.4f %12.4f %12.4f %12.4f %12.4f\n",
        pproc->dar[5],pproc->dar[6],pproc->dar[7],pproc->dar[8],pproc->dar[9]);
 
    fprintf(fptr,
"iar1- 5: %12d %12d %12d %12d %12d\n",
        pproc->iar[0],pproc->iar[1],pproc->iar[2],pproc->iar[3],pproc->iar[4]);
 
    fprintf(fptr,
"iar6-10: %12d %12d %12d %12d %12d\n",
        pproc->iar[5],pproc->iar[6],pproc->iar[7],pproc->iar[8],pproc->iar[9]);
 
    fprintf(fptr,
"car1- 5: %8.8s %8.8s %8.8s %8.8s %8.8s\n",
        pproc->car[0],pproc->car[1],pproc->car[2],pproc->car[3],pproc->car[4]);
 
    fprintf(fptr,
"car6-10: %8.8s %8.8s %8.8s %8.8s %8.8s\n",
        pproc->car[5],pproc->car[6],pproc->car[7],pproc->car[8],pproc->car[9]);

	fflush(fptr);
	return;
}

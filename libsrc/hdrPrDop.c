#include	<stdio.h>
#include    <aovmehdrLib.h>
/******************************************************************************
*hdrPrDop	- output doppler header to file ptr						      
*DESCRIPTION
*Output doppler header to supplied file ptr. A ptr to the start of the
*dop header is input.
*
* history:
* 08dec99 - started
dopFac: xx.xxxxxxxxx velTyp: sss velCrd: sss dopAll: d tm: ddddd   id:xxxx
BCRest: xxxxx.xxxxxx velorZ: xxxxx.xxxxxx    obsVel: xxxxx.xxxxxx ver:xxxx
frqOff: xxxxx.xxxxxx         xxxxx.xxxxxx            xxxxx.xxxxxx xxxxx.xxxxxx
*/
void	hdrPrDop
	(
	 FILE *fptr,
	 void *phdr			/* ptr to start of iflo portion of header*/
	)
{
	HDRAO_DOP	*pdop;
	char		*pc1,*pc2;
	char		ctemp[80];

	pdop=(HDRAO_DOP*)phdr; 

	switch (pdop->stat.velType){
		case 0: pc1="vel";break;
		case 1: pc1="zo ";break;
		case 2: pc1="zr ";break;
		default: pc1="3";break;
	}
	switch (pdop->stat.velCrdSys){
		case 1: pc2="geo";break;
		case 2: pc2="hel";break;
		case 3: pc2="lsr";break;
		default: sprintf(ctemp,"%2d",pdop->stat.velCrdSys);
				pc2=ctemp;
				break;
	}

	fprintf(fptr,
"dopFac: %12.9f velTyp: %3s velCrd: %3s dopAll: %1d tm: %5d   id:%4.4s\n",
		pdop->dopFactor,pc1,pc2,pdop->stat.dopCorAllBands,pdop->tmDop, 
		pdop->id);

	fprintf(fptr,
"BCRest: %12.6f velorZ: %12.6f obsVel: %12.6f    ver:%4.4s\n",
		pdop->freqBCRest,pdop->velOrZ,pdop->velObsProj,pdop->ver);
		
	fprintf(fptr,
"frqOff: %12.6f         %12.6f        %12.6f %12.6f\n",
		pdop->freqOffsets[0],pdop->freqOffsets[1],pdop->freqOffsets[2],
		pdop->freqOffsets[3]);

	fflush(fptr);
	return;
}

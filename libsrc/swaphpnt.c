
/***************
 * swaphpnt - swap pnt hdr from bit to little endian.
 * SYNTAX: swaphpnt(ppnt);
 * ARGS  :
 * ppnt  : *HDRAO_PNT ptr pnt hdr block structure
 *
 *DESCRIPTION: 
 * 	swap the pnt hdr structure from big to little endian.
 *It does the flipping inplace. This is the structure
 *stored in the pointing part of the  ao headers
 *
*/
#include	<utilLib.h>
#include    <aovmehdrLib.h>

#define FLIP2i(val)  byteswap_2i(val)
#define FLIP2iu(val) byteswap_2iu(val)
#define FLIP4i(val) byteswap_4i(val)
#define FLIP4f(val) byteswap_4f(val)
#define FLIP8d(val) byteswap_8d(val)

void	swaphpnt(HDRAO_PNT *phdr)
{
	
	FLIP4i(&phdr->m.azTTD);
	FLIP4i(&phdr->m.grTTD);
	FLIP4i(&phdr->m.chTTD);
	FLIP4i(&phdr->m.agcTmStamp);
	FLIP4f(&phdr->m.turPosRd);
	FLIP4i(&phdr->m.turTmStamp);
	FLIP4f(&phdr->m.tdPosIn[0]);
	FLIP4f(&phdr->m.tdPosIn[1]);
	FLIP4f(&phdr->m.tdPosIn[2]);
	FLIP4i(&phdr->m.tdTmStamp[0]);
	FLIP4i(&phdr->m.tdTmStamp[1]);
	FLIP4i(&phdr->m.tdTmStamp[2]);
	FLIP4f(&phdr->m.platformHgt[0]);
	FLIP4f(&phdr->m.platformHgt[1]);
	FLIP4f(&phdr->m.platformHgt[2]);
	FLIP4f(&phdr->m.platformTemp);
	FLIP4i(&phdr->m.platformTmStamp);
	FLIP4f(&phdr->m.terPos[0]);
	FLIP4f(&phdr->m.terPos[1]);
	FLIP4f(&phdr->m.terPos[2]);
	FLIP4i(&phdr->m.terTmStamp);
	FLIP8d(&phdr->r.ut1Frac);
	FLIP8d(&phdr->r.lastRd);
	FLIP4i(&phdr->r.mjd);
	FLIP4i(&phdr->r.yearDaynum);
	FLIP4f(&phdr->r.reqPosRd[0]);
	FLIP4f(&phdr->r.reqPosRd[1]);
	FLIP4f(&phdr->r.reqOffRd[0]);
	FLIP4f(&phdr->r.reqOffRd[1]);
	FLIP4f(&phdr->r.reqRateRdSec[0]);
	FLIP4f(&phdr->r.reqRateRdSec[1]);
	FLIP8d(&phdr->r.dayNumAstRateStart);
	FLIP4f(&phdr->r.raJCumRd);
	FLIP4f(&phdr->r.decJCumRd);
	FLIP8d(&phdr->r.geoVelProj);
	FLIP8d(&phdr->r.helioVelProj);
	FLIP4f(&phdr->r.posCorrectionRd[0]);
	FLIP4f(&phdr->r.posCorrectionRd[1]);
	FLIP4f(&phdr->r.modelLocDeg[0]);
	FLIP4f(&phdr->r.modelLocDeg[1]);
	FLIP4i(&phdr->r.secMid);
	FLIP4f(&phdr->errAzRd);
	FLIP4f(&phdr->errZaRd);
	bitfieldflip((unsigned int *)&phdr->stat,"9,1,1,1,1,4,4,4,1,1,1,1,1,1,1");
	return;
}

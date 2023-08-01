
/***************
 * swaphsps - swap sps hdr from big to little endian.
 * SYNTAX: swaphsps(psps);
 * ARGS  :
 * psps  : *HDRAO_SPSBG ptr sps hdr block structure
 *
 *DESCRIPTION: 
 * 	swap the spsBg hdr structure from big to little endian.
 *It does the flipping inplace. This is the structure
 *stored in the sps part of the  ao headers
 *
*/
#include	<utilLib.h>
#include    <aovmehdrLib.h>

#define FLIP2i(val)  byteswap_2i(val)
#define FLIP2iu(val) byteswap_2iu(val)
#define FLIP4i(val) byteswap_4i(val)
#define FLIP4f(val) byteswap_4f(val)
#define FLIP8d(val) byteswap_8d(val)
#define FLIP_RCV_WIN(val) \
          			FLIP4f(val.startUsec); \
          			FLIP4i(val.numSamples); \
          			FLIP4i(val.numSamplesCal)
              

void	swaphsps(HDRAO_SPSBG *phdr)
{
	int i;
    FLIP4f(&phdr->ipp);
    FLIP4f(&phdr->gw);
    FLIP4f(&phdr->baudLen);
    FLIP4f(&phdr->bwCodeMhz);
    FLIP4f(&phdr->codeLenUsec);
    FLIP4f(&phdr->txIppToRfOn);
    FLIP4f(&phdr->rfLen);
    FLIP4i(&phdr->numRfPulses);
    FLIP4f(&phdr->mpUnit);
	for (i=0;i<HDRAO_SPSBG_MAX_MP_SEQ;i++) FLIP4i(&phdr->mpSeq[i]);
    FLIP4i(&phdr->smpInTxPulse);
    FLIP4i(&phdr->numRcvWin);
	for (i=0;i<HDRAO_SPSBG_MAX_RCV_WIN;i++) {
      		FLIP4f(&phdr->rcvWin[i].startUsec);
      		FLIP4i(&phdr->rcvWin[i].numSamples);
      		FLIP4i(&phdr->rcvWin[i].numSamplesCal);
	}
	return;
}

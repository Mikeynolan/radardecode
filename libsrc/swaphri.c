
/***************
 * swaphri - swap sps ri from big to little endian.
 * SYNTAX: swaphri(pri);
 * ARGS  :
 * pri   : *HDRAO_RI ptr ri hdr block structure
 *
 *DESCRIPTION: 
 * 	swap the ri hdr structure from big to little endian.
 *It does the flipping inplace. This is the structure
 *stored in the ri (input) part of the  ao headers
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
              

void	swaphri(HDRAO_RI *phdr)
{
    FLIP4i(&phdr->extTiming);
    FLIP4i(&phdr->smpMode);
    FLIP4i(&phdr->packing);
    FLIP4i(&phdr->muxAndSubCycle);
    FLIP4i(&phdr->fifoNum);
    FLIP4i(&phdr->smpPairIpp);
    FLIP4i(&phdr->ippsPerBuf);
    FLIP4i(&phdr->ippNumStartBuf);
    FLIP4f(&phdr->ipp);
    FLIP4f(&phdr->gw);
    FLIP4i(&phdr->startOn);
    FLIP4i(&phdr->free);
	return;
}

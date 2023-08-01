
/***************
 * swaphcor - swap cor hdr from bit to little endian.
 * SYNTAX: swaphcor(pcor);
 * ARGS  :
 * pagc  : *HDRAO_STD ptr cor hdr block structure
 *
 *DESCRIPTION: 
 * 	swap the cor hdr structure from big to little endian.
 *It does the flipping inplace. This is the structure
 *stored in the interim cor part of the  ao headers
 *
*/
#include	<utilLib.h>
#include    <aovmehdrLib.h>

#define FLIP2i(val)  byteswap_2i(val)
#define FLIP2iu(val) byteswap_2iu(val)
#define FLIP4i(val) byteswap_4i(val)
#define FLIP4f(val) byteswap_4f(val)
#define FLIP8d(val) byteswap_8d(val)

void	swaphcor(HDRAO_COR *phdr)
{
	
	FLIP4i(&phdr->masterClkPeriod);
	FLIP4i(&phdr->dumpLen);
	FLIP4i(&phdr->dumpsPerInteg);
	FLIP4i(&phdr->lagSbcIn);
	FLIP4i(&phdr->lagSbcOut);
	FLIP4i(&phdr->numSbcIn);
	FLIP4i(&phdr->numSbcOut);
	FLIP4i(&phdr->bwNum);
	FLIP4i(&phdr->lagConfig);
	FLIP4i(&phdr->state);
	FLIP4i(&phdr->frqBufThisRec);
	FLIP4i(&phdr->cycleLen);
	FLIP4i(&phdr->boardId);
	FLIP4i(&phdr->numBrdsUsed);
	FLIP4i(&phdr->attnDb[0]);
	FLIP4i(&phdr->attnDb[1]);
	FLIP4f(&phdr->pwrCnt[0]);
	FLIP4f(&phdr->pwrCnt[1]);
	FLIP4f(&phdr->lag0PwrRatio[0]);
	FLIP4f(&phdr->lag0PwrRatio[1]);
	FLIP4f(&phdr->calOff[0]);
	FLIP4f(&phdr->calOff[1]);
	FLIP4f(&phdr->calOn[0]);
	FLIP4f(&phdr->calOn[1]);
	FLIP4i(&phdr->state2);
	FLIP4i(&phdr->baudLen);
	FLIP4i(&phdr->codeLen);
	FLIP4i(&phdr->codeDelay);
	FLIP4i(&phdr->cohAdd);
	return;
}

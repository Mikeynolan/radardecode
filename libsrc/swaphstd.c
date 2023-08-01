
/***************
 * swaphdrstd - swap std hdr from bit to little endian.
 * SYNTAX: swaphstd(pstd);
 * ARGS  :
 * pagc  : *HDRAO_STD ptr std hdr block structure
 *
 *DESCRIPTION: 
 * 	swap the standard hdr structure from big to little endian.
 *It does the flipping inplace. This is the structure
 *stored in the start of the ao headers
 *
*/
#include	<utilLib.h>
#include    <aovmehdrLib.h>

#define FLIP4i(val) byteswap_4i(val)

void	swaphstd(HDRAO_STD *phdr)
{
	
	FLIP4i(&phdr->hdrlen);
	FLIP4i(&phdr->reclen);
	FLIP4i(&phdr->date);
	FLIP4i(&phdr->time);
	FLIP4i(&phdr->expNumber);
	FLIP4i(&phdr->scanNumber);
	FLIP4i(&phdr->recNumber);
	FLIP4i(&phdr->stScanTime);
	FLIP4i(&phdr->sec.inp);
	FLIP4i(&phdr->sec.iflo);
	FLIP4i(&phdr->sec.proc);
	FLIP4i(&phdr->sec.time);
	FLIP4i(&phdr->sec.pnt);
	FLIP4i(&phdr->sec.misc);
	FLIP4i(&phdr->grpNum);
	FLIP4i(&phdr->grpTotRecs);
	FLIP4i(&phdr->grpCurRec);
	FLIP4i(&phdr->azTTD);
	FLIP4i(&phdr->grTTD);
	FLIP4i(&phdr->chTTD);
	FLIP4i(&phdr->posTmMs);
	return;
}

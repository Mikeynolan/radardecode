/***************
 * swapcorhdr - swap entire interim cor hdr 
 * SYNTAX: swapcorhdr(phdr);
 * ARGS  :
 * phdr  : *HDRAO_STD ptr cor hdr block structure
 *
 *DESCRIPTION: 
 * 	swap the entire interrim cor hdr structure from big to little endian.
 *It does the flipping inplace. This is flips all of the structures
 *in the header not just the hdrCor part
 *
*/
#include	<utilLib.h>
#include    <aovmehdrLib.h>

#define FLIP2i(val)  byteswap_2i(val)
#define FLIP2iu(val) byteswap_2iu(val)
#define FLIP4i(val) byteswap_4i(val)
#define FLIP4f(val) byteswap_4f(val)
#define FLIP8d(val) byteswap_8d(val)

void	swapcorhdr(HDRAO_STD *phdr)
{
	swaphstd(phdr);
	swaphcor((HDRAO_COR*)(CORHDR_GET(phdr)));
	swaphpnt((HDRAO_PNT*)(PNTHDR_GET(phdr)));
	swaphdop((HDRAO_DOP*)(HDRAO_DOP_GET(phdr)));
	swaphiflo((HDRAO_IFLO*)(IFLOHDR_GET(phdr)));
	swaphproc((HDRAO_PROC*)(HDRAO_PROC_GET(phdr)));
	return;
}

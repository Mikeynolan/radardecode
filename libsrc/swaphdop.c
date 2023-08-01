
/***************
 * swaphdop - swap dop hdr from bit to little endian.
 * SYNTAX: swaphdop(pdop);
 * ARGS  :
 * pdop  : *HDRAO_DOP ptr dop hdr block structure
 *
 *DESCRIPTION: 
 * 	swap the dop hdr structure from big to little endian.
 *It does the flipping inplace. This is the structure
 *stored in the doppler part of the  ao headers
 *
*/
#include	<utilLib.h>
#include    <aovmehdrLib.h>

#define FLIP2i(val)  byteswap_2i(val)
#define FLIP2iu(val) byteswap_2iu(val)
#define FLIP4i(val) byteswap_4i(val)
#define FLIP4f(val) byteswap_4f(val)
#define FLIP8d(val) byteswap_8d(val)

void	swaphdop(HDRAO_DOP *phdr)
{
	
	FLIP8d(&phdr->dopFactor);
	FLIP8d(&phdr->velOrZ);
	FLIP8d(&phdr->freqBCRest);
	FLIP8d(&phdr->freqOffsets[0]);
	FLIP8d(&phdr->freqOffsets[1]);
	FLIP8d(&phdr->freqOffsets[2]);
	FLIP8d(&phdr->freqOffsets[3]);
	FLIP8d(&phdr->velObsProj);
	FLIP4i(&phdr->tmDop);
	bitfieldflip((unsigned int*)&phdr->stat,"1,4,2,25");
	return;
}

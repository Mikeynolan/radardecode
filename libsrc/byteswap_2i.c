/*
 * byteswap_2i(short *)
 * swap a 2 byte short in place.
*/
#include   <byteswap.h>
#include   <utilLib.h>

void byteswap_2i(short *pval)
{
	unsigned short ush;
	ush= bswap_16(*(unsigned short *)pval);
	*pval= *(short*)&ush;
	return;
}

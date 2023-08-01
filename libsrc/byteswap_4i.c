/*
 * byteswap_4i(int *)
 * swap a 4 byte int in place.
*/
#include	<byteswap.h>
#include <utilLib.h>

void byteswap_4i(int *pval)
{
    unsigned int ui;
    ui=bswap_32(*(unsigned int *)pval);
    *pval= *(int *)&ui;
	return;
}

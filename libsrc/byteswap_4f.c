/*
 * byteswap_4f(float *)
 * swap a 4 byte float in place.
*/
#include	<byteswap.h>
#include <utilLib.h>

void byteswap_4f(float *pval)
{
			
	unsigned int ui;
    ui= bswap_32(*(unsigned int *)pval);
    *pval= *(float*)&ui;
	return;
}

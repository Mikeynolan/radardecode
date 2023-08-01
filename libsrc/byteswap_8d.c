/*
 * byteswap_8(double *)
 * swap a 8 byte double in place.
*/
#include	<byteswap.h>
#include <utilLib.h>

void byteswap_8d(double *pval)
{
     unsigned long long uli;
     uli= bswap_64(*(unsigned long long *)pval);
	 *pval= *(double *)&uli;
	return;
}

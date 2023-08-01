/*
 * byteswap_8i(long int *)
 * swap a 8 byte long  in place.
*/
#include	<byteswap.h>
#include <utilLib.h>

void byteswap_8i(long long *pval)
{
     unsigned long long uli;
     uli= bswap_64(*(unsigned long long *)pval);
	 *pval= *(long long*)&uli;
	return;
}

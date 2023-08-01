#include	<phlibExtras.h>
#include	<string.h>
#include	<aovmehdrLib.h>
//
STATUS	hdrIsHdr(char *phdr)
{
/*
 * 	return true if the thing pointed at by phdr is a real ao header
*/
	int 	i1;
    i1=memcmp(((HDRAO_STD*)phdr)->hdrMarker,HDR_MARKER,strlen(HDR_MARKER));
	return(i1==0);
}

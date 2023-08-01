
/***************
 * swaphproc - swap proc hdr from bit to little endian.
 * SYNTAX: swaphproc(pproc);
 * ARGS  :
 * pproc : *HDRAO_PROC ptr proc hdr block structure
 *
 *DESCRIPTION: 
 * 	swap the proc hdr structure from big to little endian.
 *It does the flipping inplace. This is the structure
 *stored in the .misc section that holds the experiment specific info.
 *
*/
#include	<utilLib.h>
#include    <aovmehdrLib.h>

#define FLIP2i(val)  byteswap_2i(val)
#define FLIP2iu(val) byteswap_2iu(val)
#define FLIP4i(val) byteswap_4i(val)
#define FLIP4f(val) byteswap_4f(val)
#define FLIP8d(val) byteswap_8d(val)

void	swaphproc(HDRAO_PROC *phdr)
{
	int	i;
	
	for (i=0;i<10;i++) FLIP8d(&phdr->dar[i]);
	for (i=0;i<10;i++) FLIP4i(&phdr->iar[i]);
	return;
}

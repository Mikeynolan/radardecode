
/***************
 * swaphiflo - swap iflo hdr from bit to little endian.
 * SYNTAX: swaphiflo(phdr);
 * ARGS  :
 * phdr : *HDRAO_IFLO ptr iflo hdr block structure
 *
 *DESCRIPTION: 
 * 	swap the iflo hdr structure from big to little endian.
 *It does the flipping inplace. This is the structure
 *stored in the iflo part of the  ao headers
 *
*/
#include	<utilLib.h>
#include    <aovmehdrLib.h>

#define FLIP2i(val)  byteswap_2i(val)
#define FLIP2iu(val) byteswap_2iu(val)
#define FLIP4i(val) byteswap_4i(val)
#define FLIP4f(val) byteswap_4f(val)
#define FLIP8d(val) byteswap_8d(val)
#define	NSYN 4

void	swaphiflo(HDRAO_IFLO *phdr)
{
	int i;
	
	bitfieldflip((unsigned int *)&phdr->if1.st1,"5,3,1,1,1,1,1,9,9,1");
	bitfieldflip((unsigned int *)&phdr->if1.st2,"4,4,4,4,1,1,1,1,4,8");
	FLIP8d(&phdr->if1.rfFrq);
	FLIP8d(&phdr->if1.lo1);
	FLIP4f(&phdr->if1.pwrDbm[0]);
	FLIP4f(&phdr->if1.pwrDbm[1]);
	FLIP4i(&phdr->if1.pwrTmStamp);
	FLIP8d(&phdr->if2.synFreq[0]);
	FLIP8d(&phdr->if2.synFreq[1]);
	FLIP8d(&phdr->if2.synFreq[2]);
	FLIP8d(&phdr->if2.synFreq[3]);
	bitfieldflip((unsigned int *)&phdr->if2.st1,"2,1,1,1,1,1,1,4,1,1,2,16");
	for (i=0;i<NSYN;i++){
		bitfieldflip((unsigned int *)&phdr->if2.st4[i],"2,2,2,7,19");
	}
	FLIP4f(&phdr->if2.pwrDbm[0]);
	FLIP4f(&phdr->if2.pwrDbm[1]);
	FLIP4i(&phdr->if2.pwrTmStamp);
	return;
}

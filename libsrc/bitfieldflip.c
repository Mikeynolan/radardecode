/* bitfieldflip - bitflip a bitfield from big to little endian

 *Jeffs comments:
 *There is no -in general- way to bit flip bit fields from Sun to gcc linux.
 *So I do this for now.

 *The string defines how many bits and fields from left to right:
 *So if Phil defines:

 *typedef struct {
 * unsigned int    synDest  :2;
 * unsigned int    mixerCfr :2;
 * unsigned int    ampInpSrc:2;
 * unsigned int    ampExtMsk:7;
 * unsigned int    unused   :19;
 *} IF2_ST_STAT4;
 *The string is "2,2,2,7,19"
 *
 * phils comments:
 * 
 * solaris BE  lays out bit fields starting at the mostSigBit of mostSigByte
 *         the first byte is the msb in BigEndian
 * Linux   LE  lays out bit fields starting with LeastSigBit of LeastSigByte
 *             this is byte 0 of Little Endian (but least not largest bit)
 *
 * let a bitfield definition be:
 * struct {
 *  unsigned int a3 :3,
 *  unsigned int b2 :2,
 *  unsigned int c3 :3,
 *  unsigned int d8 :8, 
 *  unsigned int e8 :8, 
 *  unsigned int f8 :8} val;
 *
 * and let Xn be n bits long
 * Suppose we have a memory layout of:
 * solaris would write this data as:
 *
 * BigEndian starts from the most significant bit and works down.
 *  MSB
 *  B0     B1 B2 B3
 * |a3b2c3|d8|e8|f8
 *
 * LittleEndian starts from the least significant bit and works up.
 * After reading in the BE data it would assign the data to fields
 * as:
 *   val.a3=c3
 *   val.b2=b2
 *   val.c3=a3
 *   ...
 * 
 * if we 4byte swap the intel data after input we have:
 * f8|e8|d8|a3b2c3
 * We can now starting pealing the data off from the largest
 * value (now a3) and stuff it into the lowest values
 *
 * This routine should probably on be compiled on linux  since the
 * byteswap.h macros aren't on solaris
 *
*/
#include <stdlib.h>
#include <byteswap.h>
#include <bitmasks.h>
#include <string.h>
#include <utilLib.h>

void bitfieldflip( unsigned int *pvalue, char *str )
{
  char *delim = ",";
  char *tok, *tokst;
  unsigned int newvalue,lmask,value,fldLen;
  unsigned int bitRStShift,bitREndShift;/* right bit start/end shift*/
	

  value = bswap_32(*pvalue);		/* swap the bytes*/
  tokst = (char *)malloc( strlen(str)+1);
  memcpy( tokst, str, strlen(str)+1 );
  newvalue=0;

  tok = strtok( tokst, delim );
  bitRStShift  = 32;		/* pealing off from high side*/
  bitREndShift = 0;		    /* putting to low side*/
  while( tok ) {
    fldLen = atoi(tok);
	bitRStShift-=fldLen;		/* right edge of field before shift*/

	/*  mask where the data will end up */

    lmask = bitmasks_4[fldLen] << bitREndShift; /*right edge after shift*/
	/*
 	* shift the data to be aligned with final mask
	 * may have to shift left or right
	*/ 
	if (bitRStShift > bitREndShift) { /* shifting right*/
	   newvalue |= (value >> (bitRStShift-bitREndShift)) & lmask;	
	} else {
	   newvalue |= (value << (bitREndShift-bitRStShift)) & lmask;	
	}
    bitREndShift+= fldLen;

    tok = strtok(NULL, delim);
  }

  free(tokst);
  *pvalue = newvalue;
}


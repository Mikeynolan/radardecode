/***************
 * swaphdr - swap entire vme hdr
 * SYNTAX: swaphdr(phdr);
 * ARGS  :
 * phdr  : *HDRAO_STD ptr cor hdr block structure
 *
 *DESCRIPTION:
 *  swap the entire vme hdr structure from big to little endian.
 *It does the flipping inplace. This is flips all of the structures
 *in the header not just the hdrCor part. It only flips header sections
 *that it recognizes.
 *
*/
#include    <utilLib.h>
#include    <aovmehdrLib.h>

void    swaphdr(HDRAO_STD *phdr)
{
    unsigned int code;

    swaphstd(phdr);
    code=HDR_S_GET_CODE(phdr->sec.inp);
/*
 * for mikes aeron processed data where he doesn't fill in the seccode
*/
    if ( (code == 0) && ( HDR_S_GET_LEN(phdr->sec.inp) == sizeof(HDRAO_RI))){
       code=HDR_S_RI;
    }

    switch (code) {
        case HDR_S_RI:
              swaphri(RIHDR_GET(phdr));
              break;
        case HDR_S_COR:
              swaphcor(CORHDR_GET(phdr));
              break;
         default:
                if (code != 0) {
                    fprintf(stderr,"unknown sec.inp code: 0x%x\n",phdr->sec.inp);
                    return;
                }
    }
/*
 * iflo
*/
    code=HDR_S_GET_CODE(phdr->sec.iflo);
    switch (code) {
        case HDR_S_IFLO:
            swaphiflo(IFLOHDR_GET(phdr));
            break;
        default:
            if (code != 0) {
                    fprintf(stderr,"unknown sec.iflo code: 0x%x\n",phdr->sec.iflo);
                    return;
            }
    }
/*
 *  processing (dop) header
*/
    code=HDR_S_GET_CODE(phdr->sec.proc);
    switch (code) {
        case HDR_S_DOP:
            swaphdop((HDRAO_DOP_GET(phdr)));
            break;
        default:
            if (code != 0) {
                fprintf(stderr,"unknown sec.proc code: 0x%x\n",phdr->sec.proc);
                return;
            }
    }
/*
 *  time (sps) header
*/
    code=HDR_S_GET_CODE(phdr->sec.time);
    if ( (code == 0) && ( HDR_S_GET_LEN(phdr->sec.time) ==
            sizeof(HDRAO_SPSBG))){
        code=HDR_S_SPSBG;
    }

    switch (code) {
        case HDR_S_SPSBG:
            swaphsps((HDRAO_SPSBG_GET(phdr)));
            break;
        default:
            if (code != 0) {
                fprintf(stderr,"unknown sec.time code: 0x%x\n",phdr->sec.time);
                return;
        }
    }
/*
 *  pnt  header
*/
    code=HDR_S_GET_CODE(phdr->sec.pnt);
    switch (code) {
        case HDR_S_PNT:
            swaphpnt((PNTHDR_GET(phdr)));
            break;
        default:
            if (code != 0) {
                fprintf(stderr,"unknown sec.pnt code: 0x%x\n",phdr->sec.pnt);
                return;
        }
    }
/*
 *  misc  header
*/
    code=HDR_S_GET_CODE(phdr->sec.misc);
    switch (code) {
        case HDR_S_PROC:
            swaphproc((HDRAO_PROC_GET(phdr)));
            break;
        default:
      if (code != 0) {
                fprintf(stderr,"unknown sec.misc code: 0x%x\n",phdr->sec.misc);
                return;
        }
    }
    return;
}


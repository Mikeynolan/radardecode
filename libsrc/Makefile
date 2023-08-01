CFLAGS = -I../include -O3 

SRC =	anglesToVec3.c bitfieldflip.c byteswap_2i.c byteswap_2iu.c byteswap_4f.c byteswap_4i.c byteswap_4iu.c byteswap_8d.c byteswap_8i.c dayNoTodm.c dif0_pi.c dms1_dms3.c dms1_rad.c dms3_rad.c dmToDayNo.c dmy1_dmy3.c dnSecMidCnv.c fmtDms.c fmtHmsD.c fmtHmsI.c fmtRdToDms.c fmtRdToHmsD.c fmtSMToHmsD.c fmtSMToHmsI.c fracToHms3.c hdrIsHdr.c hdrPrAll.c hdrPrCor.c hdrPrDop.c hdrPrIfLo.c hdrPrPnt.c hdrPrProc.c hdrPrRi.c hdrPrSpsBg.c hdrPrStd.c hm2_IminMid.c hms1_hms3.c hms1_IsecMid.c hms1_rad.c hms3_IsecMid.c hms3_rad.c hms3ToFrac.c IminMid_hm2.c IsecMid_hms3.c isLeapYear.c rad_deg.c rad_dms3.c rad_hms3.c sec1970_ymd_secMid.c secMid_hms3.c setSign.c swapcorhdr.c swaphcor.c swaphdop.c swaphdr.c swaphiflo.c swaphpnt.c swaphproc.c swaphri.c swaphsps.c swaphstd.c to0_2pi.c tosecs1970.c vec3ToAngles.c ymd1_ymd3.c

OBJ = $(SRC:.c=.o)

libaohdr.a: $(OBJ)
	@ar rcs libaohdr.a $(OBJ)

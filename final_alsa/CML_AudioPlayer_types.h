/******************************************************************************
 *
 *
 *   ALLGO EMBEDDED SYSTEMS CONFIDENTIAL PROPRIETARY
 *
 *    (C) 2019 ALLGO EMBEDDED SYSTEMS PVT. LTD.
 *
 *   FILENAME        - CML_types.h
 *
 *   COMPILER        - gcc 5.4.0
 *
 ******************************************************************************
 *
 *   CHANGE HISTORY
 *   mm/dd/yy          DESCRIPTION                        Author
 *   --------          -----------                        ------
 *   26/08/2019        Initial Version                    Ankit Raj,Roshini taj
 											   Aishwarya M
 ********************************************************************************
 *
 *   DESCRIPTION
 *   CML_AudioPlayer internals functions  header
 *
 ******************************************************************************/
#ifndef CML_AUDIOPLAYER_TYPES_H_

#define CML_AUDIOPLAYER_TYPES_H_

#include"CML_types.h"
#include "cml_error_codes.h"

eRetType cml_checkinput(CML_UINT32 iNoOfargs,CML_CHAR *pacargslist[]);
eRetType cml_myplaylist(CML_INT32 iNoOfargs,CML_CHAR *pacargslist[],CML_INT32 eshufflag , scmlmyplaylist samyplaylist[] , scmlaudioinfo *spfileinfo); 
CML_VOID cml_shuffle( scmlmyplaylist spmyplaylist[] , scmlaudioinfo *spfileinfo);
CML_VOID cml_extractheaderinfo(scmlwavheaderinfo *spsetconfig , scmlaudioinfo *spfile);

#endif

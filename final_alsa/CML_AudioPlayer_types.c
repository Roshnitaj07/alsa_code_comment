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
 *  CHANGE HISTORY
 *   ---------------------------------------------------------------------------
 *   DATE           REVISION      		AUTHOR                  COMMENTS
 *   ---------------------------------------------------------------------------
 *   26/08/2019     1.0         	Ankit Raj,Roshini         Initial Version
 *   					        	Taj,Aishwarya M
 *  
 ********************************************************************************
 *   DESCRIPTION
 *   This file contains all audio player related internal header functions implimentation.
 *   
 ******************************************************************************/
#include<strings.h>
#include<stdlib.h>
#include"cml_error_codes.h"
#include"CML_types.h"
#include"CML_AudioPlayer_types.h"
#include<signal.h>
#define MAX_AUDIO_FILES_RANGE 10000

CML_VOID swap (scmlmyplaylist *splist1, scmlmyplaylist *splist2)  
{ // printf("in swap\n");
    scmlmyplaylist temp = *splist1;  
    *splist1 = *splist2; 
    *splist2 = temp;  
}
CML_VOID cml_shuffle( scmlmyplaylist samyplaylist[] , scmlaudioinfo *spfileinfo)
{
    srand (time(NULL));  
int i,j;
    //printf("in shuffle\n");
    for (i = spfileinfo->icount - 1; i >= 0; i--)  
    {
         j = rand() % (i + 1); 
        swap(&samyplaylist[i], &samyplaylist[j]);  
    }   
}


eRetType cml_checkinput(CML_UINT32 iNoOfargs,CML_CHAR *pacargslist[])
{
    CML_CHAR *pc;
    eRetType ciret=E_CML_SUCCESS;
    //printf("in check input\n");
    if(iNoOfargs < 2)
    {
        ciret = E_CML_NO_ARGUMENTS;
        //kill(getpid(),SIGINT);
    }
    else
    {
        if((strcmp(pacargslist[1],"-s"))== 0 || (strcmp(pacargslist[1],"-S"))== 0)
        {  // printf("hi\n");
            pc=strrchr(pacargslist[2],'.');
if(NULL == pc)
printf("error in extension\n");
 else if(strcmp(pc,".wav")==0 || strcmp(pc,".txt")==0)
            {   //printf("hi\n");
                ciret = E_CML_SHUFFLE;
            }
            else if(NULL == pacargslist[2])
            {
                ciret = E_CML_INVALID_ARGUMENTS_PASSED ;
                //kill(getpid(),SIGINT);                
            }
        }
        else
        {
            pc=strrchr(pacargslist[1],'.');
   if(NULL == pc)
printf("error in extension\n");
 else if(strcmp(pc,".wav")==0 || strcmp(pc,".txt")==0)
            {
                ciret = E_CML_SUCCESS;
            }
            else
            {
                ciret = E_CML_INVALID_ARGUMENTS_PASSED;
            }
            
        }
    }
    return ciret;
}


eRetType cml_myplaylist(CML_INT32 iNoOfargs , CML_CHAR *pacargslist[] , CML_INT32 eshufflag , scmlmyplaylist samyplaylist[] , scmlaudioinfo *spfileinfo)
{
    eRetType mpret;
    CML_FILE *fp;
    //CML_CHAR str[256]; 
    CML_INT32 i,j=1;
    spfileinfo->icount=0;
    CML_CHAR *pfext,fline[256];
    CML_CHAR clen=0;
    mpret = E_CML_SUCCESS;
    
    if(eshufflag)
    {
        j=2;
    }

    for(i=j;i<iNoOfargs;i++)
    {
        pfext=strrchr(pacargslist[i],'.');
   if(NULL == pfext)
printf("error in extension\n");
 else if(strcmp(pfext,".wav")==0)
        {
            clen=strlen(pacargslist[i]);
            //printf("%d%d",clen,sizeof(pacargslist[i]));
            samyplaylist[spfileinfo->icount].pfilepath=(char*)malloc((clen)*sizeof(char));
            if(NULL == samyplaylist[spfileinfo->icount].pfilepath)
            {
                mpret = E_CML_MEMORY_NOT_ALLOCATED;
                continue;
            }
            strncpy(samyplaylist[spfileinfo->icount].pfilepath,pacargslist[i],clen+1);
            spfileinfo->icount++;
            clen=0;
        }
        else if(strcmp ( pfext,".txt" ) == 0)
        {
            fp=fopen(pacargslist[i],"r");
            if(NULL == fp)
            {
                printf("couldn,t open file empty file at %dth arguments\n",i);
                continue;
            }
            else
            {
                while(fgets ( fline, sizeof (fline), fp ) != NULL)
                {
                    if(strcmp( fline,"\n")!=0)
                    {
                        pfext=strrchr(fline, '.');
       if(NULL == pfext)
printf("error in extension\n");
 else if(strcmp(pfext, ".wav\n") == 0)
                        {
                            fline[strlen(fline)-1]='\0';
                            clen=strlen(fline);
                            //printf("%c\n",clen);
                            samyplaylist[spfileinfo->icount].pfilepath=(char*)malloc((clen+1)*sizeof(char));
                            if(NULL == samyplaylist[spfileinfo->icount].pfilepath)
                            {
                                mpret = E_CML_MEMORY_NOT_ALLOCATED;
                                continue;
                            }
                            strncpy(samyplaylist[spfileinfo->icount].pfilepath, fline,clen+1);
                            spfileinfo->icount++;
                        }
                    }
                }
            }
            fclose(fp);
        }
        else
        {
            printf("invalid file extension at %dth arguments\n",i);
        }
        
        if(spfileinfo->icount >= MAX_AUDIO_FILES_RANGE)
        {
            mpret = E_CML_CANNOT_CREATE_PLAYLIST_EXCEEDED_AUDIO_RANGE;
            return mpret;
        }
        clen=0;
    }
    for(i=0;i<spfileinfo->icount;i++)
    {
    printf("%s\n",samyplaylist[i].pfilepath);
    }
    printf("NO of AudioFiles:%d\n",spfileinfo->icount);
    
    return mpret;         
}


CML_VOID cml_extractheaderinfo(scmlwavheaderinfo *spsetconfig , scmlaudioinfo *spfile)
{
    
    CML_CHAR *cheader;
    cheader=(CML_CHAR*)malloc(44);
  
    if(NULL == cheader)
    {
        printf("CML MEMORY NOT ALLOCATED");
    }

   
    if(read(spfile->ifiledesc,cheader,44) != 44)
    {
        printf("Unable to read header data\n");
        
    }
    /*overall size*/
    spsetconfig->isize=(*(CML_UINT32 *)(cheader+4));

    /*no of channels*/
    spsetconfig->uwchannel=(*(CML_UINT32 *)(cheader+22));

    /*Sample Rate*/
    spsetconfig->isamplerate=(*(CML_UINT32 *)(cheader+24));

    /*BYTE RATE*/
    spsetconfig->ibyterate=(*(CML_UINT32 *)(cheader+28));

    /*Time*/
    spsetconfig->iduration= (int)(spsetconfig->isize)/(spsetconfig->ibyterate);

    //close(iheader);
    free(cheader);

}

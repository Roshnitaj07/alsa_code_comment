#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "CML_types.h"
//#include "check_format.h"

void swap (struct CML_myplaylist *a, struct  CML_myplaylist *b)  
{  
  struct  CML_myplaylist temp = *a;  
    *a = *b; 
    *b = temp;  
}
void cml_shuffle(CML_UINT32 count)
{
  srand (time(NULL));  
  for (int i = count - 1; i >= 0; i--)  
    {
   int j = rand() % (i + 1); 
          swap(&samyplaylist[i], &samyplaylist[j]);  
    }   
}

CML_UINT32 cml_checkinput(CML_UINT32 iNoOfargs,CML_CHAR *pacargslist[])
{
    CML_UINT32 i;
    CML_CHAR *pc;
    if(iNoOfargs<2)
    {
        printf("NO ARGUMENTS PASSED ,ENTER ARGUMENTS as :<-s><.wav files><.txt files>");
        return -1;
    }
    else
    {
        if((strcmp(pacargslist[1],"-s"))==0 || (strcmp(pacargslist[1],"-S"))==0)
        
            {
                pc=strrchr(pacargslist[2],'.');
                if(strcmp(pc,".wav")==0 || strcmp(pc,".txt")==0)
                {
                   return 1;
                }
                else
                {
                    printf("give valid inputs\n");
                    return 1;
                }
                   
            }
        else
        {
            pc=strrchr(pacargslist[1],'.');
            if(strcmp(pc,".wav")==0 || strcmp(pc,".txt")==0)
            {
                return 0;
            }
            

        }
    }
}

CML_INT32 main(CML_INT32 iNoOfargs,CML_CHAR *pacargslist[])
{
     CML_FILE *fp;
     CML_UINT32 j;
     CML_UINT32 ishuffle=cml_checkinput(iNoOfargs,pacargslist);
     if(ishuffle== -1)
     {
        return 0;
     }
     else if(1 == ishuffle)
     {
         j=2;
     }
     else
     {
         j=1;
     }

     CML_CHAR str[256]; 
     CML_INT32 i,count=0;
     CML_INT32 pcount=0;
     CML_CHAR *pfext,fline[256];
     CML_CHAR clen;
     /*len=strlen(argv[1]);
     printf("%d\n",len);*/
     for(i=j;i<iNoOfargs;i++)
     {
         pfext=strrchr(pacargslist[i],'.');
         if(strcmp(pfext,".wav")==0)
           
         {
             
             clen=strlen(pacargslist[i]);
             samyplaylist[count].pfilepath=(char*)malloc((clen+1)*sizeof(char));
             strcpy( samyplaylist[count++].pfilepath,pacargslist[i]);
               
         }
         else if(strcmp ( pfext,".txt" ) == 0)
         {
             fp=fopen(pacargslist[i],"r");
             if(NULL == fp)
             {
                 printf("couldn,t open file empty file at %dth arguments\n",i);
                 
             }
             else
             {
                
                 while(fgets ( fline, sizeof (fline), fp ) != NULL)
                 {
                     if(strcmp( fline,"\n")!=0)
                     {
                         pfext=strrchr(fline, '.');
                         if(strcmp(pfext, ".wav\n") == 0)
                         {
                             fline[strlen(fline)-1]='\0';
                             clen=strlen(fline);
                             samyplaylist[count].pfilepath=(char*)malloc((clen+1)*sizeof(char));
                             strcpy( samyplaylist[count++].pfilepath, fline);
                             
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
     }
      for(i=0;i<count;i++)
     {
         printf("%s\n",samyplaylist[i].pfilepath);
     }
     if(1 == ishuffle)
     {
         printf("After shuffle\n\n");
         cml_shuffle(count);
         for(i=0;i<count;i++)
        {
             printf("%s\n",samyplaylist[i].pfilepath);
        }
     }
    return 0;
     
}

#include<stdio.h>
//#include"CML_AudioPlayer_types.h"
#include"CML_types.h"
#include"cml_error_codes.h"
int main(int argc ,char **argv)
{/*int i;
eRetType status,e1;
status=cml_checkinput(argc,argv);
printf("%dhiiii\n",status);
e1=cml_myplaylist(argc,argv,status);
for(i=0;i<sCurFile.icount;i++)
{
printf("%s\n",samyplaylist[i].pfilepath);
}
if(status == E_CML_SHUFFLE)
{
   cml_shuffle();
}
printf("\n\n");
for(i=0;i<sCurFile.icount;i++)
{
printf("%s\n",samyplaylist[i].pfilepath);
}*/
eRetType e1;
int a;
scanf("%d",&a);
if(a==1)
   e1=-41;
else if(a==2)
   e1=-17;
else
   e1=-33;

printf("%s\n",cml_perror(e1));      
}

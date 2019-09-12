/******************************************************************************
 *
 *
 *   ALLGO EMBEDDED SYSTEMS CONFIDENTIAL PROPRIETARY
 *
 *    (C) 2019 ALLGO EMBEDDED SYSTEMS PVT. LTD.
 *
 *   FILENAME        - test.c
 *
 *   COMPILER        - gcc 5.4.0
 *
 ******************************************************************************
 *  CHANGE HISTORY
 *   ---------------------------------------------------------------------------
 *   DATE           REVISION      		AUTHOR                  COMMENTS
 *   ---------------------------------------------------------------------------
 *   26/08/2019     1.0             Ankit Raj,Roshni         Initial Version
 *                                  Taj,Aishwarya M
 *     
 ********************************************************************************
 *   DESCRIPTION
 *   This file contains all audio player related interfaces apllication.
 *   
 ******************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include <alsa/asoundlib.h> 
#include <stdio.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include<signal.h>
#include"CML_AudioPlayer_Interfaces.h"
#include"cml_error_codes.h"

#define INC_VOLUME 10
#define DEC_VOLUME 10
#define MUTE 0
#define MAX_VOLUME 100
char getkey();
void cml_help();
struct time
{
    int h,m,s;
};

CML_UINT32 sec_to_time(CML_UINT32 sec)
{
 struct time sectime;
    sectime.h = (sec/3600); 
    sectime.m = (sec-(3600*sectime.h))/60;
    sectime.s = (sec -(3600*sectime.h)-(sectime.m*60));
    printf("HH:MM:SS - %u:%u:%u\n",sectime.h,sectime.m,sectime.s);
    return 0;
}

CML_UINT32 time_to_sec(struct time stime1)
{
    CML_UINT32 timeinsec =(CML_UINT32) stime1.s + (stime1.m * 60) + (stime1.h * 60 * 60);
    return timeinsec;
}
CML_VOID display_info(scmlplayer *spdisplay)
{
   
   printf("File Name:%s\n",spdisplay->samyplaylist[spdisplay->spfileinfo->icurfileindex].pfilepath);
   printf("Overall Size:%u\n", spdisplay->spwavheaderinfo->isize);
 
    printf("Channel:%u", spdisplay->spwavheaderinfo->uwchannel);
      if(1==spdisplay->spwavheaderinfo->uwchannel)
         printf("(MONO)\n");
     else 
         printf("(Stereo)\n");
        /*Sample Rate*/
     printf("Sample rate:%u\n",spdisplay->spwavheaderinfo->isamplerate);
    
    /*Time*/
    printf("Duration in sec:%u\n",spdisplay->spwavheaderinfo->iduration);
 sec_to_time(spdisplay->spwavheaderinfo->iduration);
}
/*CML_VOID sig_kil_handler(int sig)
{
    printf("Interrupt signal\n");
    exit(0);
}*/
int main(int argc,char **argv)
{
    eRetType estatus;
    CML_UINT32 isec;
    CML_CHAR ch;
    static CML_LONG ivolume = 50;
    struct time sTime;
   // signal(SIGINT,sig_kil_handler);
    scmlplayer *spmyplayer = (scmlplayer *)malloc(sizeof(scmlplayer));
    if(NULL == spmyplayer)
    {
        printf("Memory not allocated");
        
    }
    estatus = cml_audioplayer_init(argc , argv , spmyplayer);
    //display_info(spmyplayer);
    
    if (estatus != E_CML_SUCCESS)
    {
        printf("%s\n", cml_perror(estatus));
    }
     //display_info(spmyplayer);
    //printf("called init\n");
   
    do
    {
        ch = getkey();
        switch(ch)
        {
            case 'P'://printf("calling pause\n");
            case 'p':estatus = cml_audioplayer_pause_resume(spmyplayer);
                     if(estatus != E_CML_SUCCESS)
                     {
                         printf("%s\n",cml_perror(estatus));
                     }
                     //printf("called pause\n");
                     break;
            case 'f'://printf("calling seekf\n");
            case 'F':estatus = cml_audioplayer_seek_forward(spmyplayer);
                     if(estatus != E_CML_SUCCESS)
                     {
                         printf("%s\n",cml_perror(estatus));
                     }
                     //printf("called seekf\n");
                     break;
            case 'g'://printf("calling seekb\n");
            case 'G':estatus = cml_audioplayer_seek_back(spmyplayer);
                     if(estatus != E_CML_SUCCESS)
                     {
                         printf("%s\n",cml_perror(estatus));
                     }
                     //printf("called seekb\n");
                     break;
            case 's'://printf("calling stop\n");
            case 'S':estatus = cml_audioplayer_playback_stop(spmyplayer);
                     if(estatus != E_CML_SUCCESS)
                     {
                         printf("%s\n",cml_perror(estatus));
                     }
                     //printf("called stop\n");
                     break;
            case 'w'://printf("calling play\n");
            case 'W':estatus = cml_audioplayer_play(spmyplayer);
                     if(estatus != E_CML_SUCCESS)
                     {
                         printf("%s\n",cml_perror(estatus));
                     }
                     //printf("called play\n");
                     break;
            case '+':if(ivolume == MAX_VOLUME)
                     {
                         printf("Maximum volume reached\n");
                         break;
                     }
                     ivolume += INC_VOLUME;
                     cml_audioplayer_volume_control(ivolume);
                     break;
            case '-': if(ivolume == MUTE)
                     {
                         printf("Minimum volume reached\n");
                         break;
                     }
                     ivolume-=DEC_VOLUME;
                     cml_audioplayer_volume_control(ivolume);
                     break;                     
            case 'j':
            case 'J':if(STOP == spmyplayer->icurstate)
                     {
                         printf("CANNOT_SEEKTO_IN_STOP_STATE\n");
                     }
                     else
                     {
                         int sval=scanf("%2d%*c%2d%*c%2d",&sTime.h,&sTime.m,&sTime.s);
                         if(0 == sval)
                          printf("unable to read seek_value\n");
                        else
                        {
                         isec = time_to_sec(sTime);
                         estatus = cml_audioplayer_seek_to(isec, spmyplayer);
                         if(estatus !=E_CML_SUCCESS)
                         {
                             printf("%s\n",cml_perror(estatus));
                         }
                     }
                     }
                     break;
            case 'n':
            case 'N':estatus = cml_audioplayer_play_next_file(spmyplayer);
                     if(estatus != E_CML_SUCCESS)
                     {
                         printf("%s\n",cml_perror(estatus));
                     }
              // display_info(spmyplayer);
            break;

            case 'b':
            case 'B':estatus = cml_audioplayer_play_prev_file(spmyplayer);
                     if(estatus != E_CML_SUCCESS)
                     {
                         printf("%s\n",cml_perror(estatus));
                     }
                  //display_info(spmyplayer);
                     break;
            case 'H':
            case 'h':cml_help();
                     break;
                   
        }
    }
        while(ch != 'q' && ch != 'Q');
        cml_audioplayer_playback_stop(spmyplayer);
        cml_audioplayer_dinit(spmyplayer);
        printf("END\n");
        
    return 0;
}

char getkey() 
{
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror ("tcsetattr ~ICANON");
    return (buf);
}
void cml_help()
{ 
    printf("=====================================CML_AUDIO_PLAYER_OPTIONS===================================\n");
    printf("\nW|w:Start the Playback\nP|p:Pause_And_Resume\nS|s:Stop_The_Playback\nN|n:Play_The_Next_Track\nB|b:Play The Prevoius Track\nF|f:SeekForward_BY_10Sec\nG|g:SeekBackward_BY_10Sec\nJ|j<HH:MM:SS>:SeekTo_Particular_TimeStamp\n+:Increase_The_Volume_By_10Per\n-:Decrease_The_Volume_By_10Per\nQ|q:To_Quit_the_Playback_From_Any_State\n");
printf("================================================================================================\n");
     }


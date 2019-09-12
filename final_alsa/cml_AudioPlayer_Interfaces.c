/******************************************************************************
 *
 *
 *   ALLGO EMBEDDED SYSTEMS CONFIDENTIAL PROPRIETARY
 *
 *    (C) 2019 ALLGO EMBEDDED SYSTEMS PVT. LTD.
 *
 *   FILENAME        - CML_AudioPlayer_Interfaces.c
 *
 *   COMPILER        - gcc 5.4.0
 *
 ******************************************************************************
 *  CHANGE HISTORY
 *   ---------------------------------------------------------------------------
 *   DATE           REVISION      		AUTHOR                  COMMENTS
 *   ---------------------------------------------------------------------------
 *   26/08/2019     1.0             Ankit Raj,Roshni         Initial Version
 *   					  	        Taj,Aishwarya M
 *  
 ********************************************************************************
 *   DESCRIPTION
 *   This file contains all audio player related API implimentation.
 *   
 ******************************************************************************/
#define _LARGEFILE64_SOURCE
#include"CML_types.h"
#include"CML_AudioPlayer_Interfaces.h"
#include"CML_AudioPlayer_types.h"
#include"cml_error_codes.h"
#include<signal.h>


CML_VOID *cml_playbackthread(CML_VOID *args);
CML_VOID setalsaconfig(scmlplayer *spsetconfig);
//eRetType cml_myplaylist(CML_UINT32 iNoOfargs,CML_CHAR *pacargslist[]);

void psetstate(CML_playerstate ecurstate , scmlplayer *spsetstate)
{	
    switch(ecurstate)
    {
        case STOP_STATE:spsetstate->icurstate = STOP;break;
        case PLAY_STATE:spsetstate->icurstate = PLAY;break;
        case PAUSED_STATE:spsetstate->icurstate = PAUSE;break;
    }
}

eRetType cml_audioplayer_init(CML_UINT32 iNoOfargs,CML_CHAR *pacargslist[], scmlplayer *spinitplay)
{   
    eRetType iret;
    CML_INT32 ishuffle = 0;
    spinitplay->spalsaconfig = (scmlalsaconfig *)malloc(sizeof(scmlalsaconfig));
   if(NULL == spinitplay->spalsaconfig)
  printf("Memory error\n");
    spinitplay->spwavheaderinfo = (scmlwavheaderinfo *)malloc(sizeof(scmlwavheaderinfo));
  if(NULL ==   spinitplay->spwavheaderinfo)
  printf("Memory error\n");
    spinitplay->spfileinfo = (scmlaudioinfo *)malloc(sizeof(scmlaudioinfo));
  if(NULL ==spinitplay->spfileinfo )
  printf("Memory error\n");
    iret = E_CML_SUCCESS;

    ishuffle = cml_checkinput(iNoOfargs,pacargslist);
    if(ishuffle < 0)
    {
        iret = E_CML_INVALID_ARGUMENTS_PASSED;
        
    }
  
    if(iret = cml_myplaylist(iNoOfargs , pacargslist , ishuffle , spinitplay->samyplaylist , spinitplay->spfileinfo) != E_CML_SUCCESS)
    {
        iret = E_CML_ERROR_CREATING_PLAYLIST;
    }
    if(ishuffle == E_CML_SHUFFLE)
    {
        cml_shuffle(spinitplay->samyplaylist , spinitplay->spfileinfo);
    }
    spinitplay->fpsetstate = psetstate;
    spinitplay->spfileinfo->icurfileindex = 0;
    spinitplay->spfileinfo->seconds = 0;
    setalsaconfig(spinitplay);
    return iret;
}

eRetType cml_audioplayer_play(scmlplayer *spplay)
{
    eRetType pret;
    pret = E_CML_SUCCESS;
    if(STOP == spplay->icurstate)
    {
        setalsaconfig(spplay);
    }
    else
    {
        pret = E_CML_INVALID_KEY_PRESSED;
    }

    return pret;
}

eRetType cml_audioplayer_pause_resume(scmlplayer *sppause)
{
    eRetType prret=E_CML_SUCCESS;
    if(sppause->icurstate == PLAY)
    {
        if(snd_pcm_pause(sppause->spalsaconfig->pcmhandle, 1) < 0)
        { 
            prret = E_CML_ERROR_WHILE_PAUSE;

        }
      else
       {

        prret = E_CML_SUCCESS;
        sppause->fpsetstate(PAUSED_STATE , sppause);

    }
    }
    else if(sppause->icurstate == PAUSE)
    {
        if(snd_pcm_pause(sppause->spalsaconfig->pcmhandle , 0) < 0)
        {
            prret = E_CML_ERROR_WHILE_RESUME;
        }
        else
       { 
        prret = E_CML_SUCCESS;
        sppause->fpsetstate(PLAY_STATE , sppause);
    }
    }
    else if(sppause->icurstate == STOP)
    {
        prret = E_CML_CANNOT_PAUSE_IN_STOP_STATE;
    }
    return prret;
}

eRetType cml_audioplayer_volume_control(CML_LONG volume)
{ 

    CML_INT64 min, max;
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    const char *card = "default";
    const char *selem_name = "Master";

    snd_mixer_open(&handle, 0);
    snd_mixer_attach(handle, card);
    snd_mixer_selem_register(handle, NULL, NULL);
    snd_mixer_load(handle);

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);
    snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
    snd_mixer_selem_set_playback_volume_all(elem, volume * max / 100);

    snd_mixer_close(handle);
    return E_CML_SUCCESS;
}

CML_VOID setalsaconfig(scmlplayer *spsetconfig)
{
    CML_UINT32 pcm;	
    spsetconfig->spfileinfo->ifiledesc = open(spsetconfig->samyplaylist[spsetconfig->spfileinfo->icurfileindex].pfilepath ,
            O_RDONLY);   

    
    cml_extractheaderinfo(spsetconfig->spwavheaderinfo , spsetconfig->spfileinfo);

   
    if (pcm = snd_pcm_open(&spsetconfig->spalsaconfig->pcmhandle, "default",
                SND_PCM_STREAM_PLAYBACK, 0) < 0) 
        printf("ERROR: Can't open \"%s\" PCM device. %s\n",
                "default", snd_strerror(pcm));

    /* Allocate parameters object and fill it with default values*/
    snd_pcm_hw_params_alloca(&spsetconfig->spalsaconfig->params);

    snd_pcm_hw_params_any(spsetconfig->spalsaconfig->pcmhandle, spsetconfig->spalsaconfig->params);

    /* Set parameters */
    if (pcm = snd_pcm_hw_params_set_access(spsetconfig->spalsaconfig->pcmhandle,
                spsetconfig->spalsaconfig->params,SND_PCM_ACCESS_RW_INTERLEAVED) < 0) 
        printf("ERROR: Can't set interleaved mode. %s\n", snd_strerror(pcm));

    if (pcm = snd_pcm_hw_params_set_format(spsetconfig->spalsaconfig->pcmhandle, 
                spsetconfig->spalsaconfig->params,	SND_PCM_FORMAT_S16_LE) < 0) 
        printf("ERROR: Can't set format. %s\n", snd_strerror(pcm));

    if (pcm = snd_pcm_hw_params_set_channels(spsetconfig->spalsaconfig->pcmhandle, 
                spsetconfig->spalsaconfig->params, spsetconfig->spwavheaderinfo->uwchannel) < 0) 
        printf("ERROR: Can't set swaveheaderinfo.uwchannel number. %s\n", 
                snd_strerror(pcm));

    if (pcm = snd_pcm_hw_params_set_rate_near(spsetconfig->spalsaconfig->pcmhandle, 
                spsetconfig->spalsaconfig->params, &spsetconfig->spwavheaderinfo->isamplerate, 0) < 0) 
        printf("ERROR: Can't set swaveheaderinfo.isamplerate. %s\n",
                snd_strerror(pcm));

    /* Write parameters */
    if (pcm = snd_pcm_hw_params(spsetconfig->spalsaconfig->pcmhandle, spsetconfig->spalsaconfig->params) < 0)
        printf("ERROR: Can't set harware parameters. %s\n", snd_strerror(pcm));

    /* Resume information */
  

    snd_pcm_hw_params_get_channels(spsetconfig->spalsaconfig->params, &spsetconfig->spfileinfo->tmp);
   

    snd_pcm_hw_params_get_rate(spsetconfig->spalsaconfig->params, &spsetconfig->spfileinfo->tmp, 0);
  
   

    /* Allocate buffer to hold single period */
    snd_pcm_hw_params_get_period_size(spsetconfig->spalsaconfig->params, &spsetconfig->spalsaconfig->frames, 0);

    if(pthread_create(&spsetconfig->spfileinfo->playbacktid, NULL, cml_playbackthread, spsetconfig) < 0)
    {   
     printf("error while creating thread\n");
    }
    



}

CML_VOID *cml_playbackthread(CML_VOID *args)
{
  //  int i=0;
    scmlplayer *spthread = (scmlplayer *)args;
    spthread->fpsetstate(PLAY_STATE , spthread);
    CML_INT32 pcm,buff_size,loops;

    buff_size = spthread->spalsaconfig->frames * spthread->spwavheaderinfo->uwchannel * 2 
        /* 2 -> sample size */;
    spthread->spalsaconfig->buffer = (char *) malloc(buff_size);
    snd_pcm_hw_params_get_period_time(spthread->spalsaconfig->params, &spthread->spfileinfo->tmp, NULL);


    for (loops = (spthread->spwavheaderinfo->iduration * 1000000) / spthread->spfileinfo->tmp; loops > 0; loops--) 
    {

        if (pcm = read(spthread->spfileinfo->ifiledesc, spthread->spalsaconfig->buffer, buff_size) == 0) 
        {
            printf("Early end of file.\n");
            
        }

        if (pcm = snd_pcm_writei(spthread->spalsaconfig->pcmhandle, spthread->spalsaconfig->buffer, 
                    spthread->spalsaconfig->frames) == -EPIPE) 
        {
            printf("XRUN.\n");
            snd_pcm_prepare(spthread->spalsaconfig->pcmhandle);
        }
        else 
        {
            printf("ERROR. Can't write to PCM device. %s\n", snd_strerror(pcm));
        }
       

    }
  return 0;
    
}

eRetType cml_audioplayer_play_next_file(scmlplayer *spnext)
{
    eRetType pnret;
    if(STOP == spnext->icurstate)
    {
        pnret = E_CML_NEXT_INVALID_IN_STOP_STATE;
    }
    else if(spnext->spfileinfo->icurfileindex == spnext->spfileinfo->icount-1)
    {
            pnret = E_CML_NEXT_INVALID_FOR_LAST_AUDIO;
           //kill(getpid(),SIGINT);
    }
    else
    {
        snd_pcm_drop(spnext->spalsaconfig->pcmhandle);
        pthread_cancel(spnext->spfileinfo->playbacktid);
        /*if(spnext->spfileinfo->icurfileindex == spnext->spfileinfo->icount)*/
        
        spnext->spfileinfo->icurfileindex += 1;

        setalsaconfig(spnext);
        pnret = E_CML_SUCCESS;
    }
    return pnret;
}


eRetType cml_audioplayer_play_prev_file(scmlplayer *spprev) 
{
    eRetType ppret;
    if(STOP == spprev->icurstate)
    {
        ppret = E_CML_PREVIOUS_INVALID_IN_STOP_STATE;
    }
    else  if(0 == spprev->spfileinfo->icurfileindex )
    {
        ppret = E_CML_PREVIOUS_INVALID_FOR_FIRST_AUDIO;
        // kill(getpid(),SIGINT);
    }
    else
    {
        snd_pcm_drop(spprev->spalsaconfig->pcmhandle);
        pthread_cancel(spprev->spfileinfo->playbacktid);
        spprev->spfileinfo->icurfileindex -= 1;
        setalsaconfig(spprev);
        ppret = E_CML_SUCCESS;
    }
    return ppret;
}

eRetType cml_audioplayer_playback_stop(scmlplayer *spstop)
{ 
    spstop->fpsetstate(STOP_STATE , spstop);
    pthread_cancel(spstop->spfileinfo->playbacktid);
    //snd_pcm_drain(spalsaconfig->pcmhandle);
    snd_pcm_drop(spstop->spalsaconfig->pcmhandle);
    //pthread_cancel(spfileinfo->playbacktid);
    return E_CML_SUCCESS;
}
eRetType cml_audioplayer_seek_back(scmlplayer *spseekb)
{ 
    eRetType sbret = E_CML_SUCCESS;
    if(STOP == spseekb->icurstate)
    {
        sbret = E_CML_CANNOT_SEEKBACKWARD;
    }
    else
    {
        pthread_cancel(spseekb->spfileinfo->playbacktid);
        snd_pcm_drop(spseekb->spalsaconfig->pcmhandle);
        snd_pcm_prepare(spseekb->spalsaconfig->pcmhandle);

        if(spseekb->spfileinfo->seconds < 0)
        {
            sbret = E_CML_CANNOT_SEEKBACKWARD ;
        }
        else
        {
            spseekb->spfileinfo->seconds -= 10;
 int ls=lseek64(spseekb->spfileinfo->ifiledesc , spseekb->spwavheaderinfo->ibyterate*(spseekb->spfileinfo->seconds)
                    , SEEK_SET);
 if(ls<0)
printf("ERROR: in lseek64\n");
else
{
            pthread_create(&spseekb->spfileinfo->playbacktid, NULL, cml_playbackthread, spseekb);
            sbret = E_CML_SUCCESS;
        }
    }
}
    return sbret;
}

eRetType cml_audioplayer_seek_forward(scmlplayer *spseekf)
{
    eRetType sfret = E_CML_SUCCESS;
    if(STOP == spseekf->icurstate)
    {
        sfret = E_CML_CANNOT_SEEKFORWARD;
    }
    else
    {
        pthread_cancel(spseekf->spfileinfo->playbacktid);
        //pthread_cancel(timer_tid);
        snd_pcm_drop(spseekf->spalsaconfig->pcmhandle);
        snd_pcm_prepare(spseekf->spalsaconfig->pcmhandle);
        if(spseekf->spfileinfo->seconds > spseekf->spwavheaderinfo->iduration)
        {
            sfret = E_CML_CANNOT_SEEKFORWARD;
        }
        else
        {
            spseekf->spfileinfo->seconds += 10;
 int ls=lseek64(spseekf->spfileinfo->ifiledesc , spseekf->spwavheaderinfo->ibyterate*(spseekf->spfileinfo->seconds)
                    , SEEK_SET);
if(ls<0)
printf("ERROR: in lseek64\n");
else
{

            pthread_create(&spseekf->spfileinfo->playbacktid, NULL, cml_playbackthread, spseekf);
            //pthread_create(&timer_tid, NULL, timer_display, NULL);
            sfret = E_CML_SUCCESS;
        }
    }
}
        return sfret;
}


eRetType cml_audioplayer_seek_to(CML_UINT32 sec , scmlplayer *spseekto)
{
    eRetType stret = E_CML_SUCCESS; 
    if(STOP == spseekto->icurstate)
    {
        stret = E_CML_CANNOT_SEEKFORWARD;
    }
    else
    {
        pthread_cancel(spseekto->spfileinfo->playbacktid);
        //pthread_cancel(timer_tid);
        snd_pcm_drop(spseekto->spalsaconfig->pcmhandle);
        snd_pcm_prepare(spseekto->spalsaconfig->pcmhandle);
        spseekto->spfileinfo->seconds = sec;
        if(sec > spseekto->spwavheaderinfo->iduration)
        {
            stret = E_CML_INVALID_SEEKVALUE_EXCEEDED_DURATION;
        }
        else
        {
   int ls=lseek64(spseekto->spfileinfo->ifiledesc , spseekto->spwavheaderinfo->ibyterate*(spseekto->spfileinfo->seconds)
                    , SEEK_SET);
if(ls<0)
printf("ERROR: in lseek64\n");
else{
            pthread_create(&spseekto->spfileinfo->playbacktid, NULL, cml_playbackthread, spseekto);
        //pthread_create(&timer_tid, NULL, timer_display, NULL);
        stret = E_CML_SUCCESS;
        }
    }
}
    return stret;
}

eRetType cml_audioplayer_dinit(scmlplayer *spdinitplay)
{
    CML_UINT32 i;
eRetType eret=E_CML_SUCCESS; 
    pthread_join(spdinitplay->spfileinfo->playbacktid, NULL);
    for(i=0;i<spdinitplay->spfileinfo->icount;i++)
    {
        free(spdinitplay->samyplaylist[i].pfilepath);
    }
    snd_pcm_close(spdinitplay->spalsaconfig->pcmhandle);
    close(spdinitplay->spfileinfo->ifiledesc);
    free(spdinitplay->spalsaconfig->buffer);
    free(spdinitplay->spalsaconfig);
    free(spdinitplay->spwavheaderinfo);
    free(spdinitplay->spfileinfo);
    free(spdinitplay);
    kill(getpid(),SIGINT);
return eret;

}


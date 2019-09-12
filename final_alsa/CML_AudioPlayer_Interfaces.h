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
 *   CML_AudioPlayer interfaces header
 *
 ******************************************************************************/
                     /*  CML AudioPlayer API interfaces*/
#ifndef _CML_AUDIOPLAYER_INTERFACES_H_
#define _CML_AUDIOPLAYER_INTERFACES_H_
#include "CML_types.h"
#include"cml_error_codes.h"
#define STOP 101
#define PLAY 102
#define PAUSE 103
typedef struct CMLhandle
{
    scmlalsaconfig *spalsaconfig;
    scmlwavheaderinfo *spwavheaderinfo;
    scmlaudioinfo *spfileinfo;
    scmlmyplaylist samyplaylist[10000];
    CML_UINT32 icurstate;
    void (*fpsetstate)(CML_playerstate ecurstate , struct CMLhandle *spsetstate);
}scmlplayer;

eRetType cml_audioplayer_init(CML_UINT32 iNoOfargs , CML_CHAR *pacargslist[], scmlplayer *spinitplay);
 
/*
Description: This API initializes the PCM device,sets the parameters, and checks the validity of the inputs that users enters. If its valid inputs then it creates internal playlist by using array of structures [to store the .wav files].
 
Parameters:
argc – Number of arguments.
Argv  - Commands and arguments.
argv[0] = -S if Playlist is to be shuffled
argv[1]....argv[n]= Playlist(.txt files) /AudioFiles(.wav files)
it takes argc to check the number of arguments entered by the user, and argv as argument vector, the API  takes all the arguments passed by the user if it has shuffle as the first argument it will shuffle the internal playlist.

  Returns status
 This is returned on successful execution of API.
  CML_INIT_SUCCESS :
  CML_AUDIOFILE_RANGE_EXCEEDED : The number of .wav AudioFiles given as input exceeded the maximum acceptable range (i.e 10000 .wav files)
  CML_CANNOT_CREATE_PLAYLIST_EXCEEDED_AUDIO_RANGE : The number of .wav files within the .txt file (Playlist) exceeded the maximum acceptable range (i.e 10000 .wav files)
  CML_INVALID_FILE_FORMAT  : Playlist/ AudioFile path not provided
  CML_INVALID_FILE_EXTENSION : The extension of the file can be either .txt or .wav only.
  CML_NO_ARGUMENTS : No arguments provided.
  CML_INVALID_NUMBER_OF_ARGUMENTS: The number arguments passed is not within the permissible range
  CML_INVALID_ARGUMENTS_PASSED: The arguments passed are invalid.
  CML_FILE_NOT_FOUND : Path to the .wav file does not exist 
  CML_PLAYLIST_NOT_FOUND : Path to the .txt file does not exist */

 eRetType cml_audioplayer_play(scmlplayer *spplay);

/*
    Description: whenever user enters the specified key to play this API fetches the file handle of the first file in the internal play-list and starts the playback.
 If the user enters ‘W’ or ‘w’ this API will be invoked.
    Returns status
    CML_CANNOT_PLAY_FILE_CORRUPTED:when file is corrupted there is no playback.*/

 eRetType cml_audioplayer_pause_resume(scmlplayer *sppause);

/*
    Description: This API toggles the playback between pause and resume depending on the val being sent to the ALSA  API so that whenever user passed the specified character key to pause and resume this API toggles between the pause and resume state, If the user enters ‘P’ or ‘p’ this API  will be invoked.

    Returns status
    CML_CANNOT_PAUSE_AND_RESUME_IN_STOP_STATE : Cannot pause audio when Playback is stopped.*/

eRetType cml_audioplayer_volume_control(CML_LONG volume);

/*
    Description: This API increases or decreases the volume depending on the character  key pressed by the user.
 If the user enters ‘I’ it will  increase the volume and if its  ‘D’ it will decrease the volume. For one press of the key this  API increases the volume by 10% and likewise decreases the volume by 10%.

   Returns status
      CML_CANNOT_INCREASE_VOLUME_IN_STOP_STATE : Cannot increase the volume when Playback is stopped.
     CML_CANNOT_DECREASE_VOLUME_IN_STOP_STATE : Cannot decrease the volume when Playback is stopped.
    CML_MIN_VOLUME_ATTAINED : Cannot decrease the volume below the minimum permissible  level.
   CML_MAX_VOLUME_ATTAINED : Cannot decrease the volume beyond the maximum permissible  level.*/

 eRetType cml_audioplayer_play_next_file(scmlplayer *spnext); 

/*
    Description: This API jumps the play-back to the next track in the internal playlist.When user enters the ‘N’ or ‘n’  character key this API is called to play the next track.

  Returns status
  CML_NEXT_INVALID_FOR_SINGLE_AUDIO : Cannot play next audio when only one audio (.wav file) is available
  CML_NEXT_INVALID_FOR_LAST_AUDIO : Cannot play the next audio when the audio (.wav file) being played is the last audio file in the Playlist.*/

eRetType cml_audioplayer_play_prev_file(scmlplayer *spprev); 

/*
    Description: This API jumps the play-back to the previous track in the internal playlist.When user enters the ‘B’ or ‘b’  character key this API is called to play the previous track.

    Returns status
   CML_PREVIOUS_INVALID_FOR_SINGLE_AUDIO : Cannot play previous audio when only one audio (.wav file) is available
   CML_PREVIOUS_INVALID_FOR_FIRST_AUDIO :  Cannot play the previous audio when the audio (.wav file) being played is the last audio file in the Playlist.

7. eRetType cml_audioplayer_seek_forward( ) 
    Description: This API seeks the play-back forward by 10sec from the current position, when user enter the character key ‘F’ or ‘f’  .
    Returns status
    CML_CANNOT_SEEKFORWARD:  Reached the end of audio 
    CML_INVALID_SEEKVALUE_EXCEEDED_DURATION : The seek value provided exceeds the duration of the audio.*/

 eRetType cml_audioplayer_seek_back(scmlplayer *spseekb);

/*
    Description:  This API seeks the play-back backward by 10sec from the current position,when user enter the character key ‘G’ or ‘g’.

  Returns status
 CML_CANNOT_SEEKBACKWARD : Reached the beginning of audio
CML_INVALID_SEEKVALUE_EXCEEDED_DURATION : The seek value provided exceeds the duration of the audio.*/
 eRetType cml_audioplayer_seek_forward(scmlplayer *spseekf);

/*
    Description:  This API seeks the play-back forward by 10sec from the current position,when user enter the character key ‘G’ or ‘g’.

  Returns status
 CML_CANNOT_SEEKFORWARD : Reached the ending of audio
CML_INVALID_SEEKVALUE_EXCEEDED_DURATION : The seek value provided exceeds the duration of the audio.*/

 eRetType cml_audioplayer_seek_to(CML_UINT32 sec , scmlplayer *spseekto);

/*
    Description: This API seeks the play-back to the particular time-stamp given by the user.If the user enters ‘J’<timestamp> or  ‘j’<timestamp> this API  will be invoked.
Parameters:
this API takes the time-stamp given by the user as parameters to seek to the particular position.The format of time-stamp is HH:MM:SS.
 
   Returns status
       CML_INVALID_SEEKVALUE_EXCEEDED_DURATION : The seek value provided exceeds the duration of the audio. */


 eRetType cml_audioplayer_playback_stop(scmlplayer *spstop);

/*
    Description: This API stops the play-back,clears the buffer,closes the device and cancel the  playback_thread.If the user enters ‘S’ or ‘s’ this API  will be invoked.

 Returns status
      CML_STOP_PLAYBACK : Stops the audio Playback.
      CML_END_OF_PLAYLIST: Playback stopped due to end of Playlist.*/
 eRetType cml_audioplayer_dinit(scmlplayer *spdinitplay);


#endif      


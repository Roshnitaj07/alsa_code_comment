#include "cml_error_codes.h"
#include "CML_types.h"

const CML_CHAR * cml_perror(eRetType estate)
{
    const CML_CHAR *cmlerrormsg;
    switch(estate)
    {
        /* The number of .wav AudioFiles given as input exceeded the maximum acceptable range (i.e 10000 .wav files)*/
        case E_CML_AUDIOFILE_RANGE_EXCEEDED:
            cmlerrormsg="AUDIOFILE_RANGE_EXCEEDED";
            break;
            /* Cannot play the next audio when the audio (.wav file) being played is the last audio file in the Playlist.*/
        case E_CML_CANNOT_CREATE_PLAYLIST_EXCEEDED_AUDIO_RANGE:
            cmlerrormsg="CANNOT_CREATE_PLAYLIST_EXCEEDED_AUDIO_RANGE";
            break;
            /*Playlist/ AudioFile path not provided*/
       
            /*The extension of the file can be either .txt or .wav only.*/
        case E_CML_INVALID_FILE_EXTENSION:
            cmlerrormsg="INVALID_FILE_EXTENSION";
            break;
            /*No arguments provided.*/
        case E_CML_NO_ARGUMENTS:
            cmlerrormsg="NO_ARGUMENTS";
            break;
            /*The number arguments passed is not within the permissible range.*/
        case E_CML_INVALID_NUMBER_OF_ARGUMENTS:
            cmlerrormsg="INVALID_NUMBER_OF_ARGUMENTS";
                break;
            /*The arguments passed are invalid.*/
        case E_CML_INVALID_ARGUMENTS_PASSED:
            cmlerrormsg="INVALID_ARGUMENTS_PASSED";
            break;
            /* Path to the .wav file does not exist */
        case CML_FILE_NOT_FOUND:
            cmlerrormsg="FILE_NOT_FOUND";
            break;
            /*Path to the .txt file does not exist*/
        case E_CML_PLAYLIST_NOT_FOUND:
            cmlerrormsg="PLAYLIST_NOT_FOUND";
            break;
            /*Cannot play previous audio when only one audio (.wav file) is available*/
        case E_CML_PREVIOUS_INVALID_FOR_SINGLE_AUDIO:
            cmlerrormsg="PREVIOUS_INVALID_FOR_SINGLE_AUDIO";
            break;
            /*Cannot play next audio when only one audio (.wav file) is available*/
        case E_CML_NEXT_INVALID_FOR_SINGLE_AUDIO:
            cmlerrormsg="NEXT_INVALID_FOR_SINGLE_AUDIO";
            break;
            /* Cannot play the next audio when the audio (.wav file) being played is the last audio file in the Playlist.*/
        case E_CML_NEXT_INVALID_FOR_LAST_AUDIO:
            cmlerrormsg="NEXT_INVALID_FOR_LAST_AUDIO" ;
            break;
            /*Cannot play the previous audio when the audio (.wav file) being played is the last audio file in the Playlist.*/
        case E_CML_PREVIOUS_INVALID_FOR_FIRST_AUDIO:
            cmlerrormsg="PREVIOUS_INVALID_FOR_FIRST_AUDIO";
            break;
            /*The seek value provided exceeds the duration of the audio.*/
        case E_CML_INVALID_SEEKVALUE_EXCEEDED_DURATION: 
            cmlerrormsg="INVALID_SEEKVALUE_EXCEEDED_DURATION";
            break;
            /* Reached the beginning of audio*/
        case E_CML_CANNOT_SEEKBACKWARD:
            cmlerrormsg="CANNOT_SEEKBACKWARD";
            break;
            /* Reached the end of audio */
        case E_CML_CANNOT_SEEKFORWARD:
            cmlerrormsg="CANNOT_SEEKFORWARD";
            break;
            /*Cannot pause audio when Playback is stopped.*/
        case E_CML_CANNOT_PAUSE_IN_STOP_STATE: 
            cmlerrormsg="CANNOT_PAUSE_IN_STOP_STATE";
            break;
            /*Cannot increase the volume when Playback is stopped.*/
        case E_CML_CANNOT_INCREASE_VOLUME_IN_STOP_STATE: 
            cmlerrormsg="CANNOT_INCREASE_VOLUME_IN_STOP_STATE";
            break;
            /*Cannot decrease the volume when Playback is stopped*/
        case E_CML_CANNOT_DECREASE_VOLUME_IN_STOP_STATE:
            cmlerrormsg="CANNOT_DECREASE_VOLUME_IN_STOP_STATE";
            break;
            /*The key pressed is invalid.*/
        case E_CML_INVALID_KEY_PRESSED:
            cmlerrormsg="INVALID_KEY_PRESSED";
            break;
            /* Cannot decrease the volume below the minimum permissible level.*/
        case E_CML_MIN_VOLUME_ATTAINED: 
            cmlerrormsg="MIN_VOLUME_ATTAINED";
            break;
            /* Cannot decrease the volume beyond the maximum permissible  level.*/
        case E_CML_MAX_VOLUME_ATTAINED: 
            cmlerrormsg="MAX_VOLUME_ATTAINED";
            break;
            /* Playback stopped due to end of Playlist*/
        case E_CML_END_OF_PLAYLIST:
            cmlerrormsg="END_OF_PLAYLIST";
            break;
            /* Cannot play the audio(.wav file)  since it is corrupted.*/
        case E_CML_CORRUPTED_AUDIO_FILE:
            cmlerrormsg="CORRUPTED_AUDIO_FILE";
            break;
            /*Cannot play the audio(.wav file)  since the playlist (.txt file)  is corrupted.*/
        case E_CML_CORRUPTED_PLAYLIST: 
            cmlerrormsg="CORRUPTED_PLAYLIST";
            break;
            /* Play  next AudioFile for single input .wav file*/
        case E_CML_UNABLE_TO_PLAY_NEXT_FILE_NOT_PRESENT:
            cmlerrormsg="UNABLE_TO_PLAY_NEXT_FILE_NOT_PRESENT";
            break;
            /* Play previous AudioFile for single input .wav file.*/
        case E_CML_UNABLE_TO_PLAY_PREV_FILE_NOT_PRESENT:
            cmlerrormsg="UNABLE_TO_PLAY_PREV_FILE_NOT_PRESENT";
            break;
            
          
            
            /*if invalid keyInput is given other than specified keyInputs.
              if any input is given other mentioned arguments.*/
       
            /*if seek previous time is less than the initial duration of audio*/
       
            /*playlist containg .wav files without any paths*/
        case E_CML_FILE_PATH_NOT_SPECIFIED:
            cmlerrormsg="FILE_PATH_NOT_SPECIFIED:";
            break;
            /*Playlist with  .wav files but 1 or 2 files without .wav extension
              Invalid type of input file other than .wav extension*/
      
            /* Multiple .wav files as arguments seperated by more than single space.
               Multiple .txt files as arguments saperated by more than single space.*/
        ;
            /* Arguments passed are not correct order for shuffle i.e < path/file.wav –s or path/file.txt-- s>*/
        case E_CML_WRONG_ORDER_OF_ARGUMENTS:
            cmlerrormsg="WRONG_ORDER_OF_ARGUMENTS";
            break;
            /* ALSA could not open device */
        case  E_CML_ERROR_WHILE_PAUSE:
             cmlerrormsg = "ERROR_WHILE_PAUSE";
            break;

        case  E_CML_ERROR_WHILE_RESUME:
              cmlerrormsg = "ERROR_WHILE_RESUME";
            break;


        case E_CML_NEXT_INVALID_IN_STOP_STATE:
              cmlerrormsg = "NEXT_INVALID_IN_STOP_STATE";
            break;
 /* Last track */
        case  E_CML_ERROR_LAST_TRACK:
            cmlerrormsg="ERROR_LAST_TRACK";
            break;
            /* First track */
        case E_CML_ERROR_FIRST_TRACK:
            cmlerrormsg="ERROR_FIRST_TRACK";
            break;
            /* Cannot stop */
        case E_CML_ERROR_STOP: 
            cmlerrormsg="ERROR_STOP";
            break;
            /* ALSA Cannot resume */
        
            /* Seek time is more than duration */
       
       
            /*Error while creating playlist*/
        case E_CML_ERROR_CREATING_PLAYLIST:
            cmlerrormsg="ERROR_CREATING_PLAYLIST";
            break;
            /*Error during Shuffle operation*/
       
            /*Failure in creating thread*/
        case E_CML_THREAD_CREATION_FAILED:
            cmlerrormsg="THREAD_CREATION_FAILED";
            break;
            /*Error in allocating memory*/
        case E_CML_MEMORY_NOT_ALLOCATED:
            cmlerrormsg="MEMORY_NOT_ALLOCATED";
            break;
           
     
       
        
    }
    return cmlerrormsg;
}


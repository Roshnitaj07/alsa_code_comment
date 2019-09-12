#ifndef _CML_TYPES_H_
#define _CML_TYPES_H_

#include<stdio.h>
#include <stddef.h>
#include<pthread.h>
#include<alsa/asoundlib.h>
#define CML_TRUE 1
#define CML_FALSE 0


typedef char CML_CHAR;
typedef unsigned char CML_UCHAR;
typedef unsigned char CML_BYTE;
typedef signed char CML_INT8;
typedef unsigned char CML_UINT8;
typedef signed short CML_INT16;
typedef unsigned short CML_UINT16;
typedef unsigned int CML_UINT32;
typedef int CML_INT32;
typedef signed long  CML_INT64;
typedef unsigned long CML_UINT64;
/*typedef signed long long CML_INT64;
  typedef unsigned long long CML_UINT64;*/
typedef signed long CML_LONG;
typedef unsigned char CML_BOOL;
typedef float CML_FLOAT;
typedef double CML_DOUBLE;
typedef void CML_VOID;
typedef size_t CML_SIZE_T;
typedef FILE CML_FILE;


typedef struct CML_alsaconifg
{
    snd_pcm_t *pcmhandle;
    snd_pcm_hw_params_t *params;
    snd_pcm_uframes_t frames;
    CML_CHAR *buffer;
}scmlalsaconfig;

typedef struct CML_headerinfo
{
    CML_UINT32 isamplerate;
    CML_CHAR   uwchannel;
    CML_UINT32 isize;
    CML_UINT32 iduration;
    CML_UINT32 ibyterate;
    CML_UINT32 ibitspersample;
}scmlwavheaderinfo;

typedef struct CML_myplaylist
{

    CML_CHAR *pfilepath;

}scmlmyplaylist;

typedef struct CML_fileinfo
{
    CML_UINT32 icurfileindex;
    CML_UINT32 icount;
    CML_INT32 seconds;
    CML_UINT32 ifiledesc;
    CML_FILE *fpFilepointer;
    pthread_t playbacktid;
    pthread_t timertid;
    CML_UINT32 tmp;
}scmlaudioinfo;

typedef enum 
{

    STOP_STATE = 101,
    PLAY_STATE = 102,
    PAUSED_STATE = 103

}CML_playerstate;


#endif /*CML_types_h*/




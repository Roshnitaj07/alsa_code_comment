#define ALSA_PCM_NEW_HW_PARAMS_API

#include <alsa/asoundlib.h> 
#include <stdio.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include"CML_types.h"
#define TRUE 1 
#define FALSE 0
/*typedef enum 
{
 STOP_STATE=101;
 PLAY_STATE=102;
 PAUSED_STATE=103;
}CML_playerstate;

struct CML_checkstate
{
 CML_UINT32 icurstate;
}ssetcurstate;*/

void (*fpsetstate)(CML_playerstate ecurstate);

void psetstate(CML_playerstate ecurstate)
{	
	switch(ecurstate)
	{
	 case STOP_STATE:ssetcurstate.icurstate=101;break;
	 case PLAY_STATE:ssetcurstate.icurstate=102;break;
	 case PAUSED_STATE:ssetcurstate.icurstate=103;break;
	}
}
// WAVE file header format
struct HEADER 
{
	// RIFF string
	unsigned char riff[4];   
  
	// overall size of file in bytes
	unsigned int overall_size; 

	// WAVE string
	unsigned char wave[4];     

	// fmt string with trailing null char
	unsigned char fmt_chunk_marker[4]; 

	// length of the format data
	unsigned int length_of_fmt;

	// format type. 1-PCM, 3- IEEE float, 6 - 8bit A law, 7 - 8bit mu law 
	unsigned int format_type;

	// no.of channels  
	unsigned int channels;	

	// sampling rate (blocks per second)
	unsigned int sample_rate;

	// SampleRate * NumChannels * BitsPerSample/8 
	unsigned int byterate;	

	// NumChannels * BitsPerSample/8
	unsigned int block_align; 

	// bits per sample, 8- 8bits, 16- 16 bits etc
	unsigned int bits_per_sample; 

	// DATA string or FLLR string
	unsigned char data_chunk_header [4];

	// NumSamples * NumChannels * BitsPerSample/8 - size of the next chunk that will be read
	unsigned int data_size;
};

unsigned char buffer4[4];
unsigned char buffer2[2];

FILE *ptr;
char *filename;
struct HEADER header;

int fr,tr;
int size;
snd_pcm_t *pcm_handle;
snd_pcm_hw_params_t *params;
unsigned int sample_rate;
int dir;
snd_pcm_uframes_t frames;
char *buffer,*ter_buf;
int paused = 0;
int mfp;
int channels;
int seconds=0;
pthread_t playback_tid,timer_tid;

int sec;

void *playback_thread(void *args);
char getch_from_term();
void extract_wave_header();
void *timer_display(void *args);
void seek_forward_10();
void seek_backward_10();
void seek_to(int s);

/*
int kbhit(void)
{
	struct timeval tv;
	fd_set read_fd;

	tv.tv_sec=0;
	tv.tv_usec=0;
	FD_ZERO(&read_fd);
	FD_SET(0,&read_fd);

	if(select(1, &read_fd, NULL, NULL, &tv) == -1)
		return 0;

	if(FD_ISSET(0,&read_fd))
		return 1;

	return 0;
}
*/


int main(int args, char **argv) {
  //ssetcurstate.icurstate = 102;
  fpsetstate=psetstate;
  char ch;

  if(args <= 1)
  {
	printf("INVALID ARGUEMENTS\n");
	exit(0);
  }
	 
 // mfp = open("music_1.wav",O_RDONLY);
 // extract_wave_header("music_1.wav");

  mfp = open(argv[1],O_RDONLY);
  extract_wave_header(argv[1]);


  /* Open PCM device for playback. */
  fr = snd_pcm_open(&pcm_handle, "default",
                    SND_PCM_STREAM_PLAYBACK, 0);
  if (fr < 0) {
    fprintf(stderr,
            "unable to open pcm device: %s\n",
            snd_strerror(fr));
    exit(1);
  }
/* Allocate a hardware parameters object. */
  snd_pcm_hw_params_alloca(&params);

  /* Fill it in with default values. */
  snd_pcm_hw_params_any(pcm_handle, params);

  /* Set the desired hardware parameters. */

  /* Interleaved mode */
  snd_pcm_hw_params_set_access(pcm_handle, params,
                      SND_PCM_ACCESS_RW_INTERLEAVED);

  /* Signed 16-bit little-endian format */
  snd_pcm_hw_params_set_format(pcm_handle, params,
                              SND_PCM_FORMAT_S16_LE);

  snd_pcm_hw_params_set_channels(pcm_handle, params, channels);

  //sample_rate = 8000;
  snd_pcm_hw_params_set_rate_near(pcm_handle, params,
                                  &sample_rate, &dir);

  /* Set period size to 32 frames. */
  //frames = 32;
  //snd_pcm_hw_params_set_period_size_near(pcm_handle, params, &frames, &dir);

  /* Write the parameters to the driver */
  fr = snd_pcm_hw_params(pcm_handle, params);
  if (fr < 0) {
    fprintf(stderr,
            "unable to set hw parameters: %s\n",
            snd_strerror(fr));
    exit(1);
  }

  /* Use a buffer large enough to hold one period */
  snd_pcm_hw_params_get_period_size(params, &frames,
                                    &dir);
  size = frames * 4;//2 * sizeof(float); /* 2 bytes/sample, 2 channels */
  buffer = (char *) malloc(size);
 // ter_buf = (char *)malloc(10);

  
  fr = pthread_create(&playback_tid, NULL, playback_thread, NULL);
	pthread_create(&timer_tid, NULL, timer_display, NULL);
  if (fr > 0)
  {
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  }
  do {

	ch = getch_from_term();

	switch(ch)
	    {
		case ' ':
			if(ssetcurstate.icurstate == 102)
			{
				if(snd_pcm_pause(pcm_handle, 1) < 0)
				{
					printf("ERROR while pause");
					return 0;
			    	}
				printf("song paused\n");
				fpsetstate(PAUSED_STATE);
				paused = 1;
			}
			else if(ssetcurstate.icurstate == 103)
			{
				if(snd_pcm_pause(pcm_handle, 0) < 0)
			{
				printf("ERROR while resume");
				return 0;
		    	}
			printf("song resumed\n");
			fpsetstate(PLAY_STATE);
			paused = 0;
			}
			break;

		case 'f':  seek_forward_10();
			     break;
		case 'g':
			seek_backward_10();			
			break;
		case 't':
		case 'j':
			scanf("%d",&sec);
			seek_to(sec);
			break;
			
            }
   
  }while(ch != 'q' && ch != 'Q');

  printf("END\n");
  pthread_cancel(playback_tid);
  pthread_cancel(timer_tid);
  snd_pcm_drop(pcm_handle);
  snd_pcm_close(pcm_handle);
  free(buffer);

  return 0;
}

void seek_forward_10()
{
	pthread_cancel(playback_tid);
	pthread_cancel(timer_tid);
	snd_pcm_drop(pcm_handle);
	snd_pcm_prepare(pcm_handle);
	seconds += 10;
	lseek64(mfp, header.byterate*(seconds) , SEEK_SET);
	pthread_create(&playback_tid, NULL, playback_thread, NULL);
	pthread_create(&timer_tid, NULL, timer_display, NULL);
}

void seek_backward_10()
{
	pthread_cancel(playback_tid);
	pthread_cancel(timer_tid);
	snd_pcm_drop(pcm_handle);
	snd_pcm_prepare(pcm_handle);
	seconds -= 10;
	lseek64(mfp, header.byterate*(seconds) , SEEK_SET);
	pthread_create(&playback_tid, NULL, playback_thread, NULL);
	pthread_create(&timer_tid, NULL, timer_display, NULL);
}

void seek_to(int s)
{
	pthread_cancel(playback_tid);
	pthread_cancel(timer_tid);
	snd_pcm_drop(pcm_handle);
	snd_pcm_prepare(pcm_handle);
	seconds = s;
	lseek64(mfp, header.byterate*(seconds) , SEEK_SET);
	//snd_pcm_prepare(pcm_handle);
	pthread_create(&playback_tid, NULL, playback_thread, NULL);
	pthread_create(&timer_tid, NULL, timer_display, NULL);
}

void extract_wave_header(char *fname)
{
	filename = (char*) malloc(sizeof(char) * 1024);
	 if (filename == NULL) {
	   printf("Error in malloc\n");
	   exit(1);
	 }
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
	   
		strcpy(filename, cwd);
		strcat(filename, "/");
		strcat(filename, fname);
		printf("%s\n", filename);
	}

	// open file
	 printf("Opening  file..\n");
	 ptr = fopen(filename, "rb");
	 if (ptr == NULL) {
		printf("Error opening file\n");
		exit(1);
	 }
	 
	 int read = 0;
	 
	 // read header parts
	 read = fread(header.riff, sizeof(header.riff), 1, ptr);
	 printf("(1-4): %s \n", header.riff); 

	 read = fread(buffer4, sizeof(buffer4), 1, ptr);
	 printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
	 
	 // convert little endian to big endian 4 byte int
	 header.overall_size  = buffer4[0] | 
				(buffer4[1]<<8) | 
				(buffer4[2]<<16) | 
				(buffer4[3]<<24);

	 printf("(5-8) Overall size: bytes:%u, Kb:%u \n", header.overall_size, header.overall_size/1024);

	 read = fread(header.wave, sizeof(header.wave), 1, ptr);
	 printf("(9-12) Wave marker: %s\n", header.wave);

	 read = fread(header.fmt_chunk_marker, sizeof(header.fmt_chunk_marker), 1, ptr);
	 printf("(13-16) Fmt marker: %s\n", header.fmt_chunk_marker);

	 read = fread(buffer4, sizeof(buffer4), 1, ptr);
	 printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

	 // convert little endian to big endian 4 byte integer
	 header.length_of_fmt = buffer4[0] |
				(buffer4[1] << 8) |
				(buffer4[2] << 16) |
				(buffer4[3] << 24);
	 printf("(17-20) Length of Fmt header: %u \n", header.length_of_fmt);

	 read = fread(buffer2, sizeof(buffer2), 1, ptr); printf("%u %u \n", buffer2[0], buffer2[1]);
	 
	 header.format_type = buffer2[0] | (buffer2[1] << 8);
	 char format_name[10] = "";
	 if (header.format_type == 1)
	   strcpy(format_name,"PCM"); 
	 else if (header.format_type == 6)
	  strcpy(format_name, "A-law");
	 else if (header.format_type == 7)
	  strcpy(format_name, "Mu-law");

	 printf("(21-22) Format type: %u %s \n", header.format_type, format_name);

	 read = fread(buffer2, sizeof(buffer2), 1, ptr);
	 printf("%u %u \n", buffer2[0], buffer2[1]);

	 header.channels = buffer2[0] | (buffer2[1] << 8);
	 printf("(23-24) Channels: %u \n", header.channels);
	channels = (int) header.channels;

	 read = fread(buffer4, sizeof(buffer4), 1, ptr);
	 printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

	 header.sample_rate = buffer4[0] |
				(buffer4[1] << 8) |
				(buffer4[2] << 16) |
				(buffer4[3] << 24);

	 printf("(25-28) Sample rate: %u\n", header.sample_rate);
	 sample_rate = header.sample_rate;

	 read = fread(buffer4, sizeof(buffer4), 1, ptr);
	 printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

	 header.byterate  = buffer4[0] | 
				(buffer4[1] << 8) | 
				(buffer4[2] << 16) | 
				(buffer4[3] << 24);

	 printf("(29-32) Byte Rate: %u , Bit Rate:%u\n", header.byterate, header.byterate*8);

	 read = fread(buffer2, sizeof(buffer2), 1, ptr);
	 printf("%u %u \n", buffer2[0], buffer2[1]);

	 header.block_align = buffer2[0] | 
				(buffer2[1] << 8);
	 printf("(33-34) Block Alignment: %u \n", header.block_align);

	 read = fread(buffer2, sizeof(buffer2), 1, ptr);
	 printf("%u %u \n", buffer2[0], buffer2[1]);

	 header.bits_per_sample = buffer2[0] |
				  (buffer2[1] << 8);

	 printf("(35-36) Bits per sample: %u \n", header.bits_per_sample);

	 read = fread(header.data_chunk_header, sizeof(header.data_chunk_header), 1, ptr);
	 printf("(37-40) Data Marker: %s \n", header.data_chunk_header);

	 read = fread(buffer4, sizeof(buffer4), 1, ptr);
	 printf("%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

	 header.data_size = buffer4[0] |
					(buffer4[1] << 8) |
					(buffer4[2] << 16) | 
					(buffer4[3] << 24 );
	 printf("(41-44) Size of data chunk: %u \n", header.data_size);


	 // calculate no.of samples
	 long num_samples = (8 * header.data_size) / (header.channels * header.bits_per_sample);
	 printf("Number of samples:%lu \n", num_samples);

	 long size_of_each_sample = (header.channels * header.bits_per_sample) / 8;
	 printf("Size of each sample:%ld bytes\n", size_of_each_sample);

	 // calculate duration of file
	 float duration_in_seconds = (float) header.overall_size / header.byterate;
	 printf("Approx.Duration in seconds=%f\n", duration_in_seconds);

	fclose(ptr);
	free(filename);
}

char getch_from_term() {
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

void *timer_display(void *args)
{
	int duration =(int) header.overall_size / header.byterate;
	while(paused == 0 && seconds <= duration)
	{
		seconds++;
		printf("\r");
		fprintf(stderr,"\rCurrent Playback Time :%d ",seconds);
		printf("\r");
		sleep(1);
	}
}

void *playback_thread(void *args)
{   fpsetstate(PLAY_STATE);
    while(paused == 0)
    {
	    memset(buffer, 0, size);
	   // fr = fgets(buffer,size, mfp);
	    int fr = read(mfp, buffer, size);
	    //tr = read(0, ter_buf, 10);
	   // printf("%d",tr);
	    if (fr == 0) {
	      //fprintf(stderr, "end of file on input\n");
	      break;
	    } else if (fr != size) {
	      //fprintf(stderr,"short read: read %d bytes\n", fr);
	    }
	    fr = snd_pcm_writei(pcm_handle, buffer, frames);
	    if (fr== -EPIPE) {
	      /* EPIPE means underrun */
	     // fprintf(stderr, "underrun occurred\n");
	      snd_pcm_prepare(pcm_handle);
	    } else if (fr < 0) {
	     // fprintf(stderr, "error from writei: %s\n",snd_strerror(fr));
	    }  else if (fr != (int)frames) {
	      //fprintf(stderr,"short write, write %d frames\n", fr);
	    }
     }
     
}



  

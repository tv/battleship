#ifndef SOUND_H_
#define SOUND_H_

#include <cfg/os.h>
#include <cfg/clock.h>
#include <dev/board.h>
#include <dev/st7036.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <io.h>
#include <math.h>

#include <sys/thread.h>
#include <sys/confos.h>
#include <sys/timer.h>
#include <sys/event.h>

#include <dev/vscodec.h>

#include "utils/fix_fft.h"
#include "utils/config.h"
#include "utils/vsutils.h"
#include <limits.h>

#include "Common.h"
#include "UI.h"


#define SIGNIFICANTS 10    // Significant count on cepstrums
#define SAMPLE_LIB_SIZE 7  // Size of the library in samples
#define CEPS_COUNT 48      // Size of the word in cepstrums
#define MIN_CEPS_COUNT 20  // min size of word
#define WIN_SIZE  128      // Window size in recorded samples
#define WIN_SIZE_E 7       // same in twos exponent

#define DTW_TRESHOLD 340000000 // DTW distance treshold

#define RECORD_BUFFER_SIZE 8192 // Rinkibufferin koko in samples
#define ABSOLUTE_COUNT 512      // Liukuvan keskiarvon ikkunankoko sekä taaksepäinnauhoituksen ikkunankoko
#define BACKTRACE 0
#define RECORD_START 600        // Record start amplitude treshold
#define RECORD_STOP 40          // Record stop amplitude treshold

typedef struct
{
	short 	samplecount;
	short *	buffer;
	int ** 	dtwTable;
	UI *	ui;
	int 	ceps_count;
	bool 	recording;
} SoundStruct;

void Sound_init(UI * ui, bool recording);

THREAD(SoundRecord, arg);
THREAD(SoundFFT, arg);

void SR_generateMFC( short * real, short * im, short * mel_buffer, short samples_exp, int * cepstrum );
void SR_PreFilter( short * real, short samples );
void SR_abs( short * real, short * im, short samples_exp );
void SR_FFT( short * real, short * imag, short samples_exponent );
void SR_melscale(short* input, short * output, short samples_exp);
void SR_DCT( short* fftbuffer, int * dctBuffer, short samples_exponent);
int SR_DTWDistance(SoundStruct * ss, int ** recorded, int index, int n);
int SR_CepstrumDistance( int * recorded, int index, int index2);


#endif /* SOUND_H_ */

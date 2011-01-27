#include <math.h>
#include "SpeechRecognition.h"

int main (int argc, char const *argv[])
{
	
	int n;
	
	double hz = 5608;
	int samplerate = 44100;
	int length = 10;					// in ms
	
	
				// ms -> µs
	
	int samplecount = length*samplerate/1000;
	
	length = length*1000;	
	// printf( "%d, %d\n", length, samplecount);
	
	double twoPIhz = 4*atan(1.0)*2*hz / samplerate;

	// exit(1);
	double sini[samplecount];
	
	for( n = 0; n < samplecount; n++) {
		sini[ n ] = sin( twoPIhz * n );
		// printf("%f, %d\n", sini[n], n);
	}
	
	SR_generateMFC(sini, samplecount);
	
	return 0;
}
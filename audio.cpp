/*
 *  audio.cpp
 *  fromScratch
 *
 *  Created by Ivy Feraco on 3/1/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "audio.h"

static Audio *thisAudio;

static int myCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* outTime, 
					  PaStreamCallbackFlags flags, void *idata);
Audio::Audio() {
	//for use in callback
	thisAudio = this;
	reProcess = false;
}


void Audio::initSND() {
	//TODO:reference these files better. why isnt resources finding them
	sndFile = "theMix.wav";
	binFile = "theMixxxxx";
		
	myFile = sf_open(sndFile, SFM_READ, &sfInfo);
	//printf("my file! %@", myFile);
	if (sf_error(myFile) == 0) {
		frames = (int)sfInfo.frames;
		channels = sfInfo.channels;
		sampleRate = sfInfo.samplerate;
		printf("samplerate = %d\nframes = %d\n", sfInfo.samplerate, frames);
		printf("channels = %d\nformat = %08x\n", sfInfo.channels, sfInfo.format);
	}
	else {
		error = sf_strerror(myFile);
		printf("ERROR! %s", error);
	}
	
	//turn on normalization (altho i think its on by default)
	sf_command (myFile, SFC_SET_NORM_FLOAT, NULL, SF_TRUE) ;
	
	soundDataToPlay = (float *)malloc(sizeof(float) * frames * channels);
	soundDataForBeats = (float *)malloc(sizeof(float) * frames * channels);

	sf_readf_float(myFile, soundDataToPlay, frames * channels);
	
	totalWindows = windowLimiter();
	
}

int Audio::getTotalWindows() {
	return totalWindows;
}

void Audio::initPA(void) {
	/* Initialize library before making any other calls. */
	err = Pa_Initialize();
	if( err != paNoError ) getPAError(err);
	
	printf("version %d\n", Pa_GetVersion());
	
	start = 0;
	/* Open an audio I/O stream. */
	err = Pa_OpenDefaultStream(
							   &stream,
							   0,					/* no input channels */
							   2,					/* stereo output */
							   paFloat32,			/* 32 bit floating point output */
							   sfInfo.samplerate,
							   FRAMES_PER_BUFFER,	/* frames per buffer */
							   myCallback,
							   soundDataToPlay);

	if (err != paNoError) getPAError(err);

	err = Pa_StartStream(stream);
	if (err != paNoError) getPAError(err);
	
	/* Sleep for several seconds. */
	//Pa_Sleep(NUM_SECONDS*1000);
}

int Audio::windowLimiter() {
	return frames/WINDOW_SIZE;
}

long double Audio::getPlaybackTime() {
	long double playbackTime = (long double)myTime->outputBufferDacTime - start;
	return playbackTime;
	
}

static int myCallback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* outTime, 
			   PaStreamCallbackFlags flags, void *userData )
{
	/* Cast data passed through stream to our structure type. */
	float * dataToPlay = (float *)userData;
	
	//if the sound is beyond the end
	if (thisAudio->position >= thisAudio->frames * 2) thisAudio->position = 0; // loop it
	
	thisAudio->myTime = outTime;
	
	//set up the timer
	if (thisAudio->position == 0) { 
		thisAudio->start = (long double)Pa_GetStreamTime(thisAudio->stream);
	}

    float *out = (float*)outputBuffer;
    
	unsigned int i;
	//feed PA 256 frames which is 512 indeces (frames * channels)
	for (i = thisAudio->position; i < thisAudio->position + (framesPerBuffer * thisAudio->channels) ; i += thisAudio->channels)
    {
		// Stereo channels are interleaved. 
		*out++ = dataToPlay[i];            // left 
        *out++ = dataToPlay[i + 1];        //right 
    }
	
	//increment position
	//PA is expecting 256 Stereo frames which is 512 indices of libsnd data
	thisAudio->position += framesPerBuffer * thisAudio->channels;
	
    return 0;
}

void Audio::stopPA(void) {
	Pa_Terminate();
	printf("Test finished.\n");

	err = Pa_StopStream( stream );
	if( err != paNoError ) getPAError(err);

	err = Pa_CloseStream( stream );
	if( err != paNoError ) getPAError(err);
}

void Audio::getPAError(int err) {
	Pa_Terminate();
	fprintf( stderr, "An error occured while using the portaudio stream\n" );
	fprintf( stderr, "Error number: %d\n", err );
	fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
}

void Audio::stopSND() {
	sf_close(myFile);
}


////////////////////////////////////////////////////*window*setup*///////////////////////////////////////////////////////

/* FFT: for any frequency we are looking at we 'compare' (or 'resonate') our measured signal with 
both a cosine and a sine wave of the same frequency. It can be shown that the + and - sign together 
with the sine and cosine phase can represent any sinusoid at the given frequency */

Window * Audio::getWindows() {
	
	return windows;
}

Window * Audio::initWindowData() {
	cout << sndFile << "\n";
	cout << binFile << "\n";
	
	createEmptyWindows(frames);
	windowFDataToRead = (float *)malloc(sizeof(float) * (WINDOW_SIZE + SUBBANDS) * totalWindows);
	windowIDataToRead = (int *)malloc(sizeof(int) * (SUBBANDS + 1) * totalWindows);
	
	FILE * windowFile = fopen(binFile, "rb");
		
	//if not preProcessedFile 
	if (windowFile == NULL || ferror(windowFile) != 0 || reProcess) {
			
			soundDataToProcessL = (float *)malloc(sizeof(float) * frames * 2);
			soundDataToProcessR = (float *)malloc(sizeof(float) * frames * 2);
			//separate for each channel, with frames * 2 for real and imaginary numbers to feed the FFT
			
			printf("Creating soundDataToProcessL and R for FFT\n");
			
			for (int k = 0; k < frames * 2; k += 2)	{
				//frames * 2 for real and imaginary in
				soundDataToProcessL[k] = soundDataToPlay[k]; 
				soundDataToProcessL[k + 1] = 0.0; //create imaginary in
				
				//because soundDataToPlay is interleaved
				soundDataToProcessR[k] = soundDataToPlay[k + 1];	
				soundDataToProcessR[k + 1] = 0.0;
			}
			
			int windowNumber = 0;
			printf("About to call fft\n");
			
			for (int m = 0; m < frames * 2; m += (WINDOW_SIZE * 2)) {
				//frames * 2 for imaginary in
			
				if (windowNumber >= totalWindows) continue;	
				//so we don't overrun the windows array, but why?
				
				smbFft(soundDataToProcessL + m, WINDOW_SIZE, -1, windowNumber, 0);
				smbFft(soundDataToProcessR + m, WINDOW_SIZE, -1, windowNumber, 1);
				
				// void smbFft(float *fftBuffer, long fftFrameSize, long sign, int windowNumber, int whichChannel); 
				windowNumber++;
			}
			
			fftPost(0, sampleRate);	//0 for L
			fftPost(1, sampleRate);	//1 for R
			printf("Finishing magnitude data\n");
			
			
			printf("Creating soundDataForBeats\n");
			soundDataForBeats = (float *)malloc(sizeof(float) * frames * channels);
			
			for (int k = 0; k < frames * channels; k += 2)	{
				soundDataForBeats[k] = soundDataToPlay[k]; 
				//because soundDataToPlay is interleaved
				soundDataForBeats[k + 1] = soundDataToPlay[k + 1];
				//printf("SDFB = %f, %f\n", soundDataForBeats[k], soundDataForBeats[k + 1]);
			}
			
			computeInstantEnergy(soundDataForBeats);
			findBeats();
			
			computeInstantEnergySub();
			findBeatsSub();
			
			windowFile = fopen(binFile, "wb");
			//printf("windowFile object %@", windowFile);
			for (int i = 0; i < totalWindows; i++) {
				fwrite(windows[i].magnitudeL, sizeof(float), WINDOW_SIZE/2, windowFile);
				fwrite(windows[i].magnitudeR, sizeof(float), WINDOW_SIZE/2, windowFile);
				fwrite(windows[i].subEnergy, sizeof(float), SUBBANDS, windowFile);
			}
			
			for (int i = 0; i < totalWindows; i++) {
				//printf("WRITING...\n");
				
				fwrite(windows[i].subBeats, sizeof(int), SUBBANDS, windowFile);
				int beat[1];
				beat[0] = windows[i].beat;
				fwrite(beat, sizeof(int), 1, windowFile);
			}
	}
	
	else {
		printf("using file\n");
		windowFile = fopen(binFile, "rb");
		fread(windowFDataToRead, sizeof(float), totalWindows * (WINDOW_SIZE + SUBBANDS), windowFile);
		fread(windowIDataToRead, sizeof(int), totalWindows * (SUBBANDS + 1), windowFile);
		
		//WINDOW_SIZE = magL + magR = WINDOW_SIZE/2 + WINDOW_SIZE/2
		printf("read successful\n");
		populateWindows(windowFDataToRead, windowIDataToRead);
				
	}
	
	fclose(windowFile);
	printf("CLOSING...\n");
	return windows;
}


void Audio::createEmptyWindows(int iframes) {
	//an array of windows that make up the length of the sound (frames/WINDOW_SIZE)
	frames = iframes;
	totalWindows = (int)ceil(frames/ WINDOW_SIZE);
	
	printf("totalWindows = %d\n", totalWindows);
	
	//array of Window struct, each of the windows holds arrays of frequency, 
	//magnitude for each channel, and phase of each Window including real and 
	//imaginary data (of size WINDOW_SIZE) for the fft
	
	windows = (Window *)malloc(sizeof(Window) * totalWindows);

	for ( int j = 0; j < totalWindows; j++) {
		//each window holds an array of frequencies, mags, and phases
		windows[j].reImDataL  = (float *)malloc(sizeof(float) * WINDOW_SIZE);
		windows[j].reImDataR  = (float *)malloc(sizeof(float) * WINDOW_SIZE);
		
		windows[j].frequency  = (float *)malloc(sizeof(float) * WINDOW_SIZE/2); 
		
		windows[j].magnitudeL = (float *)malloc(sizeof(float) * WINDOW_SIZE/2);
		windows[j].magnitudeR = (float *)malloc(sizeof(float) * WINDOW_SIZE/2);
		
		windows[j].rawMagL	  = (float *)malloc(sizeof(float) * WINDOW_SIZE/2);
		windows[j].rawMagR    = (float *)malloc(sizeof(float) * WINDOW_SIZE/2);
		
		windows[j].subEnergy  = (float *)malloc(sizeof(float) * SUBBANDS); 
		windows[j].subBeats	  =	  (int *)malloc(sizeof(int)	  * SUBBANDS); 
		
		int beat			  =   0;
		float instantEnergy	  =   0.;
		//windows[j].phase    = (float *)malloc(sizeof(float) * WINDOW_SIZE/2);
		
		//why WINDOW_SIZE/2 ?
		//Of these coefficients only the first N/2 are useful 
		//(the others are just a mirror image as this is a real valued signal).
		//http://en.wikipedia.org/wiki/Short-time_Fourier_transform
		
	}
	
}

void Audio::populateWindows(float * iwindowFData, int * iwindowIData) {
	float * windowFData;
	  int * windowIData;
	
	for ( int j = 0; j < totalWindows; j++) {
		windowFData = (iwindowFData + ((WINDOW_SIZE + SUBBANDS) * j));
		windowIData = (iwindowIData + ((SUBBANDS + 1) * j));
		
		for (int k = 0; k < WINDOW_SIZE/2; k ++) windows[j].magnitudeL[k] = windowFData[k];
		for (int m = WINDOW_SIZE/2; m < WINDOW_SIZE; m ++) windows[j].magnitudeR[m - WINDOW_SIZE/2] = windowFData[m];
		
		windowFData += WINDOW_SIZE; //just to move pointer again for subEnergy
		for ( int n = 0; n < SUBBANDS; n ++) windows[j].subEnergy[n] = windowFData[n];
		
		for ( int k = 0; k < SUBBANDS; k++) {  
			windows[j].subBeats[k] = windowIData[k]; 
			//printf("%d\n", windows[j].subBeats[n]); 
		}
		windows[j].beat =  *(windowIData + SUBBANDS);
	}
	
}

//////////////////////////////////////////////////////*FFT*/////////////////////////////////////////////////////////

void Audio::smbFft(float * fftBuffer, long fftFrameSize, long sign, int windowNumber, int whichChannel) {
	/* 
	 FFT routine, (C)1996 S.M.Bernsee. Sign = -1 is FFT, 1 is iFFT (inverse)
	 Fills fftBuffer[0...2*fftFrameSize-1] with the Fourier transform of the time domain data 
	 in fftBuffer[0...2*fftFrameSize-1]. 
	 The FFT array takes and returns the cosine and sine parts in an interleaved manner, 
	 ie. fftBuffer[0] = cosPart[0], fftBuffer[1] = sinPart[0], asf. 
	 fftFrameSize must be a power of 2. It expects a complex input signal (see footnote 2), 
	 ie. when working with 'common' audio signals our input 
	 signal has to be passed as {in[0],0.,in[1],0.,in[2],0.,...} asf. 
	 *****In that case, the transform of the frequencies of interest is in fftBuffer[0...fftFrameSize].*/
	
	float wr, wi, arg, *p1, *p2, temp;
	float tr, ti, ur, ui, *p1r, *p1i, *p2r, *p2i;
	long i, bitm, j, le, le2, k, logN;
	logN = (long)(log(fftFrameSize)/log(2.)+.5);
	for (i = 2; i < 2*fftFrameSize-2; i += 2) {
		
		for (bitm = 2, j = 0; bitm < 2*fftFrameSize; bitm <<= 1) {
			if (i & bitm) j++;
			j <<= 1;
		}
		
		if (i < j) {
			p1 = fftBuffer + i; p2 = fftBuffer + j;
			temp = *p1; *(p1++) = *p2;
			*(p2++) = temp; temp = *p1;
			*p1 = *p2; *p2 = temp;
		}
		
	}
	
	for (k = 0, le = 2; k < logN; k++) {
		le <<= 1;
		le2 = le>>1;
		ur = 1.0;
		ui = 0.0;
		arg = MY_PI / (le2>>1);
		wr = cos(arg);
		wi = sign*sin(arg);
		for (j = 0; j < le2; j += 2) {
			p1r = fftBuffer + j; p1i = p1r+1;
			p2r = p1r+le2; p2i = p2r+1;
			for (i = j; i < 2*fftFrameSize; i += le) {
				
				tr = *p2r * ur - *p2i * ui;
				ti = *p2r * ui + *p2i * ur;
				*p2r = *p1r - tr; *p2i = *p1i - ti;
				*p1r += tr; *p1i += ti;
				p1r += le; p1i += le;
				p2r += le; p2i += le;
			}
			
			tr = ur*wr - ui*wi;
			ui = ur*wi + ui*wr;
			ur = tr;
		}
	}
	
	/*In that case, the transform of the frequencies of interest is in fftBuffer[0...fftFrameSize].*/
	for (int i = 0; i < fftFrameSize; i++) {
		//fill the window struct with the fft data
		
		if (whichChannel == 0) //left 
			windows[windowNumber].reImDataL[i] = fftBuffer[i];
		
		else if (whichChannel == 1) //right
			windows[windowNumber].reImDataR[i] = fftBuffer[i];
	}
}

/*	 Spectrum Bars =  FFT Buffer / 2 
 
	 For instance: a FFT of 1024 PTS gives a representation of 512 bars.
 
	 The number of FFT points also gives the frequency resolution, that is  
	 how many Hz are represented by a single bar (that is how large the every single 
	 bar band filter is). An FFT bar is also called "FFT bin" in literature to emphasize 
	 that it contains the energy (or effective voltage) from a frequency range, not a 
	 single frequency (english: "bin" ~ bucket, container; german: "Eimer", italian: bidone). 
	 You can see it this way: For SR = 44100 Hz, a frequency range of  22050 Hz is equally split 
	 into 512 bins (or "bars"), every bin is 22050 / 512 = 43.066 Hz wide. 
	 
	 Frequency Resolution = Sample Rate / FFT Points = Frequency range / Bins numbers 
*/

void Audio::fftPost(int whichChannel, int sampleRate) {
	int windowNumber = totalWindows;  //computed at init
	float re, im;
	//float max = 0;	
	for (int j = 0; j < windowNumber; j++) {
		
		for (int k = 0; k < WINDOW_SIZE/2; k++) {
			re = 0.;
			im = 0.;
			
			windows[j].frequency[k] = k * sampleRate/WINDOW_SIZE ;
				
			if (whichChannel == 0) {
				re = windows[j].reImDataL[k*2] ;
				im = windows[j].reImDataL[k*2 + 1]; 
				
				//magnitudes = sqrt of re2 + im2
				windows[j].rawMagL[k] = sqrt(re * re + im * im);
				//or
				windows[j].magnitudeL[k] = 2 * sqrt(re * re + im * im)/(float)WINDOW_SIZE;
			}
			
			else if (whichChannel == 1)  {
				re = windows[j].reImDataR[k*2] ;
				im = windows[j].reImDataR[k*2 + 1];
				
				windows[j].rawMagR[k] = sqrt(re * re + im * im);
				windows[j].magnitudeR[k] = 2 * sqrt(re * re + im * im)/(float)WINDOW_SIZE;
			}
		}
	}
	
	if (whichChannel == 0) printf("finishing fftPost on L channel\n");
	else if (whichChannel == 1)  printf("finishing fftPost on R channel\n");
}

////////////////////////////////////////////////////*beat*detection*///////////////////////////////////////////////////////

void Audio::computeInstantEnergy(float * isoundDataForBeats) {
	float leftEnergy = 0., rightEnergy = 0.;
	float instantEnergy = 0.;
	float * soundDataForBeats;
	
	for (int i = 0; i < totalWindows; i ++) {
		instantEnergy = 0.;
		leftEnergy = 0.;
		rightEnergy = 0.;
		
		soundDataForBeats = isoundDataForBeats + (i * WINDOW_SIZE * 2);
												//1024 of L and 1024 of R
		
		for (int j = 0; j < WINDOW_SIZE; j += 2) {
			
			leftEnergy += soundDataForBeats[j] * soundDataForBeats[j];
			rightEnergy += soundDataForBeats[j + 1] * soundDataForBeats[j + 1];
			//printf("SEFB %f lEnergy %f\n", soundDataForBeats[j], leftEnergy);
		}
		instantEnergy = leftEnergy + rightEnergy;
		windows[i].instantEnergy = instantEnergy;
		//printf("iE = %f\n", windows[i].instantEnergy);
	}
}

void Audio::findBeats() {
	float C = 0., V = 0.;	//energy variance
	float averageEnergy, average;
	
	for (int h = 0; h < 43; h++) windows[h].beat = 0;
	//for the first second we won't find a beat
						
	for (int k = 43; k < totalWindows; k++) {
			averageEnergy = 0.;
			average = 0.;
			
			for (int m = k - 43; m < k; m ++) {
				//sum the past 43 windows
				averageEnergy += windows[m].instantEnergy;
			}
			///find the average for the subband
			average = averageEnergy/ 43.0; 
			//printf("average = %f\n", average);
			
			V = 0.;
			C = 0.;
			
			for (int m = k - 43; m < k; m ++) {
				//sum the past 43 windows
				V += (windows[m].instantEnergy - average) * (windows[m].instantEnergy - average) ;
			}
			V /= 43.;
			
			if(V > 200) C = 1.0f;
			else if (V < 25) C = 1.45;
			else C = (-0.0025714 * V) + 1.5142857;
			
			///C = (-0.0025714 * V) + 1.5142857;	//his calc
			//C = (-.0025714 * V) + 0.9357143;		//my calc
			
			//when V = 200, C = 1.0 when V = 25, C = 1.45 
			//printf("C = %f, V = %f, is %f > than %f\n", C, V, windows[k].instantEnergy, C * average);
			if (windows[k].instantEnergy > C * average) {
				windows[k].beat = 1;
				//printf("found beat at sub[%d] in windows[%d]\n", j, k);
			}
	}
}
			
	
void Audio::computeInstantEnergySub() {
	float subInstantEnergy;
	//for nonlinear
	
	for (int k = 0; k < totalWindows; k ++) {
	//go through each window
	
		float floatSubSize = 1;

		int subBandIndex = 0;
		for (int bandIndex = 0; bandIndex < WINDOW_SIZE/2; subBandIndex++) {
			int subSize = floor(floatSubSize);
			subInstantEnergy = 0.;
			
			//printf("window %d SUBBAND %d subSize %d\n", k, l, subSize);
			
			int endOfSubBand = bandIndex + subSize;
			for ( ; bandIndex < endOfSubBand && bandIndex < WINDOW_SIZE/2 ; bandIndex ++ ) {
				subInstantEnergy += windows[k].magnitudeL[bandIndex];
				//printf("should go up to 512 each time  = %d\n", m);
				
			}
			windows[k].subEnergy[subBandIndex] = subInstantEnergy / subSize;
			
			//printf("subInstantEnergy = %f of subSize = %d\n", subInstantEnergy, subSize);
			//printf("windows[%d].subEnergy[%d] = %f\n", k, l, windows[k].subEnergy[l]);
			//printf("windows[%d].subEnergy[%d] = %f\n", k, l, windows[k].subEnergy[l]);
			floatSubSize *= 1.12;		
		}	
		
	}
}

void Audio::findBeatsSub() {
	float C, V = 0.;	//energy variance
	float averageEnergy;
	float average;
	
	
	for (int h = 0; h < 43; h++) {
		for (int i = 0; i < SUBBANDS; i++) {
		//for the first second we won't find a beat
		windows[h].subBeats[i] = 0;
		}
	}
	
	for (int k = 43; k < totalWindows; k++) {
		for (int j = 0; j < SUBBANDS; j++) {
			averageEnergy = 0.0;
			average = 0.;

			for (int m = k - 43; m < k; m ++) {
				//sum the past 43 windows
				averageEnergy += windows[m].subEnergy[j];
				//printf("windows[%d].subEnergy[%d] = %f\n", m, j, windows[m].subEnergy[j]);
			}
			///find the average for the subband
			average = averageEnergy/ 43.0; 
			
			float C = 2.;
			//printf("windows[%d].subEnergy[%d] = %f, windows[%d].sub[%d] average = %f\n", k, j, windows[k].subEnergy[j], k, j, average);
			if (windows[k].subEnergy[j] > C * average) 
			{
				windows[k].subBeats[j] = 1;
				//printf("found beat at sub[%d] in windows[%d]\n", j, k);				
			}
			else 
			{
				windows[k].subBeats[j] = 0;
				//printf("NO BEAT\n");
			}
		}
	}
}

////////////////////////////////////////////////////*filteres*///////////////////////////////////////////////////////

void Audio::loPassFilter(float * soundData, int ichannels, int sampleRate) {
	printf("frames = %d, low passing, sampleRate = %d\n", frames, sampleRate);
	float * outputArray = (float *)malloc(sizeof(float) * frames * ichannels);
	
	float r = sqrt(2.); //rez amount, from sqrt(2) to ~ 0.1;
	float f  = 200;		//cutoff frequency
	float c = 1.0 / tan(MY_PI * f / sampleRate);
	float a1, a2, a3, b1, b2;
	
	a1 = 1.0 / ( 1.0 + r * c + c * c);
	a2 = 2 * a1;
	a3 = a1;
	b1 = 2.0 * ( 1.0 - c * c) * a1;
	b2 = ( 1.0 - r * c + c * c) * a1;
	
	outputArray[0] = soundData[0];
	outputArray[1] = soundData[1];
	outputArray[2] = soundData[2];
	outputArray[3] = soundData[3];
	
	for (int i = 2; i < frames * ichannels; i++) {
		float outputL, outputR = 0.;
		//output	= .5 * (soundData[i] + soundData[i - 2]);
		
		outputL = a1 * soundData[i] + a2 * soundData[i - 2] + a3 * soundData[i - 4] - b1 * outputArray[i - 2] - b2 * outputArray[i - 4];
		outputR = a1 * soundData[i + 1] + a2 * soundData[i - 1] + a3 * soundData[i - 3] - b1 * outputArray[i - 1] - b2 * outputArray[i - 3];
		
		outputArray[i] = outputL;
		outputArray[i + 1] = outputR;

		
	}
	
	for (int i = 2; i < frames * ichannels; i++) {
		soundData[i] = outputArray[i];
	}
}


void Audio::hiPassFilter(float * soundData, int ichannels, int sampleRate) {
	printf("frames = %d, high passing, sampleRate = %d\n", frames, sampleRate);
	float * outputArray = (float *)malloc(sizeof(float) * frames * ichannels);
	
	float r = sqrt(2.); //rez amount, from sqrt(2) to ~ 0.1;
	float f  = 6500;		//cutoff frequency
	float c = tan(MY_PI * f / sampleRate);
	
	float a1, a2, a3, b1, b2;
	
	outputArray[0] = soundData[0];
	outputArray[1] = soundData[1];
	outputArray[2] = soundData[2];
	outputArray[3] = soundData[3];

	a1 = 1.0 / ( 1.0 + r * c + c * c);
	a2 = -2*a1;
	a3 = a1;
	b1 = 2.0 * ( c*c - 1.0) * a1;
	b2 = ( 1.0 - r * c + c * c) * a1;
	
	for (int i = 4; i < frames * ichannels; i+=2) {
		float outputL, outputR = 0.;
		//output	= .5 * (soundData[i] + soundData[i - 2]);
		
		outputL = a1 * soundData[i] + a2 * soundData[i - 2] + a3 * soundData[i - 4] - b1 * outputArray[i - 2] - b2 * outputArray[i - 4];
		outputR = a1 * soundData[i + 1] + a2 * soundData[i - 1] + a3 * soundData[i - 3] - b1 * outputArray[i - 1] - b2 * outputArray[i - 3];
		
		outputArray[i] = outputL;
		outputArray[i + 1] = outputR;
	}
	
	for (int i = 2; i < frames * ichannels; i++) {
		soundData[i] = outputArray[i];
	}
}



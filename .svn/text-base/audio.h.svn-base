/*
 *  audio.h
 *  fromScratch
 *
 *  Created by Ivy Feraco on 3/1/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MYSTERIOUS
#define MYSTERIOUS


#include <Carbon/Carbon.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <strings.h>

#include <math.h>

#include "sndfile.h"
#include "portaudio.h"


#define NUM_SECONDS 10
#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 256
#define MY_PI 3.14159265358979323846
#define WINDOW_SIZE 1024
#define SUBBANDS 64

using namespace std;

struct Window {
				float * reImDataL;
				float * reImDataR;
				
				float * frequency;
				
				float * rawMagL;
				float * rawMagR;
				
				float * magnitudeL;
				float * magnitudeR;
				
				float * subEnergy;
				  int * subBeats;
				  int	beat;
				float	instantEnergy;

};

class Audio {

		public:
			Audio(); 
		
			unsigned frames, sampleRate, position, channels;
			bool reProcess;

			
			Window * windowDataToWrite;				
			float * windowFDataToRead;
			  int * windowIDataToRead;
			float * soundDataToPlay;
				
			long double start;
			long double playbackTime;
			const PaStreamCallbackTimeInfo* myTime;
			PaStream * stream;
			
			void getPAError(PaError err);
			
			void initSND();
			void initPA();	
			
			Window * initWindowData(); 
			Window * getWindows();
			int getTotalWindows();
	
			long double getPlaybackTime(); 
			void stopPA();
			void stopSND();

		private:
			Window * windows;
			int totalWindows;
	
			SNDFILE * myFile;
			SF_INFO sfInfo;
			char * sndFile;
			char * binFile;
			const char * error;
			
			
	
			PaError err;
			
			float * soundDataForBeats;
			float * soundDataToProcessL; //for fft and processing, left channel
			float * soundDataToProcessR; //for fft and processing, right channel

			//calls initWindows + populates them if binFile is present
			void populateWindows(float * fwindowData, int * iwindowData);
			void createEmptyWindows(int iframes);
			int windowLimiter();
			
			//a two part fft process
			void smbFft(float *fftBuffer, long fftFrameSize, long sign, int windowNumber, int whichChannel); 

			void fftPost(int whichChannel, int sampleRate);			
			//takes the interleaved data from smbFft and computes magnitude from re and im

			//finds beats using simple energy analysis
			void computeInstantEnergy(float * soundDataForBeats);
			void findBeats();

			//finds beats using subbanded energy analysis
			void computeInstantEnergySub();
			void findBeatsSub();

			void loPassFilter(float * soundData, int ichannels, int sampleRate);
			void hiPassFilter(float * soundData, int ichannels, int sampleRate);
		
};


			
#endif
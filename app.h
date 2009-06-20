/*
 *  app.h
 *  fromScratch
 */

#ifndef APP
#define APP

#include <Carbon/Carbon.h>
#include <GLUT/glut.h>

#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

#include "audio.h"
#include "lissajous.h"
#include "ring.h"
#include "analysis_view.h"

class App {
	
	public:
		App();
		
		//Our callbacks
		void myInit(int ifullW, int ifullH, int iW, int iH);
			void initAudio();
			void initAnimation();
			void initLissajousAnimation();
			void initRingAnimation();

		void myIdle();
		void myDraw();
			void lissajousDraw();
			void ringDraw();
			void analysisDraw();

		void myKey		( unsigned char key, int x, int y );
		void myMouseUp  ( int button, int x, int y );
		void myMouseDown( int button, int x, int y );
		void myMouseDrag( int x, int y );
		void myMouseMove( int x, int y );
		void myReshape( int width, int height );

		void simulateTick();		
		void cameraHome();
		void cameraSet();
	
		void windowLoopCheck();
		void resetLoopAndAnimation();

		
	private: 
		Audio * appAudio;
		Lissajous * LISS[NUMBER_OF_LISSAJOUS];
		ring * RINGS[NUMBER_OF_RINGS];
		AnalysisView * analyzer;
		
		void (App::*currentDrawFunc)();
		
		int mouseX, mouseY;
		int windowWidth, windowHeight;
		int fullWindowWidth, fullWindowHeight;
		
		//for syncing
		float currentTime;
		long double playbackTime;
		unsigned int currentWindow, windowMaxIndex;
		Window * theWindows;
		float * timer;
	
		bool allLissOn;
	
	
	
};


#endif 
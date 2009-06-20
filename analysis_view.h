/*
 *  analysis_view.h
 *  fromScratch
 *
 *  Created by Ivy Feraco on 3/29/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANALYSIS
#define ANALYSIS

#include <Carbon/Carbon.h>
#include <GLUT/glut.h>
#include "eq.h"

#define NUMBER_OF_EQS 128

class AnalysisView {
	
	private:
		Eq * EQ[NUMBER_OF_EQS];
		
		float magLHistory[NUMBER_OF_EQS][EQ_POINTS];
		float magRHistory[NUMBER_OF_EQS][EQ_POINTS];
		int historyCounter;
		
		float * subEnergy;
		int * subBeats;
		int beat;
		
		void drawClipping();
		void drawAxes();
		void drawSubbands();
		void drawMagnitudes();
		void runAnalysis();
	
	public:
		
		AnalysisView(int isubbands);
		int subbands;
		
		void setCurrentWindowValues(float * imagL, float * imagR, float * isubEnergy, int * subBeats, int ibeat);
		
		~AnalysisView();
};

#endif
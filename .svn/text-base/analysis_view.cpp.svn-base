/*
 *  analysis.cpp
 *  fromScratch
 *
 *  Created by Ivy Feraco on 3/29/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "analysis_view.h"

AnalysisView::AnalysisView(int isubbands) {
	subbands = isubbands;
	historyCounter = 0;
	
	for (int i = 0; i < NUMBER_OF_EQS; i++) {
		EQ[i] = new Eq();
		EQ[i]->eqSet( -.8, 0.0, 1.0, 1.0, 1.0, 1.0 - i/NUMBER_OF_EQS, 0.0, 0.0, i); 
		//magLHistory[i] = NULL;
		//magRHistory[i] = NULL;
	}
}


void AnalysisView::drawClipping() {
	//draw near clipping
	glColor3f (0., 1., 0.0);
	glBegin(GL_LINE_LOOP);
		glVertex3f(-.62, -0.41, -1);
		glVertex3f( .62, -0.41, -1);
		glVertex3f( .62,  0.41, -1);
		glVertex3f(-.62,  0.41, -1);
	glEnd();
	
	//draw far clipping
	glColor3f (1., 1., 0.0);
	glBegin(GL_LINE_LOOP);
		glVertex3f(-.62 * 1000, -0.41 * 1000, -1000);
		glVertex3f( .62 * 1000, -0.41 * 1000, -1000);
		glVertex3f( .62 * 1000,  0.41 * 1000, -1000);
		glVertex3f(-.62 * 1000,  0.41 * 1000, -1000);
	glEnd();
}		

void AnalysisView::drawAxes() {
	//axes
	glColor3f (1.0, 0.0, 0.0);
	
	glBegin(GL_LINES);
		glVertex3f(-1.24, 0., -2);
		glVertex3f( 1.24, 0., -2);
	glEnd();
	
	glColor3f (0.0, 1.0, 0.0);
	
	glBegin(GL_LINES);
		glVertex3f(0., -.82, -2.);
		glVertex3f(0., .82, -2.);
	glEnd();
}
	
void AnalysisView::setCurrentWindowValues(float * imagL, float * imagR, float * isubEnergy, int * isubBeats, int ibeat) {
	subEnergy = isubEnergy;
	subBeats = isubBeats;
	beat = ibeat;
	
	memcpy(magLHistory[historyCounter],imagL, EQ_POINTS* sizeof(float));
	memcpy(magRHistory[historyCounter],imagR, EQ_POINTS* sizeof(float));
	runAnalysis();
	
	historyCounter++;
	historyCounter %= NUMBER_OF_EQS;
	
}
		
void AnalysisView::drawSubbands() {
		for (int i = 0; i < subbands; i++) {
			
			glColor3f(1., 1., 1.);
			float height = subEnergy[i];
			
			if (subBeats[i]) {
				glColor3f(0., 0., 1.);
				//printf("*");
			}
			//else printf("|");
			
			glBegin(GL_LINE_LOOP);
				glVertex3f(-.8 + (i * 1.6/subbands), -.4, 0);
				glVertex3f(-.8 + (i * 1.6/subbands) + 1.2/subbands, -.4, 0);
				glVertex3f(-.8 + (i * 1.6/subbands) + 1.2/subbands, -.8, 0);
				glVertex3f(-.8 + (i * 1.6/subbands), -.8, 0);
			glEnd();
			
			glColor3f(1., 0., 1.);
			
			glBegin(GL_LINE_LOOP);
				glVertex3f(-.8 + (i * 1.6/subbands), -.8 + height * .4, 0);
				glVertex3f(-.8 + (i * 1.6/subbands) + 1.2/subbands,  -.8 + height * .4, 0);
				glVertex3f(-.8 + (i * 1.6/subbands) + 1.2/subbands, -.8, 0);
				glVertex3f(-.8 + (i * 1.6/subbands), -.8, 0);
			glEnd();
		}
		
		if (beat) glRectf(.9 , -.4, 1.1, -.2);
}

void AnalysisView::drawMagnitudes() {
	int index = historyCounter;
	for (int i = 0; i < NUMBER_OF_EQS; i++, index--) {
		if (magLHistory[index] == NULL) 
			break;
		if (index < 0) {
			index = NUMBER_OF_EQS-1;
		}		
		
		EQ[i]->eqSetHeights(magLHistory[index],  magRHistory[index],  beat);
		EQ[i]->runEq();
		//glPushMatrix();
		//glPopMatrix();
		//glFlush();
	}
}

void AnalysisView::runAnalysis() {

	drawMagnitudes();
	drawSubbands();
	drawAxes();
	drawClipping();
}

AnalysisView::~AnalysisView() 
{
	printf("goodbye analysis view/n");
}


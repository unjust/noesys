/* equalizer.cpp * PA4 */

#include "eq.h"

Eq::Eq() {
	//printf("default eqBar constructor called\n");
	heightL = (float *)malloc(sizeof(float) * EQ_POINTS);
	heightR = (float *)malloc(sizeof(float) * EQ_POINTS);
	
	for (int i = 0; i < EQ_POINTS; i++) {	
		heightL[i] = 1.0;
		heightR[i] = 1.0;
	}
	
	x = 0.;
	y = 0.;
	R = 60.;
	G = 60.;
	B = 60.;
	
	beat = 0;
	window = 0;
}

void Eq::eqSet(float ix, float iy,  float iR, float iG, float iB, float iA, float iheightL, float iheightR, int iwindow) {
	R = iR;
	G = iG;
	B = iB;
	A = iA;
	x = ix;
	y = iy;
	
	for (int i = 0; i < EQ_POINTS; i ++) {	
		heightL[i] = iheightL;
		heightR[i] = iheightR;
	}
	
	window = iwindow;
	beat = 0;
	
}	

//may not need it
void Eq::eqSetHeights(float * iheightL, float * iheightR, int ibeat)  {
	beat = ibeat;
	for (int i = 0; i < EQ_POINTS; i++) {	
		heightL[i] = iheightL[i];
		heightR[i] = iheightR[i];
		//make sure horizontal spacing of eq points and height scaling is correct
	}
}


void Eq::runEq() {
	if (window == 0 && !beat) glColor4f(0., 0., 1., 1);
	else if (beat) glColor4f(1., 1., 0., 1.0);
	else glColor4f(R, G, B, 1 );
	
	//draw L
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < EQ_POINTS; i ++) {	
		glVertex3f(x + 2.48 * (i/(float)EQ_POINTS) , y + heightL[i]  * 0.82, (float)window * -.05); 
	}
	glEnd();
		
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < EQ_POINTS; i ++) {	
		glVertex3f(x + 1. + 2.48 * (i/(float)EQ_POINTS) , y + heightR[i]  * 0.82, (float)window * -.05); 
		//make sure horizontal spacing of eq points and height scaling is correct
		//why 10 * ??
	}
	glEnd();
}

 Eq::~Eq() {
	printf("goodbye/n");
}


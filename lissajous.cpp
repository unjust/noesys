/**  lissajous.cpp **/

#include "lissajous.h"

Lissajous::Lissajous() {
	xRadius = 1.;
	yRadius = 1.;
	zRadius = 1.;
	
	alive = false;
	expanding = false;
	
	phaserA = 0., phaserB = 0., counter = 0.;
	xCenter = .0;
	yCenter = .0;
	zCenter = 0.;
	
	R = .60;
	G = .60;
	B = .60;
	A = 1.;
	
	nPoints = 200;
	coils = 0.;
	stretch = 0.;
}

Lissajous::Lissajous(int iident, float ixCenter, float iyCenter, float izCenter, float ixRadius, 
					 float iyRadius, float iR, float iG, float iB, float iA, bool ialive)
{
	ident = iident;
	//printf("lissajousSet called\n");
	xRadius = ixRadius;
	yRadius = iyRadius;
	
	R = iR;
	G = iG;
	B = iB;
	A = iA;
	
	xCenter = ixCenter;
	yCenter = iyCenter;
	zCenter = izCenter;
	
	xEndCenter = xCenter;
	yEndCenter = yCenter;
	zEndCenter = zCenter;
		
	counter = PI_2;
	nPoints = 500;
	coils = 0.;
	stretch = 1.;
	
	alive = ialive;
	
	selected = false;
	expanding = false;
	acceleration = 1.;
	xacceleration = 1.;
	threshhold = .92;
	expansionFactor = .001;
}	

void Lissajous::lissajousPosition(float ixCenter, float iyCenter, float izCenter) {
	xEndCenter = ixCenter;
	yEndCenter = iyCenter;
	zEndCenter = izCenter;
}

void Lissajous::lissajousMove() {
	if (xCenter == xEndCenter && yCenter == yEndCenter && zCenter == zEndCenter) return;
	
	float xStartCenter = xCenter;
	float yStartCenter = yCenter;
	float zStartCenter = zCenter;
	
	acceleration += .03;
	
	xCenter += acceleration * (.001 * (xEndCenter - xStartCenter));
	yCenter += acceleration * (.001 *  (yEndCenter - yStartCenter));
	zCenter += acceleration * (.001 *  (zEndCenter - zStartCenter));
	
	if (fabs(xEndCenter - xStartCenter) < .001)  {
		acceleration = 1.;
	}
	//printf("LISS %d accel = %f\n", ident, acceleration);
}

void Lissajous::lissajousExpansion() {
	xRadius += expansionFactor * xacceleration;
	xacceleration += .07;
	A -= .001;
}

void Lissajous::lissajousCoiling(float ixRadius, float icoils) {
	if (expanding) {
		lissajousExpansion();
		if (A < 0.1) alive = false;
		return;
	}
	if (selected && ixRadius > threshhold) {
		expanding = true;
		return;
	}
	
	float startCoils = coils;
	float endCoils   = icoils;
	
	coils += (.001 * (endCoils - startCoils));
	
	float startXRadius = xRadius;
	float endXRadius = ixRadius;
	
	xRadius += (.001 * (endXRadius - startXRadius));
}

void Lissajous::runLissajous() {
	//counter +=;
	glColor4f(R, G, B, A);
	
	lissajousMove();
	
	if (alive && A < 1. && expanding == false) {
		A += .001;
	}
	
	phaserA += .008;
	phaserB += .003;
	counter += .0001;
	float x, y, z;
	
	glPointSize(4.0);
	
	glBegin(GL_POINTS); 
	
	float pointNum, circleTime, r;
	r = yRadius * 6.;
	for (int point = 1; point < nPoints; point++) {
		pointNum = (float)point/nPoints;
		circleTime =  (-90 + 360) * pointNum * DEG2RAD;
		x = xCenter + cos(circleTime) * xRadius;
		
		y = (sin(circleTime) * yRadius + pointNum * counter * coils * r);
		y = yCenter  - (r/2.) + fmod(y + (r/2.0), r);
		
		z = zCenter;
		//z = zCenter + tan(circleTime) * zRadius;

		glVertex3f(x, y, z); 
	}
	glEnd();
}

bool Lissajous::status() {
	return alive;
}

bool Lissajous::status(bool istatus) {
	alive = istatus;
	return	alive;
}


Lissajous::~Lissajous() {
	printf("goodbye Lissajous/n");
}



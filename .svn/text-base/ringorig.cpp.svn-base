/*  ring.cpp * PA4 */

#include "ring.h"

ring::ring() 
{
	//printf("default ring constructor called\n");
	radius, finalRadius = 0.;
	alive = false;
	ident = 0;
	
	x, y = 0;
	R, G, B = .6;

	A = 1.0;
}

void ring::ringSet(int iident, float ix, float iy, float iz, float iR, float iG, float iB, float iA, float ifinalRadius) 
{
	radius = 0.;
	
	R = iR;
	G = iG;
	B = iB;
	A = iA;
	
	ident = iident;
	x = ix;
	y = iy;
	z = iz;
	
	
	
	float commonRadius = .4;
	centerX = commonRadius * sin(ident * 360./NUMBER_OF_RINGS) + x;
	centerY =  commonRadius * cos(ident * 360./NUMBER_OF_RINGS) + y;
	centerZ = 0.;
	
	startX = centerX , startY = centerY, startZ = centerZ ;
	endX = centerX, endY = centerY, endZ = centerZ;
	step = ident/10000.;
	finalRadius =  ifinalRadius;
	alive = true;
	bounce = false;
	counter = 0.;
	acceleration = 0.;
	ready = false;
	positionType = 0;
}	

void ring::ringBounce()
{
	bounce = true;
}

void ring::moveRing()
{
	startX = centerX;
	startY = centerY;
	startZ = centerZ;
	
	if (fabs(endZ - startZ) < .1) 
	{
		ready = true;
	}
	else ready = false;
	
	//printf("accleration = %f\n", acceleration);
	centerX += (.001 + acceleration/10.) * (endX - startX);
	centerY += (.001 + acceleration/10.) * (endY - startY);
	//z += (.001 + acceleration/10.) * (endZ - startZ);
}


void ring::ringPosition(float ix, float iy) 
{
	//x = ix * 2.6/NUMBER_OF_RINGS * 6.;
}

void ring::ringPosition() 
{
	
	if (positionType == 0)
	{
		float commonRadius = .4;
		//printf("%d\n");
		endX = commonRadius * sin(ident + 1 * 360./NUMBER_OF_RINGS);
		endY =  commonRadius * cos(ident + 1 * 360./NUMBER_OF_RINGS);
		//endZ = 0.;
	}
	//else if (switcher == 1)
	//{
	//	endY = -.1 + (.6/NUMBER_OF_RINGS) * ident;
	//	endX = 0.;
	//	endZ = ident * .01;
		//printf("%d\n");
	//}
}

void ring::ringFinalRadius(float ifinalRadius) 
{
	finalRadius = ifinalRadius;
	alive = true; //turned on
}

void ring::runRing(int ipositionNum, float iacceleration) 
{
	//printf("RUNNING\n");
	positionType = ipositionNum;
	acceleration = iacceleration;
	GLint circle_points = 100; 
	
	moveRing();
	
	if (radius < finalRadius)
	{
		ringPosition();
		
		glColor4f(R, G, B, A);
		
		//glBegin(GL_POLYGON); 
		glBegin(GL_LINE_LOOP); 
		
		for (int i = 0; i < circle_points; i++) 
		{    
			float angle = 2 * PI * i/circle_points; 
			glVertex3f(centerX + cos(angle) * radius, centerY + sin(angle) * radius, centerZ); 
		} 
		
		glEnd();
		//glEnd();
		
		radius += .0005;
		//counter += step;
		if (A > 0.) A -= .001;
	}
	else 
	{
		//bounce = false;
		alive = false; //turned off ready for new value
		radius = 0.0;
		A = 1.0;
	}
	
}

bool ring::status()
{
	return alive;
}

 ring::~ring() 
{
	printf("goodbye/n");
}


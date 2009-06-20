/*  ring.cpp * PA4 */

#include "ring.h"

ring::ring() 
{
	//printf("default ring constructor called\n");
	radius = 0.;
	finalRadius = 0.;
	alive = false;
	ident = 0;
	positionRadius = .4;
	x = 0;
	y = 0;
	R = .6;
	G = .6;
	B = .6;
	A = 1.0;
}

void ring::ringSet(int iident, float ix, float iy, float iz, float iR, float iG, float iB, float iA, float ifinalRadius) 
{
	//printf("ringSet called\n");
	radius = 0.;
	
	R = iR;
	G = iG;
	B = iB;
	A = iA;
	
	ident = iident;
	x = ix;
	y = iy;
	z = iz;
	
	startX = x, startY = y, startZ = z;
	endX = x, endY = y, endZ = z;
	//finalRadius =  rand() % 255;									//is this working?
	step = 1/10000.;
	finalRadius =  ifinalRadius;
	alive = true;
	bounce = false;
	counter = 0;
	acceleration = 0.;
	ready = false;
	switcher = 0;
	startX = positionRadius * sin(ident * PI_2/NUMBER_OF_RINGS);
	startY = positionRadius * cos(ident * PI_2/NUMBER_OF_RINGS);
}	

void ring::ringBounce()
{
	bounce = true;
	printf("BOUNCE\n");
}

void ring::moveRing()
{
	startX = x;
	startY = y;
	startZ = z;
	
	if (abs(endZ - startZ) < .1) 
	{
		ready = false;
	}
	else ready = true;
	
	x += (.001 + acceleration) * (endX - startX);
	y += (.001 + acceleration) * (endY - startY);
	//z += (.001 + acceleration/10.) * (endZ - startZ);
	//fprintf("x, y = %f %f\n", x,y );
}



void ring::ringPosition() 
{
	
	//x = ix * 2.6/NUMBER_OF_RINGS * 6.;
	
	if (switcher == 0)
	{
		endX = positionRadius * sin(ident * PI_2/NUMBER_OF_RINGS + counter);
		endY = positionRadius * cos(ident * PI_2/NUMBER_OF_RINGS + counter);
		//endZ = 0.;
	}
	else if (switcher == 1)
	{
		endY = -.1 + (.6/NUMBER_OF_RINGS) * ident;
		endX = 0.;
		//endZ = ident * .01;
		printf("SWITCH************************* \n");
	}
}

void ring::ringFinalRadius(float ifinalRadius) 
{
	finalRadius = ifinalRadius;
	alive = true; //turned on
}

void ring::runRing(int iswitcher, float iacceleration) 
{
	printf("RUNNING\n");
	switcher = iswitcher;
	acceleration = iacceleration;
	GLint circle_points = 100; 
	
	if (radius < finalRadius)
	{
		ringPosition();
		moveRing();
	
		if (bounce) 
		{
			glColor4f(R, G, B, A);
			
			glBegin(GL_POLYGON); 
			for (int i = 0; i < circle_points; i++) 
			{    
				float angle = 2 * PI * i/circle_points; 
				//glVertex3f(-1.6 + x + cos(angle) * radius, sin(angle) *  radius, 0.0); 
				glVertex3f(x + cos(angle) * radius, y + sin(angle) * radius, z); 
			} 
			
			glEnd();
		}		
		
		glColor4f(R, G, B, A);
		glBegin(GL_LINE_LOOP); 
		
		for (int i = 0; i < circle_points; i++) 
		{    
			float angle = 2 * PI * i/circle_points; 
			//glVertex3f(-1.6 + x + cos(angle) * radius, sin(angle) *  radius, 0.0); 
			glVertex3f(x + cos(angle) * radius, y + sin(angle) * radius, 0); 
		} 
		
		glEnd();
		radius += .0001;
		counter += .001;
		if (A > 0.) A -= .0001;
	}
	else 
	{
		bounce = false;
		alive = false; //turned off ready for new value
		radius = 0.0;
		A = 1.0;
		//counter = 0.;
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


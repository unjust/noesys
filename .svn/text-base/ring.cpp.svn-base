/*  ring.cpp * PA4 */

#include "ring.h"

ring * RINGS[NUMBER_OF_RINGS];


ring::ring() 
{
	printf("default ring constructor called\n");
	alive = false;
	
	currentRadius = 0., finalRadius = 1.;
	ident = 0;
	
	orbitRadius = .4;
	x = 0, y = 0;
	R = .6, G = .6, B = .6;
	A = 1.0;
}

ring::ring(int iident, float ix, float iy, float iz, float iR, float iG, float iB, float iA, float ifinalRadius) 
{
	orbitRadius = .2;
	currentRadius = 0.;
	finalRadius =  .2;
	
	R = iR, G = iG, B = iB;
	A = iA;
	
	ident = iident;
	x = ix, y = iy, z = iz;
	
	startX = x, startY = y, startZ = z;
	endX = x, endY = y, endZ = z;
	//finalRadius =  rand() % 255;									//is this working?
	
	step = 1/10000.;
	
	
	
	alive = true;
	ready = false, subBeat = false;
	
	counter = 0;
	acceleration = 0.;
	
	switcher = 0;
	//startX = orbitRadius * sin(ident * PI_2/NUMBER_OF_RINGS);
	//startY = orbitRadius * cos(ident * PI_2/NUMBER_OF_RINGS);
}	

void ring::ringBounce()
{
	subBeat = true;
	//printf("BOUNCEDDDDDDDD\n");
}

void ring::moveRing()
{
	startX = x;
	startY = y;
	startZ = z;
	
	//if (fabs(endZ - startZ) < .1) 
//		ready = false;
//	else ready = true;
	
	x += (.001 + acceleration) * (endX - startX);
	y += (.001 + acceleration) * (endY - startY);
	//z += (.001 + acceleration/10.) * (endZ - startZ);
	//printf("x, y = %f %f \n", x, y );
	
}



void ring::ringPosition() 
{
	
	//x = ix * 2.6/NUMBER_OF_RINGS * 6.;
	
	if (switcher == 0)
	{
		float anAngleOfSorts = ident * PI_2/NUMBER_OF_RINGS + counter;
		
		endX = orbitRadius * sin(anAngleOfSorts);
		endY = orbitRadius * cos(anAngleOfSorts);
		//printf("ANGLE = %f, endX, endY =%f %f\n", anAngleOfSorts, endX, endY);
		//endZ = 0.;
	}
	else if (switcher == 1)
	{
		endY = -.1 + (.6/NUMBER_OF_RINGS) * ident;
		endX = 0.;
		//endZ = ident * .01;
		//printf("switcher!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! \n");
	}
}

void ring::ringFinalRadius(float ifinalRadius) 
{
	finalRadius = ifinalRadius;
	alive = true; //turned on
}

void ring::runRing(int iswitcher, float iacceleration) 
{
	
	switcher = iswitcher;
	acceleration = .2;
	GLint circle_points = 100; 
	
	if (currentRadius < finalRadius)
	{
		//printf("RUNNING acceleration %f\n", iacceleration);
		ringPosition();
		moveRing();
		if (subBeat)
		{
			glColor4f(R, G, B, A);
			
			glBegin(GL_POLYGON); 
			for (int i = 0; i < circle_points; i++) 
			{    
				float angle = 2 * PI * i/circle_points; 
				float anX = x + sin(angle) * currentRadius;
				float aY = y + cos(angle) * currentRadius;
				float aZ = z;
				glVertex3f(anX, aY, aZ); 
				
			} 
			glEnd();
		}		
		
		glColor4f(R, G, B, A);
		glBegin(GL_LINE_LOOP); 
		
		for (int i = 0; i < circle_points; i++) 
		{    
			float angle = 2 * PI * i/circle_points; 
			//glVertex3f(-1.6 + x + cos(angle) * radius, sin(angle) *  radius, 0.0); 
			float anX = x + sin(angle) * currentRadius;
			float aY = y + cos(angle) * currentRadius;
			float aZ = z;
			glVertex3f(anX, aY, 0); 
			//printf("hello %f %f %f \n", anX, aY, aZ);
		} 
		
		glEnd();
		currentRadius += .0001;
		counter += .001;
		if (A > 0.) A -= .001;
	}
	else 
	{
		subBeat = false;
		alive = false; //turned off ready for new value
		currentRadius = 0.0;
		A = 1.0;
		//counter = 0.;
	}
	
}

bool ring::status()
{
	return alive;
}

bool ring::isReady()
{
	return ready;
}

 ring::~ring() 
{
	printf("goodbye/n");
}


/* ring.h *  PA4 *  Created by ivy on 4/05/06. */

#ifndef RINGS_
#define RINGS_

#include <Carbon/Carbon.h>
#include <GLUT/glut.h>
#include "audio.h"
#define NUMBER_OF_RINGS SUBBANDS
//#define RINGS_POINTS WINDOW_SIZE/2

#define PI 3.14159265
#define PI_2 6.28318531

class ring {
	
	private:
		float x, y, z;
		float centerX, centerY, centerZ;
		float startX, startY, startZ;
		float endX, endY, endZ;
		float R, G, B, A;
		
		float orbitRadius;
		float currentRadius; 
		float finalRadius;
	
		int ident;
		int positionType;
		float step;
		float counter;
		float acceleration;
		
		int switcher;
		bool subBeat;
		bool alive;
		bool ready;
	
		
	public:
		ring();
		ring(int iident, float ix, float iy, float iz, float iR, float iG, float iB, float iA, float ifinalRadius);
		
		void ringPosition(float ix, float iy);
		void ringPosition();
	
		void ringBounce();
		void moveRing();
		void ringFinalRadius(float ifinalRadius);
		void runRing(int ipositionNum, float iacceleration);
	
		bool isReady();
		bool status();
		~ring();
	
};


//we'll be getting half as many frequencies as the window

#endif
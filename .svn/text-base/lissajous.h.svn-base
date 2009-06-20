/**  lissajous.h **/

#ifndef LISSAJOUS
#define LISSAJOUS

#include <Carbon/Carbon.h>
#include <GLUT/glut.h>

#define NUMBER_OF_LISSAJOUS 9

#define PI 3.14159265
#define PI_2 6.28318531
#define DEG2RAD PI/180


class Lissajous {
	public:
		bool selected;
		float acceleration;
		float A;
		float xCenter, yCenter, zCenter;
		float xRadius, yRadius, zRadius; 
		
		Lissajous();
		Lissajous(int iident, float ixCenter, float iyCenter, float izCenter, float ixRadius, float iyRadius, float iR, float iG, float iB, float iA, bool ialive);
		void lissajousPosition(float ixCenter, float iyCenter, float izCenter);
		void lissajousMove();
		void lissajousExpansion();
		void lissajousCoiling(float ixRadius, float icoils);
		void runLissajous();
		
		bool status();
		bool status(bool istatus);
		~Lissajous();

	private:
		float xEndCenter, yEndCenter, zEndCenter;
		float R, G, B;
		
		int ident;
		int nPoints;
		float coils;
		float stretch;
		
		float counter, phaserA, phaserB, expansionFactor;
		
		float threshhold,  xacceleration;
		
		bool alive;
		bool expanding;
		
			
};



//we'll be getting half as many frequencies as the window

#endif



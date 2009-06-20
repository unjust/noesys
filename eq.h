/* eq.h * PA4 */

#ifndef EQCLASS
#define EQCLASS

#include <Carbon/Carbon.h>
#include <GLUT/glut.h>
#define EQ_POINTS 1024/2

class Eq {
	
	public:
			
		Eq();
		void eqSet(float ix, float iy, float iR, float iG, float iB, float iA, float iheightL, float iheightR, int iwindow);
		void eqSetHeights(float * iheightL, float * iheightR, int beat);
		void runEq();
		~Eq();
		
	private:
		float x, y;
		float R, G, B, A;
		float * heightL; 
		float * heightR; 
		int window;
		int beat;
		
};


//we'll be getting half as many frequencies as the window

#endif



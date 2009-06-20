/*
 *  app.cpp
 *  fromScratch
 *  Updated 03.12.09.
*/

#include "app.h"

#define MUSIC_CHANGE_POINT 136
float background[3] = {0., 0., 0.}; 

//global transforms + camera
float translateX = 0, translateY = 0, translateZ = -2.;
float scaleX = 1, scaleY = 1, scaleZ = 1.;
float rotateAngle = 0, rotateX = 0, rotateY = 0, rotateZ = 0.;
double rotateAngleX = 0, rotateAngleY = 0, rotateAngleZ = 0;

//screen calculations for lissajous
float xRight = .86;
float xLeft = xRight * -1.;
float yTop = 1.3;
float yBottom = yTop * -1.;
float xFactor = 0.;

//TODO: incorporate into Liss object

float coils = 1.;
float lradius = .4;
int silence = 0;

int ringSwitch = 0;
int ringSubs = 8;
bool fullScreen = false;
bool loopRestart = false;

float beatCount = 0.;

App::App() {
	currentDrawFunc = &App::lissajousDraw;
}

void App::myInit(int ifullW, int ifullH, int iW, int iH) {
	
	windowWidth = iW;
	windowHeight = iH;
    fullWindowWidth = ifullW;
	fullWindowHeight = ifullH;
	printf("W = %d, H = %d\n", windowWidth, windowHeight);
	
	if (fullScreen) {
		glutFullScreen();
		glutReshapeWindow(fullWindowWidth, fullWindowHeight);
	}
	else glutReshapeWindow(windowWidth, windowHeight);
	
	initAudio();
	initAnimation();
	analyzer = new AnalysisView(SUBBANDS);
	
}

void App::initAudio() {
	appAudio = new Audio();
	
	appAudio->initSND();
	windowMaxIndex = appAudio->getTotalWindows() - 1;
	printf("windowMaxIndex = %d\n", windowMaxIndex);
	
	appAudio->initPA();
	appAudio->initWindowData();
	
	//TODO:doc the diff btwn windows and totalWindows
	theWindows = appAudio->getWindows(); 
	currentWindow = 0;
	
}


void App::initAnimation() 
{
	initLissajousAnimation();
	initRingAnimation();
}

void App::myIdle()
{}

void App::simulateTick() 
{}


void App::windowLoopCheck()
{
	bool loopTheData;
	if (loopTheData = (currentWindow >= windowMaxIndex))
	{
		resetLoopAndAnimation();
	}
}

void App::resetLoopAndAnimation()
{
	//printf("reset this bitch \n");
	currentWindow = 0;
	initAnimation();
	currentDrawFunc = &App::lissajousDraw;
}

void App::myDraw() {
	
	//currentTime = timer[currentWindow];
	cameraSet();
	//printf("playbackTime == %f\n", (float)playbackTime);
	
	if ((float)playbackTime > 1.0 && (float)playbackTime < 500.0 && (int)playbackTime % MUSIC_CHANGE_POINT == 0)
		//if (currentTime > 1.0 && currentTime < 500.0 && (int)currentTime % 136 == 0)
	{
		cameraHome();
		currentDrawFunc = &App::ringDraw;
	}
	
	playbackTime = appAudio->getPlaybackTime();
	
	currentWindow = ceil((playbackTime * (long double)44100.)/(long double)WINDOW_SIZE);
	
	bool loopTheData;
	if (loopTheData = (currentWindow >= windowMaxIndex))
		currentWindow = 0;
	
	// Call current drawing function (set by user keys)
	((*this).*currentDrawFunc)();
	
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
}

void App::initLissajousAnimation()
{
	allLissOn = false;
	srand((unsigned)time(0));
	
	for (int i = 0; i < NUMBER_OF_LISSAJOUS; i++) {
		float a, radius, x, y, z;
		bool alive;
		if (i == 0) { 
			a = 0.0;
			radius = 1.3;
			alive = true;
			x = 0.;
			z = -2.0;
		}
		else {
			a = 0.;
			radius = .7;
			alive = false;
			z -= .2;
			//evens on left
			if (i % 2 == 0) {
				x = -.42 - fabs(z) *((i- 1) * radius * (xRight - xLeft)/NUMBER_OF_LISSAJOUS);
			}
			else {
				x = .42 + fabs(z) *( i *  radius * (xRight - xLeft)/NUMBER_OF_LISSAJOUS);
			}
		}
		//printf("x for %d = %f, z = %f\n", i, x, z); // svn test
		
		float r = ((rand() % 40) /100.) + .5;
		float g = ((rand() % 40) /100.) + .5;
		float b = ((rand() % 40) /100.) + .5;
		y = yBottom + (4  * - 1. * z) ;
		
		LISS[i] = new Lissajous(i, x , -1, z, radius, 1.2, r, g, b, a, alive); 
	}	
}


void App::lissajousDraw() {
	
	glClearColor( background[0], background[1], background[2], 1);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
	
	float totalMagnitude = 0.;
	windowLoopCheck();
	
	for (int i = 0; i < WINDOW_SIZE/2; i++) 
	{
		//TODO: error happens here bad access?
		totalMagnitude += theWindows[currentWindow].magnitudeL[i];
	}
	
	//totalMagnitude /= (WINDOW_SIZE/2);
	int frequencySpan = (int)((WINDOW_SIZE/2.)/NUMBER_OF_LISSAJOUS);
	bool selectedLiss = false;
	int theLeader = 0;
	
	//go through Liss and select a Leader | a new Liss must be selected if the previous dies
	for (int i = 0; i < NUMBER_OF_LISSAJOUS; i++) {
		float subMagnitude = 0.;
		if (!LISS[i]->status()) continue; //if its dead carry on
		coils = i + 3;
		
		for (int y = i * frequencySpan; y < (i + 1) * frequencySpan; y++) 
		{
			subMagnitude += theWindows[currentWindow].magnitudeL[y];
		}
		
		if (!selectedLiss) {
			LISS[i]->selected = true;
			LISS[i]->lissajousPosition(0., 0., .4);
			//lissajousPosition(float ixCenter, float iyCenter, float izCenter);
			selectedLiss = true;
		}
		float xCenterPos = LISS[i]->xCenter;
		
		if (LISS[i]->selected == true && (xCenterPos < 0.01 && xCenterPos > -0.01)) {
			LISS[i]->lissajousCoiling((totalMagnitude * .3) + .1, (totalMagnitude + coils) * 1.5);
			theLeader = i;
		}
		else LISS[i]->lissajousCoiling(.1, subMagnitude/2 + coils);
		
		//repositioning all the liss TODO: make more readable
		if (!LISS[i]->selected && theLeader != 0 && i > 2 && (theLeader - i) % 2 == 0) {
			float newXPosition = 0., newYPosition = 0., newZPosition = 0.;
			newZPosition = LISS[i - 1]->zCenter;
			newYPosition = LISS[i - 1]->yCenter;
			float padding = .5;
			if (i % 2 == 0) {
				newXPosition = (LISS[i - 2]->xCenter) - padding - ( 1.5 * (xRight - xLeft)/NUMBER_OF_LISSAJOUS);
			}
			else  {
				newXPosition = (LISS[i - 2]->xCenter) + padding + ( 1.5  * (xRight - xLeft)/NUMBER_OF_LISSAJOUS);
			}
			LISS[i]->lissajousPosition(newXPosition, newYPosition, newZPosition);
		}
	}
	
	//when the first Lissajous is ready, coiled in position, turn the rest on
	if (LISS[0]->status() && LISS[0]->xRadius < .5 && allLissOn == false) {
		for (int i = 1; i < NUMBER_OF_LISSAJOUS; i ++)  LISS[i]->status(true);
		allLissOn = true;
	}
	for (int l = 0; l < NUMBER_OF_LISSAJOUS; l++) {
		LISS[l]->runLissajous();
	}

}

void App::initRingAnimation()
{
	for (int i = 0; i < NUMBER_OF_RINGS; i++) 
		RINGS[i] = new ring(i, 0., 0., 0., 0.529, 0.878, 1.0, 1., 20.); 
	// ring(int ident, float ix, float iy, int iR, int iG, int iB, float iA, float ifinalRadius);	
}

void App::ringDraw() {
	
	glClearColor( 1., 1., 1., 1);	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
	
	//TODO:this might be duped, and needs to be debugged
	float aTotalMagnitude = 0.;
	
	windowLoopCheck();
	for (int i = 0; i < WINDOW_SIZE/2; i++)
		aTotalMagnitude += theWindows[currentWindow].magnitudeL[i];
	
	int ringsInPlace = 0;
	for (int aCurrentRing = 0; aCurrentRing < NUMBER_OF_RINGS; aCurrentRing++)
	{
		float radius = 0.;
		float freqSpanPerRing = (WINDOW_SIZE/2)/NUMBER_OF_RINGS;
		
		if (RINGS[aCurrentRing]->isReady())
			ringsInPlace ++;
		
		for (int i = aCurrentRing * freqSpanPerRing; i < (aCurrentRing + 1) * freqSpanPerRing; i++)
		{
			//current magnitude of entire freq range for that ring
			radius += theWindows[currentWindow].magnitudeL[i];
		}
		//radius /= freqSpanPerRing;
		RINGS[aCurrentRing]->ringFinalRadius(radius * 200.);
		
		if (theWindows[currentWindow].subBeats[aCurrentRing] == 1)
			RINGS[aCurrentRing]->ringBounce();
	}
	
	
	if (ringSwitch == 0 && aTotalMagnitude > 2.) 
		ringSwitch = 1;
	else if (ringSwitch == 1 && aTotalMagnitude > 2.) 
		ringSwitch = 0;
	
	for (int aCurrentRing = 0; aCurrentRing < NUMBER_OF_RINGS; aCurrentRing++)
		RINGS[aCurrentRing]->runRing(ringSwitch, 1);	

		//RINGS[aCurrentRing]->runRing(ringSwitch, (float)playbackTime);	
					
}


void App::analysisDraw() {
	
	glClearColor( background[0], background[1], background[2], 1);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
	
	//make sure we don't overrun the window array
	int windowToDraw = 0;
	if (currentWindow < windowMaxIndex)	
	{
		windowToDraw = currentWindow;
		analyzer->setCurrentWindowValues(theWindows[windowToDraw].magnitudeL,theWindows[windowToDraw].magnitudeR, 
		theWindows[windowToDraw].subEnergy, theWindows[windowToDraw].subBeats, theWindows[windowToDraw].beat);
	}
}

void App::cameraHome() {
	translateX = 0, translateY = 0, translateZ = -2.;
	scaleX = 1, scaleY = 1, scaleZ = 1.;
	rotateAngle = 0, rotateX = 0, rotateY = 0, rotateZ = 0.;
}

void App::cameraSet() {
	//TO DO:check if all are necessary
	glTranslatef(translateX, translateY, translateZ);
	glRotatef(rotateAngle, rotateX, rotateY, rotateZ);
	glRotatef(rotateX, 1, 0, 0);
	glRotatef(rotateY, 0, 1, 0);
	glRotatef(rotateZ, 0, 0, 1);

}

#pragma mark -
#pragma mark GLUT callback implementaions
void App::myKey( unsigned char key, int x, int y ) {

	switch (key) {
		case 'v':
			currentDrawFunc = &App::analysisDraw;
			break;
		
		case 'f':
			if (fullScreen) {
				glutReshapeWindow(windowWidth, windowHeight);
				fullScreen = false;
				printf("reshaping to smaller screen %d, %d\n", windowWidth, windowHeight);
			}
			else {
				glutFullScreen();
				glutReshapeWindow(fullWindowWidth, fullWindowHeight);
				fullScreen = true;
			}
			break;
			
		case 'L':
			currentDrawFunc = &App::lissajousDraw;			
			break;
			
		case 'R': 
			currentDrawFunc = &App::ringDraw;
			break;
			
		case 'u': //up
			translateY += 0.1;
			break;
			
		case 'd':
			translateY -= 0.1;
			break;
			
		case 'r':	//right
			translateX += 0.1;
			break;
			
		case 'l':
			translateX -= 0.1;	
			break;
			
		case 'C': //clockwise
			rotateAngle -= 25.0;
			rotateY = 1.0;		
			break;
			
		case 'c': //counterclockwise
			rotateAngle += 25.0;
			rotateY = 1.0;
			break;
			
		case 'Z':
			translateZ += .1;
			break;
			
		case 'z': //zoom out
			translateZ -= .1;
			
		case 's': //switch rings
			//if (switcher == 0) switcher = 1;
			//else switcher = 0;
			break;
		
		case 'h':
			cameraHome();
			
		default:
			break;
	}	
}

void App::myMouseUp( int button, int x, int y) {
	
}

void App::myMouseDown( int button, int x, int y ) {  

}


void App::myMouseDrag( int x, int y ) {

}


void App::myMouseMove( int x, int y ) {
    mouseX = x;
    mouseY = y;
}


void App::myReshape( int width, int height ) {
    int newWindowWidth = width;
    int newWindowHeight = height;
	
	glViewport (0, 0, (GLsizei) newWindowWidth, (GLsizei) newWindowHeight);

	// clear the matrix 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective (80., newWindowWidth/(float)newWindowHeight, .5, 1000.);
}


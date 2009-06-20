/*
 *  glut_basic.cpp
 *  fromScratch
 *
 *  Created by Ivy Feraco on 3/1/08.
*
 */

#include "glut_basic.h"

static App * myApp;

int mouseX, mouseY;
int fullW, fullH, W, H;


int startGLUT( int argc, char** argv, App * imyApp )
{
	myApp = imyApp;
	printf("entering glut startup\n");
    // initialize GLUT properties
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize( fullW, fullH );
    glutCreateWindow( "noesys" );
    glutDisplayFunc( Display );
	
	printf("about to call init()\n");
    glInit();
    printf("successfully called init()\n");
    
	// tell GLUT about our callback functions of myApp
	glutIdleFunc( Idle );
    glutKeyboardFunc( Key );
    glutMouseFunc( MouseDown );
    glutMotionFunc( MouseDrag );
    glutPassiveMotionFunc ( MouseMove );
    glutReshapeFunc( Reshape );
	
	printf("entering glut main loop\n");
    // and we're off...
    glutMainLoop();
    return 0;
}

void glInit(void) {
	fullW = 1280;
	fullH = 1024;
	W = 800;
	H = 600;

    // enable standard OpengGL settings
    glEnable( GL_COLOR_MATERIAL ); 
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glEnable( GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable( GL_TEXTURE_2D );
    glShadeModel( GL_SMOOTH );
    
    myApp->myInit(fullW, fullH, W, H);
}

void setup2DCamera() {
    // set viewing matrices
    glViewport( 0, 0, fullW, fullH );    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D( 0, fullW, 0, fullH );
	// set modeling matrix
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void setup3DCamera() {
    // set viewing matrices
    glViewport( 0, 0, fullW, fullH );
	 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
   // gluPerspective(45.0f, (GLfloat)fullW/(GLfloat)fullH, 0.1f, 1000.0f);
	/*
	 The perspective of an OpenGL camera consists of four elements: fovy, aspect, near and far. 
	 "Fovy" specifies the field of view angle, in degrees, in the y-direction. 
	 "Aspect" specifies the aspect ratio that determines the field of view in the x-direction. 
	 The aspect ratio is the ratio of x (width) to y (height). 
	 "Near" specifies the distance from the viewer to the nearest clipping plane (always positive). 
	 "Far" specifies the disistance from the viewer to the far clipping plane (always positive). 
	 */
    // set modeling matrix this was messing things up before
	
	gluPerspective(45.0f, (GLfloat)fullW/(GLfloat)fullH, 0.1f, 4000.0f);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
}

void Display(void) {
    // set up the camera
    setup3DCamera();
	
    // clear the window
    glClearColor(1, 1, 1, 1);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
    glDisable( GL_DEPTH_TEST );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
    // call our separate draw functions
    Draw();
    
    // draw again, set up next frame
    glutPostRedisplay();
    glutSwapBuffers();
}

void Draw() {
	myApp->myDraw();
}

void Idle() {
	myApp->myIdle();
}

void Key( unsigned char key, int x, int y ) {	
	myApp->myKey(key, x, y);
}

void MouseDown( int button, int state, int x, int y ) {  
	mouseX = x;
    mouseY = fullH - y;
	
	if (state == GLUT_DOWN) myApp->myMouseDown(button, mouseX, mouseY);
	else myApp->myMouseUp(button, mouseX, mouseY);
	
}

void MouseDrag( int x, int y ) {
	mouseX = x;
    mouseY = y;
	
	myApp->myMouseDrag(mouseX, mouseY);
}

void MouseMove( int x, int y ) {
	mouseX = x;
    mouseY = fullH - y;
	myApp->myMouseMove(mouseX, mouseY);
}

void Reshape( int width, int height ) {
    myApp->myReshape(width, height);
}








#include "ofMain.h"
#include "FluidBoard.h"

//========================================================================
int main( ){
	ofSetupOpenGL(768,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new FluidBoard());

}

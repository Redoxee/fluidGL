#ifndef FLUIDBOARD_H
#define FLUIDBOARD_H

#include "ofMain.h"

class FluidBoard: public ofBaseApp{

	public:
        FluidBoard();
        virtual ~FluidBoard();

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        void setUpFBO();

        ofPlanePrimitive planeScreen;

		ofPlanePrimitive plane1;
        ofPlanePrimitive plane2;
        ofPlanePrimitive plane3;
        ofPlanePrimitive plane4;

		ofFbo fbo1;
		ofFbo fbo2;
		ofFbo fbo3;
		ofFbo fbo4;

		ofShader simpleShader;
		ofShader nightValeShader;
		ofShader laplacianShader;
		ofShader processShader;
		ofShader expansionShader;
		ofShader fluidShader;

		unsigned char *liquidGrid;
		ofImage liquidImage;
        //ofTexture liquidTex;

};
#endif // FLUIDBOARD_H

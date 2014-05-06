#include "FluidBoard.h"
#define FLUID_WIDTH ofGetWidth()
#define FLUID_HEIGHT ofGetHeight()
#define POS(x,y,c) (y * FLUID_WIDTH + x) * 3 + c
#define NB_MOD 2
FluidBoard::FluidBoard()
{
    liquidGrid = new unsigned char[FLUID_WIDTH * FLUID_HEIGHT * 3];
}

FluidBoard::~FluidBoard()
{
    delete liquidGrid;
}


bool isNotFirst;
int DisplayMode;
//--------------------------------------------------------------
void FluidBoard::setup(){
    std::cout << ofGetWidth() << " : " << ofGetHeight() << std::endl;
    for(int x = 0 ; x < FLUID_WIDTH ; ++ x)
    {
        for(int y = 0 ; y < FLUID_HEIGHT ; ++ y)
        {
            liquidGrid[POS(x,y,0)] = 0;
            liquidGrid[POS(x,y,1)] = 0;
            liquidGrid[POS(x,y,2)] = 0;
        }
    }

     for(int x = 55 ; x < 200 ; ++ x)
    {
        for(int y = 55 ; y < 200 ; ++ y)
        {
            liquidGrid[POS(x,y,0)] = 55;
            liquidGrid[POS(x,y,1)] = 50;
            liquidGrid[POS(x,y,2)] = 255;
        }
    }

    liquidImage.setFromPixels(liquidGrid,FLUID_WIDTH,FLUID_HEIGHT,OF_IMAGE_COLOR,true);

    plane1.set(ofGetWidth()/2, ofGetHeight()/2);
    plane1.setPosition(ofGetWidth()/4, ofGetHeight()/4,0);

    plane2.set(ofGetWidth()/2, ofGetHeight()/2);
    plane2.setPosition(3*ofGetWidth()/4, ofGetHeight()/4,0);

    plane3.set(ofGetWidth()/2, ofGetHeight()/2);
    plane3.setPosition(ofGetWidth()/4, 3*ofGetHeight()/4,0);

    plane4.set(ofGetWidth()/2, ofGetHeight()/2);
    plane4.setPosition(3 * ofGetWidth()/4, 3 * ofGetHeight()/4,0);

    planeScreen.set(ofGetWidth(),ofGetHeight());
    planeScreen.setPosition(ofGetWidth()/2, ofGetHeight()/2,0);

    simpleShader.load("shader/shader.vert","shader/simple.frag");
    nightValeShader.load("shader/shader.vert","shader/nightvale.frag");
    laplacianShader.load("shader/shader.vert","shader/laplacian5.frag");
    processShader.load("shader/shader.vert","shader/simpleProcesse.frag");
    expansionShader.load("shader/shader.vert","shader/expansion.frag");
    fluidShader.load("shader/shader.vert","shader/simple.frag");


    decalShader.load("shader/shader.vert","shader/testDecal.frag");
    setUpFBO();
    isNotFirst = false;
    DisplayMode = 0;

}

//--------------------------------------------------------------
void FluidBoard::setUpFBO(){
    ofFbo::Settings settings;
    settings.width = ofGetWidth();
    settings.height = ofGetHeight();
//    settings.useDepth = true;
//    settings.useStencil = true;
//    settings.depthStencilAsTexture = true;
    fbo1.allocate(settings);

    settings = ofFbo::Settings();
    settings.width = ofGetWidth();
    settings.height = ofGetHeight();
    fbo2.allocate(settings);

    settings = ofFbo::Settings();
    settings.width = ofGetWidth();
    settings.height = ofGetHeight();
    fbo3.allocate(settings);

    settings = ofFbo::Settings();
    settings.width = ofGetWidth();
    settings.height = ofGetHeight();
    fbo4.allocate(settings);

    settings = ofFbo::Settings();
    settings.width = FLUID_WIDTH;
    settings.height = FLUID_HEIGHT;
    fboFluid.allocate(settings);
}


//--------------------------------------------------------------
void FluidBoard::update(){

}
//--------------------------------------------------------------

void FluidBoard::drawPipeLineNightVale()
{
    fbo1.begin();
    nightValeShader.begin();
    nightValeShader.setUniform2f("iResolution",ofGetWidth(),ofGetHeight());
    nightValeShader.setUniform1f("timeElapsed",ofGetElapsedTimef());
    planeScreen.draw();
    nightValeShader.end();
    fbo1.end();

    fbo2.begin();
    laplacianShader.begin();
    laplacianShader.setUniformTexture("texture",fbo1.getTextureReference(0),0);
    laplacianShader.setUniform2f("iResolution",ofGetWidth(),ofGetHeight());
    laplacianShader.setUniform1f("timeElapsed",ofGetElapsedTimef());
    planeScreen.draw();
    laplacianShader.end();
    fbo2.end();

    fbo3.begin();
    expansionShader.begin();
    expansionShader.setUniformTexture("texture",fbo2.getTextureReference(0),0);
    expansionShader.setUniform2f("iResolution",ofGetWidth(),ofGetHeight());
    planeScreen.draw();
    expansionShader.end();
    fbo3.end();

    fbo4.begin();
    processShader.begin();
    processShader.setUniformTexture("texture",fbo3.getTextureReference(0),0);
    processShader.setUniform2f("iResolution",ofGetWidth(),ofGetHeight());
    planeScreen.draw();
    processShader.end();
    fbo4.end();

    simpleShader.begin();
    simpleShader.setUniformTexture("texture",fbo1.getTextureReference(0),0);
    simpleShader.setUniform2f("iResolution",ofGetWidth(),ofGetHeight());
    plane1.draw();
    simpleShader.end();

    simpleShader.begin();
    simpleShader.setUniformTexture("texture",fbo2.getTextureReference(0),0);
    simpleShader.setUniform2f("iResolution",ofGetWidth(),ofGetHeight());
    plane2.draw();
    simpleShader.end();

    simpleShader.begin();
    simpleShader.setUniformTexture("texture",fbo3.getTextureReference(0),0);
    simpleShader.setUniform2f("iResolution",ofGetWidth(),ofGetHeight());
    plane3.draw();
    simpleShader.end();

    simpleShader.begin();
    simpleShader.setUniformTexture("texture",fbo4.getTextureReference(0),0);
    simpleShader.setUniform2f("iResolution",ofGetWidth(),ofGetHeight());
    plane4.draw();
    simpleShader.end();
}
void FluidBoard::drawFluidPipeLine()
{
    fboFluid.begin();
    simpleShader.begin();
    processShader.setUniformTexture("texture",liquidImage.getTextureReference(),0);
    simpleShader.setUniform2f("iResolution",FLUID_WIDTH,FLUID_HEIGHT);
    planeScreen.draw();
    simpleShader.end();
    fboFluid.end();

    if(isNotFirst){
        fbo2.begin();
        decalShader.begin();
        decalShader.setUniformTexture("texture",fbo2.getTextureReference(0),0);
        decalShader.setUniform2f("iResolution",ofGetWidth(),ofGetHeight());
        decalShader.setUniform1f("timeElapsed",ofGetElapsedTimef());
        planeScreen.draw();
        decalShader.end();
            fbo2.end();
    }else{
        isNotFirst = true;
        fbo2.begin();
        decalShader.begin();
        decalShader.setUniformTexture("texture",fboFluid.getTextureReference(0),0);
        decalShader.setUniform2f("iResolution",ofGetWidth(),ofGetHeight());
        decalShader.setUniform1f("timeElapsed",ofGetElapsedTimef());
        planeScreen.draw();
        decalShader.end();
        fbo2.end();
    }

    simpleShader.begin();
    simpleShader.setUniformTexture("texture",fboFluid.getTextureReference(0),0);
    simpleShader.setUniform2f("iResolution",ofGetWidth(),ofGetHeight());
    plane1.draw();
    simpleShader.end();

    simpleShader.begin();
    simpleShader.setUniformTexture("texture",fbo2.getTextureReference(0),0);
    simpleShader.setUniform2f("iResolution",ofGetWidth(),ofGetHeight());
    plane2.draw();
    simpleShader.end();
}
void FluidBoard::draw(){

   switch (DisplayMode){
   case 0 :
    drawFluidPipeLine();
    break;
   case 1 :
    drawPipeLineNightVale();
    break;
   }
}



//--------------------------------------------------------------
void FluidBoard::keyPressed(int key){
 if( key == OF_KEY_ESC ){
        simpleShader.unload();
        nightValeShader.unload();
    }
    switch (key) {
    case 'r':
        isNotFirst = false;
        break;
    case OF_KEY_UP :
        DisplayMode += 1;
        DisplayMode %= NB_MOD;
        break;
    case OF_KEY_DOWN :
        DisplayMode -= 1;
        DisplayMode %= NB_MOD;
        break;
    }
}

//--------------------------------------------------------------
void FluidBoard::keyReleased(int key){

}

//--------------------------------------------------------------
void FluidBoard::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void FluidBoard::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void FluidBoard::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void FluidBoard::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void FluidBoard::windowResized(int w, int h){

}

//--------------------------------------------------------------
void FluidBoard::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void FluidBoard::dragEvent(ofDragInfo dragInfo){

}

#include "FluidBoard.h"
#define FLUID_WIDTH ofGetWidth()
#define FLUID_HEIGHT ofGetHeight()
#define POS(x,y,c) (y * FLUID_WIDTH + x) * 3 + c
#define NB_MOD 2
#define NB_SOURCE 2
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
int DisplayImage;
float deltaTime;
float prevTime;

float visc,diff,force,source;
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
            liquidGrid[POS(x,y,0)] = 0;
            liquidGrid[POS(x,y,1)] = 0;
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

    decalShader.load("shader/shader.vert","shader/testDecal.frag");
    fluidShader.load("shader/shader.vert","shader/fluidProcess.frag");

    DiffuseDensityShader.load("shader/shader.vert","shader/diffuseDensity.frag");
//    AddVecDensityShader.load("shader/shader.vert","shader/addVecDensity.frag");

    setUpFBO();
    isNotFirst = false;
    DisplayMode = 0;
    DisplayImage = 0;


    deltaTime = 0.1f;
    diff = 1.f;
    visc = 0.0000001f;
    force = 2.0f;
    source = 300.0f;

    prevTime = ofGetElapsedTimef();

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
    float curTime = ofGetElapsedTimef();
    deltaTime = curTime - prevTime;
    prevTime = curTime;

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
/*
void FluidBoard::ComputFluid()
{
        fbo2.begin();
        fluidShader.begin();
        fluidShader.setUniformTexture("texture",fbo2.getTextureReference(0),0);
        fluidShader.setUniform2f("iResolution",ofGetWidth(),ofGetHeight());
        fluidShader.setUniform1f("timeElapsed",ofGetElapsedTimef());
        fluidShader.setUniform1f("uDeltaTime",deltaTime);
        fluidShader.setUniform1f("difusionRate",diff);
        planeScreen.draw();
        fluidShader.end();
        fbo2.end();
}
*/

void FluidBoard::ComputFluid()
{

    for(unsigned int k = 0; k < 1 ; ++k)
    {
        fbo2.begin();
        DiffuseDensityShader.begin();

        DiffuseDensityShader.setUniformTexture("texture",fbo2.getTextureReference(0),0);
        DiffuseDensityShader.setUniform2f("iResolution",ofGetWidth(),ofGetHeight());
        DiffuseDensityShader.setUniform1f("uDeltaTime",deltaTime);
        DiffuseDensityShader.setUniform1f("difusionRate",diff);

        planeScreen.draw();

        DiffuseDensityShader.end();
        fbo2.end();
    }
}

void FluidBoard::drawFluidPipeLine()
{

    switch (DisplayImage){
    case 0:

        fboFluid.begin();
        simpleShader.begin();
        simpleShader.setUniformTexture("texture",liquidImage.getTextureReference(),0);
        simpleShader.setUniform2f("iResolution",FLUID_WIDTH,FLUID_HEIGHT);
        planeScreen.draw();
        simpleShader.end();
        fboFluid.end();
        break;
    case 1:

        fboFluid.begin();
        nightValeShader.begin();
        nightValeShader.setUniformTexture("texture",liquidImage.getTextureReference(),0);
        nightValeShader.setUniform2f("iResolution",FLUID_WIDTH,FLUID_HEIGHT);
        planeScreen.draw();
        nightValeShader.end();
        fboFluid.end();
        break;
    }

    if(isNotFirst){
        ComputFluid();
    }else{
        isNotFirst = true;
        fbo2.begin();
        simpleShader.begin();
        simpleShader.setUniformTexture("texture",fboFluid.getTextureReference(0),0);
        simpleShader.setUniform2f("iResolution",ofGetWidth(),ofGetHeight());
        simpleShader.setUniform1f("timeElapsed",ofGetElapsedTimef());
        planeScreen.draw();
        simpleShader.end();
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
        DisplayMode += DisplayMode < 0 ? NB_MOD:0;
        break;
    case OF_KEY_LEFT:
        DisplayImage -= 1;
        DisplayImage += DisplayImage < 0 ? NB_SOURCE:0;
        break;
    case OF_KEY_RIGHT:
        DisplayImage += 1;
        DisplayImage %=NB_SOURCE;
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

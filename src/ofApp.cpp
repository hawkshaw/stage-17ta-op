#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    timeLine.setup();
    
    //ofEnableSmoothing();
    ofEnableAlphaBlending();
    ofBackground(0);
    ofSetCircleResolution(100);
    
    //ofSetFullscreen(true);

    setupFireFluid();
    setupFireParticle();
    setupTriangleBoal();
    setupFlowTools();

    /*    sound    */
    sound.loadSound("spiral.mp3");
    sound.setLoop( true );
    sound.play();
    spectrum_ave_pre = 0;
    
    /*   Rolling Cam   */
    rollCam.setup();//rollCam's setup.
    rollCam.setCamSpeed(0.1);//rollCam's speed set;

    // MOUSE DRAW
    mouseForces.setup(flowWidth, flowHeight, drawWidth, drawHeight);
    
    // CAMERA
    simpleCam.setup(640, 480, true);
    didCamUpdate = false;
    cameraFbo.allocate(640, 480);
    cameraFbo.black();
    
    // GUI
    setupGui();
    b_DrawGui = true;
    lastTime = ofGetElapsedTimef();
    
    //TitleShake
    b_TitleShake = false;
    imgTitleShake.load("titleshake.png");
    
    //cam.setPosition(0, -100, 100);
    cam.setDistance(420);
    
    
    //Fbo
    myFbo.allocate( ofGetWidth(), ofGetHeight(),GL_RGBA);
    myGlitch.setup(&myFbo);
    
    
    /*String AVS*/
    setupAVSName();

}


//--------------------------------------------------------------
void ofApp::update(){
    timeLine.update(sound.getPosition());
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));

    if(pb_DrawDelaunay)triangulation.triangulate();
    if(pb_DrawFireFluid)updateFireFluid();
    if(pb_DrawFireParticles)updateFireParticle();
    if(pb_DrawFireFlow)updateFlowTools();
    updateTriangleBoal();

    rollCam.update();   //rollCam's rotate update.
    
    updateAVSName();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(b_TitleShake){
        myFbo.begin();
    }
    ofBackground(0);
    
    ofPushStyle();
    ofPushMatrix();
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    
    cam.begin();
    rollCam.begin(); //rollCam begin
    
    if(pb_DrawFireFluid)drawFireFluid();
    if(pb_DrawBoal)drawTriangleBoal();
    if(pb_DrawFireParticles)drawFireParticle();
    
    drawAVSName();
    
    rollCam.end();  //rollCam end
    cam.end();
    
    ofPopMatrix();
    ofPopStyle();
    
    if(pb_DrawFireFlow)drawFlowTools();
    
    if(pb_DrawDelaunay){
        ofPushStyle();
        ofNoFill();
        triangulation.draw();
        ofPopStyle();
    }

    if(b_TitleShake){
        ofEnableAlphaBlending();
        imgTitleShake.draw(30*ofRandom(-1.0,1.0), 30*ofRandom(-1.0,1.0), ofGetWidth(), ofGetHeight());
        //imgTitleShake.draw(50*ofRandom(-1.0,1.0), 50*ofRandom(-1.0,1.0), ofGetWidth(), ofGetHeight());
        imgTitleShake.draw((ofRandom(-1.0,1.0)-2)*ofGetWidth(), (ofRandom(-1.0,1.0)-2)*ofGetHeight(), ofGetWidth()*5, ofGetHeight()*5);
    }
    
    if(b_TitleShake){
        myFbo.end();
        myGlitch.generateFx();
        myFbo.draw(0, 0);
    }

    ofPushStyle();
    if(b_DrawGui){
        drawGui();
        ofSetColor(255);
        ofDrawBitmapString(cam.getDistance(), 600, 300);
        ofDrawBitmapString(sound.getPosition(), 600, 350);
        ofShowCursor();
    }else{
        ofHideCursor();
    }
    ofPopStyle();

    timeLine.draw();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case 'f':
            ofToggleFullscreen();
            myFbo.allocate( ofGetWidth(), ofGetHeight(),GL_RGBA);
            myGlitch.setup(&myFbo);
            break;
        case 'd':
            b_DrawGui = !b_DrawGui;
            break;
    }
    if (key=='1') {//All Random.
        rollCam.setRandomScale(0.5, 1.5);
        rollCam.setRandomPos(270);
    }
    if (key=='2') {//Random rotate.
        rollCam.setRandomPos(270);
    }
    if (key=='3') {//Inputting optional rotate.
        rollCam.setPos(0, 0, 0);
    }
    if (key=='4') {//Random distance.
        rollCam.setRandomScale(0.5, 1.5);
    }
    if (key=='5') {//Inputting optional distance.
        rollCam.setScale(1);
    }
    if (key == '1') myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE	, ! myGlitch.getFx(OFXPOSTGLITCH_CONVERGENCE));
    if (key == '2') myGlitch.setFx(OFXPOSTGLITCH_GLOW			, ! myGlitch.getFx(OFXPOSTGLITCH_GLOW));
    if (key == '3') myGlitch.setFx(OFXPOSTGLITCH_SHAKER			, ! myGlitch.getFx(OFXPOSTGLITCH_SHAKER));
    if (key == '4') myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER		, ! myGlitch.getFx(OFXPOSTGLITCH_CUTSLIDER));
    if (key == '5') myGlitch.setFx(OFXPOSTGLITCH_TWIST			, ! myGlitch.getFx(OFXPOSTGLITCH_TWIST));
    if (key == '6') myGlitch.setFx(OFXPOSTGLITCH_OUTLINE		, ! myGlitch.getFx(OFXPOSTGLITCH_OUTLINE));
    if (key == '7') myGlitch.setFx(OFXPOSTGLITCH_NOISE			, ! myGlitch.getFx(OFXPOSTGLITCH_NOISE));
    if (key == '8') myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN		, ! myGlitch.getFx(OFXPOSTGLITCH_SLITSCAN));
    if (key == '9') myGlitch.setFx(OFXPOSTGLITCH_SWELL			, ! myGlitch.getFx(OFXPOSTGLITCH_SWELL));
    if (key == '0') myGlitch.setFx(OFXPOSTGLITCH_INVERT			, ! myGlitch.getFx(OFXPOSTGLITCH_INVERT));
    
    if (key == 'q') avs.play(0, 400, 400);

    if (key == 'x'){
        rollCam.setPos(ofRandom(-60,60),0,0);
    }
    if (key == 'y'){
        rollCam.setPos(0,ofRandom(-180,180),0);
    }
    if (key == 'z'){
        rollCam.setPos(0,0,ofRandom(-45,45));
    }

    if(key == OF_KEY_RETURN){
        b_TitleShake=true;
    }
    if(key == OF_KEY_UP){
        v_avs[i_AvsId].play(0,400,400);
        i_AvsId = MIN(i_AvsId+1,v_avs.size()-1);
    }
    if(key == OF_KEY_DOWN){
        v_avs[i_AvsId].play(0,400,400);
        i_AvsId = MAX(i_AvsId-1,0);
    }
    if(key == 'r'){
        sound.setPosition(0);
    }
    if(key == 's'){
        sound.setPaused(true);
    }
    if(key == 'p'){
        sound.setPaused(false);
    }
    if(key == 'l'){
        sound.setPosition(0.24);
        sound.setSpeed(testParam2/128.0);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == OF_KEY_RETURN){
        b_TitleShake=false;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    triangulation.addPoint(ofPoint(x,y));
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::drawComposite(int _x, int _y, int _width, int _height) {
    ofPushStyle();
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    fluidSimulation.draw(_x, _y, _width, _height);
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    if (particleFlow.isActive())
        particleFlow.draw(_x, _y, _width, _height);
    
    if (showLogo) {
        flowToolsLogoImage.draw(_x, _y, _width, _height);
    }
    
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawGui() {
    guiFPS = (int)(ofGetFrameRate() + 0.5);
    
    // calculate minimum fps
    deltaTimeDeque.push_back(deltaTime);
    
    while (deltaTimeDeque.size() > guiFPS.get())
        deltaTimeDeque.pop_front();
    
    float longestTime = 0;
    for (int i=0; i<deltaTimeDeque.size(); i++){
        if (deltaTimeDeque[i] > longestTime)
            longestTime = deltaTimeDeque[i];
    }
    
    guiMinFPS.set(1.0 / longestTime);
    
    
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    gui.draw();
    
    // HACK TO COMPENSATE FOR DISSAPEARING MOUSE
    ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
    ofDrawCircle(ofGetMouseX(), ofGetMouseY(), ofGetWindowWidth() / 300.0);
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofDrawCircle(ofGetMouseX(), ofGetMouseY(), ofGetWindowWidth() / 600.0);
    ofPopStyle();
}


//--------------------------------------------------------------
void ofApp::setupGui() {
    
    gui.setup("settings");
    gui.setDefaultBackgroundColor(ofColor(0, 0, 0, 127));
    gui.setDefaultFillColor(ofColor(160, 160, 160, 160));
    gui.add(guiFPS.set("average FPS", 0, 0, 60));
    gui.add(guiMinFPS.set("minimum FPS", 0, 0, 60));

    gui.add(pb_DrawFireFlow.set("DrawFireFlow", false));
    gui.add(pb_DrawFireFluid.set("DrawFireFluid", true));
    gui.add(pb_DrawFireParticles.set("DrawFireParticles", false));
    gui.add(pb_DrawDelaunay.set("DrawDelaunay", false));
    gui.add(pb_DrawBoal.set("DrawBoal", false));

    
    gui.add(doFullScreen.set("fullscreen (F)", false));
    doFullScreen.addListener(this, &ofApp::setFullScreen);
    gui.add(toggleGuiDraw.set("show gui (G)", false));
    gui.add(doFlipCamera.set("flip camera", true));
    gui.add(doDrawCamBackground.set("draw camera (C)", true));
    gui.add(drawName.set("MODE", "draw name"));
    
    
    int guiColorSwitch = 0;
    ofColor guiHeaderColor[2];
    guiHeaderColor[0].set(160, 160, 80, 200);
    guiHeaderColor[1].set(80, 160, 160, 200);
    ofColor guiFillColor[2];
    guiFillColor[0].set(160, 160, 80, 200);
    guiFillColor[1].set(80, 160, 160, 200);
    
    gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    gui.add(opticalFlow.parameters);
    
    gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    gui.add(velocityMask.parameters);
    
    gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    gui.add(fluidSimulation.parameters);
    
    gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    gui.add(particleFlow.parameters);
    
    gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    gui.add(mouseForces.leftButtonParameters);
    
    gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    gui.add(mouseForces.rightButtonParameters);
    
    visualizeParameters.setName("visualizers");
    visualizeParameters.add(showScalar.set("show scalar", true));
    visualizeParameters.add(showField.set("show field", true));
    visualizeParameters.add(displayScalarScale.set("scalar scale", 0.15, 0.05, 1.0));
    displayScalarScale.addListener(this, &ofApp::setDisplayScalarScale);
    visualizeParameters.add(velocityFieldScale.set("velocity scale", 0.1, 0.0, 0.5));
    velocityFieldScale.addListener(this, &ofApp::setVelocityFieldScale);
    visualizeParameters.add(temperatureFieldScale.set("temperature scale", 0.1, 0.0, 0.5));
    temperatureFieldScale.addListener(this, &ofApp::setTemperatureFieldScale);
    visualizeParameters.add(pressureFieldScale.set("pressure scale", 0.02, 0.0, 0.5));
    pressureFieldScale.addListener(this, &ofApp::setPressureFieldScale);
    visualizeParameters.add(velocityLineSmooth.set("line smooth", false));
    velocityLineSmooth.addListener(this, &ofApp::setVelocityLineSmooth);
    
    testParameters.setName("test param");
    testParameters.add(testParam1.set("testParam1", 1, 0, 255));
    testParameters.add(testParam2.set("testParam2", 128, 0, 255));
    gui.add(testParameters);
    
    gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    gui.add(visualizeParameters);
    
    gui.setDefaultHeaderBackgroundColor(guiHeaderColor[guiColorSwitch]);
    gui.setDefaultFillColor(guiFillColor[guiColorSwitch]);
    guiColorSwitch = 1 - guiColorSwitch;
    gui.add(velocityDots.parameters);
    
    // if the settings file is not present the parameters will not be set during this setup
    if (!ofFile("settings.xml"))
        gui.saveToFile("settings.xml");
    
    gui.loadFromFile("settings.xml");
    
    gui.minimizeAll();
    toggleGuiDraw = true;
    
}

//--------------------------------------------------------------
void ofApp::drawSource(int _x, int _y, int _width, int _height) {
    ofPushStyle();
    ofEnableBlendMode(OF_BLENDMODE_DISABLED);
    cameraFbo.draw(_x, _y, _width, _height);
    ofPopStyle();
}




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
    rollCam.setCamSpeed(0.05);//rollCam's speed set;

    
    // GUI
    setupGui();
    b_DrawGui = true;
    lastTime = ofGetElapsedTimef();
    
    //TitleShake
    b_TitleShake = false;
    b_TitleShakeManual = false;
    imgTitleShake.load("titleshake.png");
    
    //cam.setPosition(0, -100, 100);
    cam.setDistance(420);
    
    
    //Fbo
    myFbo.allocate( ofGetWidth(), ofGetHeight(),GL_RGBA);
    myGlitch.setup(&myFbo);
    
    //Fbo Avs
    myFboAvs.allocate(1200, 150,GL_RGBA);
    myGlitchAvs.setup(&myFboAvs);
    myGlitchAvs.setFx(OFXPOSTGLITCH_GLOW , true);

    /*String AVS*/
    setupAVSName();
    
    b_soundCountKey=false;
    i_FadeOutAlpha=0;
    b_TimeLineManual = false;
}


//--------------------------------------------------------------
void ofApp::update(){
    if(!b_TimeLineManual)timeLine.update(sound.getPosition());
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));

    if(pb_DrawDelaunay)triangulation.triangulate();
    if(pb_DrawFireFluid)updateFireFluid();
    if(pb_DrawFireParticles)updateFireParticle();
    if(pb_DrawFireFlow)updateFlowTools();
    updateTriangleBoal();

    rollCam.update();   //rollCam's rotate update.
    
    
    /*time line*/
    if(timeLine.getState(AID_INIT)){
        pb_DrawFireFlow=false;
        pb_DrawFireFluid=false;
        pb_DrawFireParticles=false;
        pb_DrawDelaunay=false;
        pb_DrawBoal=false;
        i_FadeOutAlpha=0;
        rollCam.setPos(0, 0, 0);
        b_DrawAVS=true;
    }
    if(timeLine.getState(AID_AVS))updateAVSName();
    if(timeLine.getState(AID_FLOW_TOOLS)){
        switch(timeLine.getParam(AID_FLOW_TOOLS)){
            case 0:
                pb_DrawFireFlow=true;
                simpleMovie.play();
                break;
            case 1:
                pb_DrawFireFlow=false;
                break;
        }
    }
    if(timeLine.getState(AID_FIRE_PARTICLE)){
        switch(timeLine.getParam(AID_FIRE_PARTICLE)){
            case 0:
                pb_DrawFireParticles=true;
                i_FireParticleScale = 10;
                break;
            case 1:
                i_FireParticleScale = 1.0;
                break;
        }
    }
    if(timeLine.getState(AID_TRI_BOAL)){
        switch(timeLine.getParam(AID_TRI_BOAL)){
            case 0:
                pb_DrawBoal=true;
                plusTriangleBoal();
                break;
            case 1:
                minusTriangleBoal();
                break;
        }
    }
    if(timeLine.getState(AID_FIRE_FLUID)){
        switch(timeLine.getParam(AID_FIRE_FLUID)){
            case 0:
                pb_DrawFireFluid=true;
                break;
            case 1:
                pb_DrawFireFluid=false;
                break;
        }
    }
    if(timeLine.getState(AID_FLASH)){
        b_FlashActivate=true;
        if(timeLine.getParam(AID_FLASH)==1){
            b_DrawAVS=false;
            pb_DrawFireParticles=false;
        }
    }
    if(timeLine.getState(AID_FADE_OUT))i_FadeOutAlpha = min(255,i_FadeOutAlpha+2);
    if(timeLine.getState(AID_SHAKE)){
        switch(timeLine.getParam(AID_SHAKE)){
            case 0:
                break;
            case 1:
                myGlitch.setFx(OFXPOSTGLITCH_GLOW	, true);
                break;
        }
        b_TitleShake = true;
    }else{
        b_TitleShake = false;
    }
    if(timeLine.getState(AID_CAM)){
        cam.setPosition(0, 0, 0);
        cam.setDistance(40);
        rollCam.setDirectPos(0,0,0);
        pb_DrawFireParticles=true;
    }
    /*
    if(timeLine.getState(AID_TRI_BOAL)){
        plusTriangleBoal();
    }else{
        minusTriangleBoal();
    }*/
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(b_TitleShake || b_TitleShakeManual){
        myFbo.begin();
    }
    ofBackground(0);
    
    ofPushStyle();
    ofPushMatrix();
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    
    cam.begin();
    rollCam.begin(); //rollCam begin

    if(b_DrawAVS)drawAVSName();
    if(pb_DrawFireFluid)drawFireFluid();
    if(pb_DrawBoal)drawTriangleBoal();
    if(pb_DrawFireParticles)drawFireParticle();

    
    
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

    if(b_TitleShake || b_TitleShakeManual){
        ofEnableAlphaBlending();
        //imgTitleShake.draw(50*ofRandom(-1.0,1.0), 50*ofRandom(-1.0,1.0), ofGetWidth(), ofGetHeight());
        imgTitleShake.draw((ofRandom(-1.0,1.0)-2)*ofGetWidth(), (ofRandom(-1.0,1.0)-2)*ofGetHeight(), ofGetWidth()*5, ofGetHeight()*5);
        imgTitleShake.draw(20*ofRandom(-1.0,1.0), 20*ofRandom(-1.0,1.0), ofGetWidth(), ofGetHeight());
    }
    
    if(b_TitleShake || b_TitleShakeManual){
        myFbo.end();
        myGlitch.generateFx();
        myFbo.draw(0, 0);
    }

    ofPushStyle();
    if(b_DrawGui){
        drawGui();
        ofSetColor(255);
        //ofDrawBitmapString(cam.getDistance(), 600, 300);
        //ofDrawBitmapString(sound.getPosition(), 600, 350);
        ofShowCursor();
    }else{
        ofHideCursor();
    }
    ofPopStyle();

    
    if(b_FlashActivate){
        if(i_FlashCount==0){
            i_FlashCount = 255;
        }
        if(i_FlashCount<0){
            i_FlashCount = 0;
            b_FlashActivate = false;
        }
        if(i_FlashCount>0){
            ofPushStyle();
            ofEnableAlphaBlending();
            ofSetColor(255,255,255,i_FlashCount);
            i_FlashCount -= 10;
            ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
            ofPopStyle();
        }
    }

    if(i_FadeOutAlpha){
        ofPushStyle();
        ofEnableAlphaBlending();
        ofSetColor(0,0,0,i_FadeOutAlpha);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        ofPopStyle();
    }
    
    
    if(b_DrawGui){
        //timeLine.draw();
        //ofDrawBitmapString(ofToString(i_TriBoalAppearCount), 800, 600);
        //ofDrawBitmapString(ofToString(i_TriBoalAppearCountBuf), 800, 640);
    }
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
        case 't':
            if(!b_soundCountKey)cout << "press " << sound.getPosition() << " ";
            b_soundCountKey=true;
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
        b_TitleShakeManual=true;
    }
    if(key == OF_KEY_UP){
        plusTriangleBoal();
    }
    if(key == OF_KEY_DOWN){
        minusTriangleBoal();
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
    if(key == 'u'){
        //b_FlowToolsBarActivate = true;
        simpleMovie.play();
    }
    if(key == 'k'){
        b_FlashActivate=true;
    }
    if(key == 'i'){
        //sabi act
        cam.setPosition(0, 0, 0);
        cam.setDistance(40);
        rollCam.setDirectPos(0,0,0);
    }
    if(key == 'q'){
        i_FadeOutAlpha = min(255,i_FadeOutAlpha+1);
    }
    if(key == 'a'){
        i_FadeOutAlpha = max(0,i_FadeOutAlpha-1);
    }
    if(key == 'm'){
        b_TimeLineManual = !b_TimeLineManual;
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == OF_KEY_RETURN){
        b_TitleShakeManual=false;
    }
    switch(key){
        case 't':
            cout << "release " << sound.getPosition() << endl;
            b_soundCountKey=false;
            break;
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
    testParameters.add(testParam1.set("testParam1", 128, 0, 255));
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




#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //ofEnableSmoothing();
    
    ofEnableAlphaBlending();
    ofBackground(0);

    ofSetCircleResolution(100);
    
    //ofSetFullscreen(true);
    
    
//Fire Fluid
    width = ofGetWidth()/4;
    height = ofGetHeight();
    
    // Initial Allocation
    fluid.allocate(width, height, 0.5);
    //fluid.allocate(width, height, GL_RGBA);
    
    // Seting the gravity set up & injecting the background image
    fluid.dissipation = 0.99;
    fluid.velocityDissipation = 0.99;
    
    fluid.setGravity(ofVec2f(0.0,0.0));
    //fluid.setGravity(ofVec2f(0.0,0.0098));
    
    //  Set obstacle
    fluid.begin();
    ofSetColor(0,0);
    ofSetColor(255);
    
    //ofCircle(width*0.5, height*0.35, 40);
    fluid.end();
    fluid.setUseObstacles(false);
    
    // Adding constant forces
    //fluid.addConstantForce(ofPoint(width*0.5,height*0.85), ofPoint(0,-2), ofFloatColor(0.5,0.1,0.0), 10.f);
    fluid.addConstantForce(ofPoint(width*0.5,height*0.7), ofPoint(0,-2), ofFloatColor(0.5,0.1,0.0), 10.f);
    
    
//Fire Particle
    sys.setup();
    group.setup(sys);
    group.setColor(ofxSPK::RangeC(ofColor(255, 255, 0, 255), ofColor(255, 0, 0, 255)),
                   ofxSPK::RangeC(ofColor(255, 0, 255, 0), ofColor(255, 255, 0, 0)));
    group.setLifeTime(0.5, 20);
    group.setFriction(0.1);
    group.setSize(3);
    group.setGravity(ofVec3f(0, -100, 0));
    group.setMass(0.1, 1);
    group.reserve(10000);
    em.setup(ofxSPK::Emitter::$Random::create(), group);
    em.setForce(10, 50);
    //	em.setZone(SPK::Line::create(toSPK(ofVec3f(100, 0, 0)),
    //								 toSPK(ofVec3f(200, 0, 0))));
    //	em.setZone(SPK::Point::create());
    em.setZone(SPK::Sphere::create(toSPK(ofVec3f(100, 0, 0)), 50));
    
    //	em.setZone(SPK::Ring::create(toSPK(ofVec3f(100, 0, 0)), toSPK(ofVec3f(0, 1, 0)), 40, 160), false);
    //	em.setZone(SPK::Plane::create(toSPK(ofVec3f(100, 0, 0)), toSPK(ofVec3f(0, 1, 0))), true);
    //	em.setZone(SPK::Cylinder::create(toSPK(ofVec3f(100, 0, 0)), toSPK(ofVec3f(0, 1, 0)), 100, 200));
    //	em.setZone(SPK::AABox::create(toSPK(ofVec3f(100, 0, 0)), toSPK(ofVec3f(100, 100, 100))), false);
    
    mod.setup(SPK::Obstacle::create(SPK::Plane::create(toSPK(ofVec3f(0, 0, 0)))), group);
    mod.setPosition(0, -300, 0);
    rot.setup(ofxSPK::Modifier::$PointMass::create(ofxSPK::Zone::$Sphere::create(toSPK(0, 0, 0), 300), SPK::INSIDE_ZONE, 20000, 20), group);
    
    
//triangle boal
    nTri = 1500;			//The number of the triangles
    nVert= nTri * 3;		//The number of the vertices
    
    float Rad = 250;	//The sphere's radius
    float rad = 25;	//Maximal triangle's ìradiusî
    //(formally, it's the maximal coordinates'
    //deviation from the triangle's center)
    
    //Fill the vertices array
    vertices.resize( nVert );		//Set the array size
    for (int i=0; i<nTri; i++) {	//Scan all the triangles
        //Generate the center of the triangle
        //as a random point on the sphere
        
        //Take the random point from
        //cube [-1,1]x[-1,1]x[-1,1]
        ofPoint center( ofRandom( -1, 1 ),
                       ofRandom( -1, 1 ),
                       ofRandom( -1, 1 ) );
        center.normalize(); //Normalize vector's length to 1
        center *= Rad;	//Now the center vector has
        //length Rad
        
        //Generate the triangle's vertices
        //as the center plus random point from
        //[-rad, rad]x[-rad, rad]x[-rad, rad]
        for (int j=0; j<3; j++) {
            vertices[ i*3 + j ] =
            center + ofPoint( ofRandom( -rad, rad ),
                             ofRandom( -rad, rad ),
                             ofRandom( -rad, rad ) );
        }
    }
    
    //Fill the array of triangles' colors
    colors.resize( nTri );
    for (int i=0; i<nTri; i++) {
        //Take a random color from black to red
        colors[i] = ofColor( ofRandom( 0, 255 ), 0, 0 ,ofRandom( 0, 255 ));
    }
    
    
/*    sound    */
    sound.loadSound( "spiral.mp3" );
    sound.setLoop( true );
    sound.play();
    spectrum_ave_pre = 0;
    
/*Rolling Cam*/
    rollCam.setup();//rollCam's setup.
    rollCam.setCamSpeed(0.1);//rollCam's speed set;
    
/*FlowTools*/
    ofSetVerticalSync(false);
    ofSetLogLevel(OF_LOG_NOTICE);
    
    drawWidth = 1280;
    drawHeight = 720;
    // process all but the density on 16th resolution
    flowWidth = drawWidth / 4;
    flowHeight = drawHeight / 4;
    
    // FLOW & MASK
    opticalFlow.setup(flowWidth, flowHeight);
    velocityMask.setup(drawWidth, drawHeight);
    
    // FLUID & PARTICLES
    fluidSimulation.setup(flowWidth, flowHeight, drawWidth, drawHeight);
    particleFlow.setup(flowWidth, flowHeight, drawWidth, drawHeight);
    
    flowToolsLogoImage.load("flowtools.png");
    fluidSimulation.addObstacle(flowToolsLogoImage.getTexture());
    showLogo = false;
    
    velocityDots.setup(flowWidth / 4, flowHeight / 4);
    
    // VISUALIZATION
    displayScalar.setup(flowWidth, flowHeight);
    velocityField.setup(flowWidth / 4, flowHeight / 4);
    temperatureField.setup(flowWidth / 4, flowHeight / 4);
    pressureField.setup(flowWidth / 4, flowHeight / 4);
    velocityTemperatureField.setup(flowWidth / 4, flowHeight / 4);
    
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
    i_AvsId = 0;
    font.loadFont(OF_TTF_SANS, 100);
    avs.setup("YUKI TAKAHASHI");
    ofxAVString avs1,avs2,avs3,avs4,avs5,avs6,avs7,avs8,avs9,avs10,avs11,avs12;
    avs1.setup("YUKI TAKAHASHI");
    avs2.setup("MANA YARITANI");
    avs3.setup("MINAMI BABA");
    avs4.setup("KYOHEI KIKUCHI");
    avs5.setup("HARUKA NAKAI");
    avs6.setup("SHIHO ONOZAWA");
    avs7.setup("YURINA SHIKANO");
    avs8.setup("KOKI HODAMA");
    avs9.setup("MASARU MIZUOCHI");
    avs10.setup("HIROYUKI SHIMA");
    avs11.setup("MAKITO KOBAYASHI");
    avs12.setup("GO NAKATANI");
    v_avs.push_back(avs1);
    v_avs.push_back(avs2);
    v_avs.push_back(avs3);
    v_avs.push_back(avs4);
    v_avs.push_back(avs5);
    v_avs.push_back(avs6);
    v_avs.push_back(avs7);
    v_avs.push_back(avs8);
    v_avs.push_back(avs9);
    v_avs.push_back(avs10);
    v_avs.push_back(avs11);
    v_avs.push_back(avs12);
}


//--------------------------------------------------------------
void ofApp::update(){

//Delaunay
    if(pb_DrawDelaunay){
        triangulation.triangulate();
    }
    
//Fire Fluid
    if(pb_DrawFireFluid){
        //center width*0.5,height*0.7
        ofPoint m = ofPoint(width*0.5+100*(sin(0.01/2*ofGetElapsedTimeMillis()))/2,
                            height*0.7+100*(min(cos(0.02/2*ofGetElapsedTimeMillis()),0.75))/2);
        //ofPoint m = ofPoint(mouseX,mouseY);
        ofPoint d = (m - oldM)*10.0;
        oldM = m;
        ofPoint c = ofPoint(640*0.5, 480*0.5) - m;
        c.normalize();
        fluid.addTemporalForce(m, d, ofFloatColor(c.x,c.y,0.5)*sin(ofGetElapsedTimef()),1.5f);
        //  Update
        //
        fluid.update();
    }
    
//Fire Particle
    if(pb_DrawFireParticles){
        sys->setCameraPosition(toSPK(cam.getPosition()));
        sys.update();
        ofVec3f p;
        p.x = ofSignedNoise(100, 0, 0, ofGetElapsedTimef() * 0.1) * 300;
        p.y = ofSignedNoise(0, 100, 0, ofGetElapsedTimef() * 0.1) * 300;
        p.z = ofSignedNoise(0, 0, 100, ofGetElapsedTimef() * 0.1) * 300;
        
        //	group.emitStatic(10, p);
        //	group.emitRandom(10, p, ofxSPK::RangeF(10, 40));
        //	group.emitStraight(10, p);
        //	group.emitSpheric(10, p, ofxSPK::RangeF(10, 40), p.crossed(ofVec3f(1, 0, 0)), ofxSPK::RangeF(0, 0.2));
        
        em.setPosition(p);
        em.rotate(0.3, ofVec3f(0, 1, 0));
        em.rotate(0.1, ofVec3f(0, 0, 1));
    }
    

    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));

/* sound */
    ofSoundUpdate();
    float *val = ofSoundGetSpectrum( soundN );
    spectrum_ave = 0;
    for ( int i=0; i<soundN; i++ ) {
        spectrum_ave += val[i];
    }
    spectrum_ave /= soundN;
    float smooth = 0.8;
    float smooth2 = 0.7;
    spectrum_ave_edge *= smooth;
    spectrum_ave_edge = max(spectrum_ave_edge , spectrum_ave);//- spectrum_ave_pre);
    spectrum_ave_edge1 *= smooth;
    spectrum_ave_edge1 = spectrum_ave_edge1 * smooth2 + (1-smooth2) * max(spectrum_ave_edge1,spectrum_ave_edge * ofRandom(0.8,1.0));
    spectrum_ave_edge2 *= smooth;
    spectrum_ave_edge2 = spectrum_ave_edge2 * smooth2 + (1-smooth2) * max(spectrum_ave_edge2,spectrum_ave_edge * ofRandom(0.8,1.0));
    spectrum_ave_edge3 *= smooth;
    spectrum_ave_edge3 = spectrum_ave_edge3 * smooth2 + (1-smooth2) * max(spectrum_ave_edge3,spectrum_ave_edge * ofRandom(0.8,1.0));
    spectrum_ave_pre = spectrum_ave;
    //cout << spectrum_ave << endl;
    
/*Rolling Cam*/
    rollCam.update();   //rollCam's rotate update.
    
/*FlowTools*/
    if(pb_DrawFireFlow){
        updateFlowTools();
    }
    
/*AVS*/
    
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
    

//Fire Fluid
    ofPushMatrix();
    {
        ofTranslate(-width/2, -height/2);
        if(pb_DrawFireFluid){
            ofTranslate(ofPoint(0,-height*0.15,-400));
            fluid.draw();
            ofTranslate(ofPoint(400,0,100));
            fluid.draw();
            ofTranslate(ofPoint(-800,0,0));
            fluid.draw();
            ofTranslate(ofPoint(200,0,100));
            fluid.draw();
            ofTranslate(ofPoint(400,0,100));
            fluid.draw();
        }
    }
    ofPopMatrix();

//triangle boal
    if(pb_DrawBoal){
        ofPushStyle();
        ofEnableAlphaBlending();
        ofPushMatrix();						//Store the coordinate system
        //Move the coordinate center to screen's center
        //ofTranslate( ofGetWidth()/2, ofGetHeight()/2, 0 );
        ofTranslate(ofPoint(0,-height*0.15,-400));
        drawTriangleBoal();
        ofTranslate(ofPoint(400,0,100));
        drawTriangleBoal();
        ofTranslate(ofPoint(-800,0,0));
        drawTriangleBoal();
        ofTranslate(ofPoint(200,0,100));
        drawTriangleBoal();
        ofTranslate(ofPoint(400,0,100));
        drawTriangleBoal();
        
        //Calculate the rotation angle
        ofPopMatrix();	//Restore the coordinate system
        ofPopStyle();
    }

    
    
    
/*Fire Particle*/
    ofPushMatrix();
    {
        if(pb_DrawFireParticles){
            em.setFlow(1500);
            //sys.debugDraw();
            sys.draw();
        }
    }
    ofPopMatrix();
    
    
    
//AVS
    {
        ofPushMatrix();
        if(avs.getLastMillis() + 1000 > ofGetElapsedTimeMillis()){
            font.drawString(avs, 0, 0);
        }
        
        for(int i=0;i<v_avs.size();i++){
            ofPushMatrix();
            ofRotateY(i*30);
            ofTranslate(0, 0,300);
            ofScale(0.1,0.1,0.1);
            if(v_avs[i].getLastMillis() + 1000 > ofGetElapsedTimeMillis()){
                font.drawString(v_avs[i], -200, 0);
            }
            ofPopMatrix();
        }
        ofPopMatrix();
    }
    
    
    rollCam.end();  //rollCam end
    cam.end();
    
    ofPopMatrix();
    ofPopStyle();
    

/*FlowToolsTitle*/
    ofPushStyle();
    ofPushMatrix();
    {
        //ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2); //when in cam
        //if(doDrawCamBackground.get())drawSource();
        if(pb_DrawFireFlow){
            ofSetColor(255, 255, 255, testParam1);
            drawComposite();
        }
    }
    ofPopMatrix();
    ofPopStyle();
    


    
    
//triangle
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


void ofApp::drawTriangleBoal(){
    ofPushStyle();
    ofPushMatrix();
    ofScale(0.3, 0.3, 0.3);
    
    float time = ofGetElapsedTimef();	//Get time in seconds
    float angle = time * 10;			//Compute angle. We rotate at speed 10 degrees per second
    ofRotate( angle, 0, 1, 0 );			//Rotate the coordinate system along y-axe
    
    //Draw the triangles
    const float f_triSoundLevel = 60.0;
    for (int i=0; i<nTri; i++) {
        ofSetColor( colors[i] );		//Set color
        ofTriangle(vertices[ i*3     ] * min(2.0,0.8 + ofRandom(0.5,1.0)*spectrum_ave_edge1*f_triSoundLevel),
                   vertices[ i*3 + 1 ] * min(2.0,0.8 + ofRandom(0.5,1.0)*spectrum_ave_edge2*f_triSoundLevel),
                   vertices[ i*3 + 2 ] * min(2.0,0.8 + ofRandom(0.5,1.0)*spectrum_ave_edge3*f_triSoundLevel));		//Draw triangle
    }
    ofPopMatrix();
    ofPopStyle();
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


void ofApp::updateFlowTools(){
    deltaTime = ofGetElapsedTimef() - lastTime;
    lastTime = ofGetElapsedTimef();
    
    simpleCam.update();
    
    if (simpleCam.isFrameNew()) {
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_DISABLED);
        cameraFbo.begin();
        
        if (doFlipCamera)
            simpleCam.draw(cameraFbo.getWidth(), 0, -cameraFbo.getWidth(), cameraFbo.getHeight());  // Flip Horizontal
        else
            simpleCam.draw(0, 0, cameraFbo.getWidth(), cameraFbo.getHeight());
        cameraFbo.end();
        ofPopStyle();
        
        opticalFlow.setSource(cameraFbo.getTexture());
        
        // opticalFlow.update(deltaTime);
        // use internal deltatime instead
        opticalFlow.update();
        
        velocityMask.setDensity(cameraFbo.getTexture());
        velocityMask.setVelocity(opticalFlow.getOpticalFlow());
        velocityMask.update();
    }
    
    
    fluidSimulation.addVelocity(opticalFlow.getOpticalFlowDecay());
    fluidSimulation.addDensity(velocityMask.getColorMask());
    fluidSimulation.addTemperature(velocityMask.getLuminanceMask());
    
    mouseForces.update(deltaTime);
    
    for (int i=0; i<mouseForces.getNumForces(); i++) {
        if (mouseForces.didChange(i)) {
            switch (mouseForces.getType(i)) {
                case FT_DENSITY:
                    fluidSimulation.addDensity(mouseForces.getTextureReference(i), mouseForces.getStrength(i));
                    break;
                case FT_VELOCITY:
                    fluidSimulation.addVelocity(mouseForces.getTextureReference(i), mouseForces.getStrength(i));
                    particleFlow.addFlowVelocity(mouseForces.getTextureReference(i), mouseForces.getStrength(i));
                    break;
                case FT_TEMPERATURE:
                    fluidSimulation.addTemperature(mouseForces.getTextureReference(i), mouseForces.getStrength(i));
                    break;
                case FT_PRESSURE:
                    fluidSimulation.addPressure(mouseForces.getTextureReference(i), mouseForces.getStrength(i));
                    break;
                case FT_OBSTACLE:
                    fluidSimulation.addTempObstacle(mouseForces.getTextureReference(i));
                default:
                    break;
            }
        }
    }
    
    fluidSimulation.update();
    
    if (particleFlow.isActive()) {
        particleFlow.setSpeed(fluidSimulation.getSpeed());
        particleFlow.setCellSize(fluidSimulation.getCellSize());
        particleFlow.addFlowVelocity(opticalFlow.getOpticalFlow());
        particleFlow.addFluidVelocity(fluidSimulation.getVelocity());
        //		particleFlow.addDensity(fluidSimulation.getDensity());
        particleFlow.setObstacle(fluidSimulation.getObstacle());
    }
    particleFlow.update();

}

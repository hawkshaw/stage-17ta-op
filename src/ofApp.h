#pragma once

#include "ofMain.h"
#include "ofxDelaunay.h"
#include "ofxFluid.h"
#include "ofxSPK.h"

#include "ofxGui.h"
#include "ofxFlowTools.h"
#include "ofxRollingCam.h"
#include "ofxPostGlitch.h"
#include "ofxAVString.h"

#include "ofxTimeLine.h"

using namespace flowTools;

enum drawModeEnum{
    DRAW_COMPOSITE = 0,
    DRAW_FLUID_DENSITY,
    DRAW_PARTICLES,
    DRAW_VELDOTS,
    DRAW_FLUID_FIELDS,
    DRAW_FLUID_VELOCITY,
    DRAW_FLUID_PRESSURE,
    DRAW_FLUID_TEMPERATURE,
    DRAW_FLUID_DIVERGENCE,
    DRAW_FLUID_VORTICITY,
    DRAW_FLUID_BUOYANCY,
    DRAW_FLUID_OBSTACLE,
    DRAW_FLOW_MASK,
    DRAW_OPTICAL_FLOW,
    DRAW_SOURCE,
    DRAW_MOUSE
};



const int soundN = 656;		//Number of bands in spectrum

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

    void setupAVSName();
    void updateAVSName();
    void drawAVSName();
    void setupFireFluid();
    void updateFireFluid();
    void drawFireFluid();
    void setupFireParticle();
    void updateFireParticle();
    void drawFireParticle();
    void setupTriangleBoal();
    void updateTriangleBoal();
    void drawTriangleBoal();
    void plusTriangleBoal();
    void minusTriangleBoal();

    void setupFlowTools();
    void updateFlowTools();
    void drawFlowTools();
    void setupTimeLine();
    void updateTimeLine();
    void drawTimeLine();
    
    
        //Dwlaunay
        ofxDelaunay triangulation;
    
/*Fire Fluid*/
        ofxFluid fluid;
        ofVec2f oldM;
        int     width,height;
        bool    bPaint, bObstacle, bBounding, bClear;

/*Fire Particle*/
    ofEasyCam cam;
    
    ofxSPK::System sys;
    ofxSPK::Group group;
    
    ofxSPK::Emitter em;
    ofxSPK::Modifier mod;
    ofxSPK::Modifier rot;
    
    
    /*triangle boal*/
    vector<ofPoint> vertices;
    vector<ofColor> colors;
    int nTri;		//The number of triangles
    int nVert;	//The number of the vertices equals nTri * 3
    
    ofSoundPlayer sound;
    bool b_soundCountKey;
    float spectrum_ave;
    float spectrum_ave_pre;
    float spectrum_ave_edge;
    float spectrum_ave_edge1;
    float spectrum_ave_edge2;
    float spectrum_ave_edge3;
    
    void drawOneTriangleBoal();
    int i_TriBoalAppearCount;
    int i_TriBoalAppearCountBuf;
    
/*Rolling Cam*/
    ofxRollingCam rollCam;    
    
    
/*FlowTools*/
    // Camera
    ofVideoGrabber		simpleCam;
    ofVideoPlayer       simpleMovie;
    bool				didCamUpdate;
    ftFbo				cameraFbo;
    ofParameter<bool>	doFlipCamera;
    
    // Time
    float				lastTime;
    float				deltaTime;
    
    // FlowTools
    int					flowWidth;
    int					flowHeight;
    int					drawWidth;
    int					drawHeight;
    
    ftOpticalFlow		opticalFlow;
    ftVelocityMask		velocityMask;
    ftFluidSimulation	fluidSimulation;
    ftParticleFlow		particleFlow;
    
    ftVelocitySpheres	velocityDots;
    
    ofImage				flowToolsLogoImage;
    bool				showLogo;
    //show bar for opticalflow
    bool                b_FlowToolsBarActivate;
    int                 i_FlowToolsBarCount;
    
    // MouseDraw
    ftDrawMouseForces	mouseForces;
    
    // Visualisations
    ofParameterGroup	visualizeParameters;
    ftDisplayScalar		displayScalar;
    ftVelocityField		velocityField;
    ftTemperatureField	temperatureField;
    ftPressureField		pressureField;
    ftVTField			velocityTemperatureField;
    
    ofParameter<bool>	showScalar;
    ofParameter<bool>	showField;
    ofParameter<float>	displayScalarScale;
    void				setDisplayScalarScale(float& _value) { displayScalar.setScale(_value); }
    ofParameter<float>	velocityFieldScale;
    void				setVelocityFieldScale(float& _value) { velocityField.setVelocityScale(_value); velocityTemperatureField.setVelocityScale(_value); }
    ofParameter<float>	temperatureFieldScale;
    void				setTemperatureFieldScale(float& _value) { temperatureField.setTemperatureScale(_value); velocityTemperatureField.setTemperatureScale(_value); }
    ofParameter<float>	pressureFieldScale;
    void				setPressureFieldScale(float& _value) { pressureField.setPressureScale(_value); }
    ofParameter<bool>	velocityLineSmooth;
    void				setVelocityLineSmooth(bool& _value) { velocityField.setLineSmooth(_value); velocityTemperatureField.setLineSmooth(_value);  }
    
    // GUI
    ofxPanel			gui;
    void				setupGui();
    void				drawGui();
    ofParameter<bool>	toggleGuiDraw;
    ofParameter<float>	guiFPS;
    ofParameter<float>	guiMinFPS;
    deque<float>		deltaTimeDeque;
    ofParameter<bool>	doFullScreen;
    void				setFullScreen(bool& _value) { ofSetFullscreen(_value);}
    
    ofParameterGroup	testParameters;
    ofParameter<int>    testParam1;
    ofParameter<int>    testParam2;
    
    bool                b_DrawGui;
    
    // DRAW
    ofParameter<bool>	doDrawCamBackground;
    
    ofParameter<int>	drawMode;
    void				drawModeSetName(int& _value) ;
    ofParameter<string> drawName;
    
    void				drawComposite()			{ drawComposite(0, 0, ofGetWindowWidth(), ofGetWindowHeight()); }
    void				drawComposite(int _x, int _y, int _width, int _height);

    void				drawSource()			{ drawSource(0, 0, ofGetWindowWidth(), ofGetWindowHeight()); }
    void				drawSource(int _x, int _y, int _width, int _height);
    
    
    
    ofParameter<bool>   pb_DrawFireFlow;
    ofParameter<bool>   pb_DrawFireFluid;
    ofParameter<bool>   pb_DrawFireParticles;
    ofParameter<bool>   pb_DrawDelaunay;
    ofParameter<bool>   pb_DrawBoal;
    
    //Title Shake Logo
    bool                b_TitleShake;
    ofImage             imgTitleShake;
    
    //shake Fbo
    ofFbo               myFbo;
    ofxPostGlitch       myGlitch;
    
    //Name
    ofxAVString avs;
    vector<ofxAVString> v_avs;
    ofTrueTypeFont font;
    int             i_AvsId;
    vector<ofVec3f> vv_AvsPos;//x y
    
    //ofxTimeLine;
    ofxTimeLine timeLine;

    
};

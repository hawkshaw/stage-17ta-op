/*
 *
*/
#pragma once

#ifndef __ofxTimeLine__
#define __ofxTimeLine__

#include "ofMain.h"

#define STEP_NUM 10
#define ACT_ID_NUM 5

enum ACT_ID{
    AID_AVS = 0,
    AID_FLOW_TOOLS,
    AID_FILE_PARTICLE,
    AID_TRI_BOAL,
    AID_FIRE_FLUID,
};

class ofxTimeLine : public ofBaseApp{
public:
    ofxTimeLine();
    void setup();
    void update(float pos);
    void draw();
    void clear();
    bool getState(int actId);
    int getParam(int actId);
    
private:
    vector<ofVec2f> vvf_TimeLineTime;
    vector<int> vi_TimeLineActId;
    vector<int> vi_TimeLineParam;
    vector<bool> vb_TimeLineUsedFlag;
    
    vector<bool> vb_ActState;
    vector<bool> vb_ActOnceFlag;
    vector<int> vb_ActParam;
};

#endif

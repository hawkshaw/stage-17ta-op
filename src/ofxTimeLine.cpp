/*
 *
*/

#include "ofxTimeLine.h"

//--------------------------------------------------------------
ofxTimeLine::ofxTimeLine(){
}
//--------------------------------------------------------------
void ofxTimeLine::setup(){
    //start,end,ACT_ID,Param
    float timeLineData[STEP_NUM][4] = {
        {0.001,0.002,AID_AVS,0},
        {0.003,0.01,AID_TRI_BOAL,0},
        {0.005,0.006,AID_AVS,0},
        {0.03,0.04,AID_TRI_BOAL,0},
        {0.05,0.06,AID_TRI_BOAL,0},
        {0.011,0.012,AID_AVS,0},
        {0.013,0.014,AID_FIRE_FLUID,0},
        {0.015,0.016,AID_FIRE_FLUID,0},
        {0.017,0.018,AID_FIRE_FLUID,0},
        {0.019,0.02,AID_AVS,0},
    };
    bool onceFlag[ACT_ID_NUM] = {true,false,false,false,false};
    for(int i=0;i<STEP_NUM;i++){
        vvf_TimeLineTime.push_back(ofVec2f(timeLineData[i][0],timeLineData[i][1]));
        vi_TimeLineActId.push_back(timeLineData[i][2]);
        vi_TimeLineParam.push_back(timeLineData[i][3]);
        vb_TimeLineUsedFlag.push_back(false);
    }
    for(int i=0;i<ACT_ID_NUM;i++){
        vb_ActState.push_back(false);
        vb_ActOnceFlag.push_back(onceFlag[i]);
    }
}

//--------------------------------------------------------------
void ofxTimeLine::update(float pos){
    for(int i=0;i<STEP_NUM;i++){
        if((vvf_TimeLineTime[i][0] < pos) && (pos < vvf_TimeLineTime[i][1])){
            if(vb_ActOnceFlag[vi_TimeLineActId[i]] && (vb_TimeLineUsedFlag[i])){
            }else{
                vb_ActState[vi_TimeLineActId[i]]=true;
                vb_TimeLineUsedFlag[i]=true;
            }
        }else if (vb_TimeLineUsedFlag[i] && (pos >= vvf_TimeLineTime[i][1])){
            vb_TimeLineUsedFlag[i]=false;
            vb_ActState[vi_TimeLineActId[i]]=false;
        }
    }
}
//--------------------------------------------------------------

void ofxTimeLine::draw(){
    for(int i=0;i<ACT_ID_NUM;i++){
        bool buf = getState(i);
        ofDrawBitmapString(buf, 800, (i+1)*30);
    }
    for(int i=0;i<STEP_NUM;i++){
        ofDrawBitmapString(vb_TimeLineUsedFlag[i], 850, (i+1)*30);
    }
}
//--------------------------------------------------------------
void ofxTimeLine::clear(){
    for(int i=0;i<STEP_NUM;i++){
        vb_TimeLineUsedFlag[i]=false;
    }
    for(int i=0;i<ACT_ID_NUM;i++){
        vb_ActState[i]=false;
    }
}
//--------------------------------------------------------------

bool ofxTimeLine::getState(int actId){
    if(vb_ActState[actId]){
        if(vb_ActOnceFlag[actId]){
            vb_ActState[actId]=false;
        }
        return true;
    }else{
        return false;
    }
}

int ofxTimeLine::getParam(int actId){
    return;
}

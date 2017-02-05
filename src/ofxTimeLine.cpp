/*
 *
*/

#include "ofxTimeLine.h"

#define f_TimeLineSabi 0.262

//--------------------------------------------------------------
ofxTimeLine::ofxTimeLine(){
}
//--------------------------------------------------------------
void ofxTimeLine::setup(){
    //start,end,ACT_ID,Param
    /*
     press 0.0135856 release 0.0138921
     press 0.0637401 release 0.0639444
     
     press 0.100922 release 0.101228
     press 0.103373 release 0.10368
     press 0.106336 release 0.106642
     press 0.1094 release 0.109707
     press 0.111647 release 0.111954
     press 0.113384 release 0.161291
     
     press 0.163028 release 0.163232
     press 0.175388 release 0.175694
     
     press 0.187747 release 0.188054
     press 0.194081 release 0.194285
     press 0.200414 release 0.20072
     
     press 0.212774 release 0.21308
     press 0.225031 release 0.225338
     
     press 0.237595 release 0.237902
     press 0.246891 release 0.247299
     
     press 0.25016 release 0.250466
     press 0.252509 release 0.252918
     press 0.254654 release 0.254961
     press 0.26395 release 0.361909
     */
    float timeLineData[STEP_NUM][4] = {
        {0.0,0.01,AID_INIT,0},

        {0.0264562/TIMELINE_SPEED,0.0264562/TIMELINE_SPEED+0.01,AID_AVS,0},
        {0.038714/TIMELINE_SPEED,0.038714/TIMELINE_SPEED+0.01,AID_AVS,1},
        {0.0514824/TIMELINE_SPEED,0.0514824/TIMELINE_SPEED+0.01,AID_AVS,2},
        
/*        {0.125948/TIMELINE_SPEED,0.125948/TIMELINE_SPEED+0.01,AID_AVS,0},
        {0.138206/TIMELINE_SPEED,0.138206/TIMELINE_SPEED+0.01,AID_AVS,1},
        {0.15077/TIMELINE_SPEED,0.15077/TIMELINE_SPEED+0.01,AID_AVS,2},*/
        
        {0.163028/TIMELINE_SPEED,0.163028/TIMELINE_SPEED+0.01,AID_AVS,3},
        {0.175388/TIMELINE_SPEED,0.175388/TIMELINE_SPEED+0.01,AID_AVS,4},
        
        {0.187747/TIMELINE_SPEED,0.187747/TIMELINE_SPEED+0.01,AID_AVS,5},
        {0.194081/TIMELINE_SPEED,0.194081/TIMELINE_SPEED+0.01,AID_AVS,6},
        {0.200414/TIMELINE_SPEED,0.200414/TIMELINE_SPEED+0.01,AID_AVS,7},
        
        {0.212774/TIMELINE_SPEED,0.212774/TIMELINE_SPEED+0.01,AID_AVS,8},
        {0.225031/TIMELINE_SPEED,0.225031/TIMELINE_SPEED+0.01,AID_AVS,9},
        
        {0.237595/TIMELINE_SPEED,0.237595/TIMELINE_SPEED+0.01,AID_AVS,10},
        {0.246891/TIMELINE_SPEED,0.246891/TIMELINE_SPEED+0.01,AID_AVS,11},
        
        {0.0135856,0.0135856+0.01,AID_FIRE_FLUID,0},//start
        {0.163028,0.163028+0.01,AID_FIRE_FLUID,1},//stop when AVS start (A melo)
        {f_TimeLineSabi,f_TimeLineSabi+0.01,AID_FIRE_FLUID,0},//start sabi

        {0.113384,0.163028,AID_TRI_BOAL,0},//start
        {0.163028,0.163028+0.1,AID_TRI_BOAL,1},//stop when AVS start (A melo)

        {0.0637401,0.0637401+0.01,AID_FLOW_TOOLS,0},//start
        {0.113384,0.113384+0.01,AID_FLOW_TOOLS,1},//intro melo
        
        {0.163028,0.163028+0.01,AID_FIRE_PARTICLE,0},//start (A melo) ParticleScale 10
        {f_TimeLineSabi,f_TimeLineSabi+0.01,AID_FIRE_PARTICLE,1},//start sabi

        {0.161291,0.161291+0.001,AID_FLASH,0},//intro end
        
        {0.25016,0.25016+0.001,AID_FLASH,0},//a (su) wo
        {0.252509,0.252509+0.001,AID_FLASH,0},//(o) mo
        {0.254654,0.254654+0.001,AID_FLASH,1},//(u)
        
        {0.101024,0.102556,AID_SHAKE,0},//shake one time
        {0.103271,0.106029,AID_SHAKE,0},//shake one time
        {0.10654,0.108787,AID_SHAKE,0},//shake one time
        {0.1094,0.111137,AID_SHAKE,0},//shake one time
        {0.111647,0.112077,AID_SHAKE,0},//shake one time

        {0.113384,0.161291,AID_SHAKE,1},//shake long (intro melo)

        {0.361909,0.361909+0.3,AID_FADE_OUT,0},//start
        
        {f_TimeLineSabi,f_TimeLineSabi+0.01,AID_CAM,0},//sabi
        
    };
    /*
     AID_INIT = 0,
     AID_AVS,
     AID_FLOW_TOOLS,
     AID_FIRE_PARTICLE,
     AID_TRI_BOAL,
     AID_FIRE_FLUID,
     AID_FLASH,
     AID_FADE_OUT,
     AID_SHAKE,
     AID_CAM,
     */
    bool onceFlag[ACT_ID_NUM] = {true,true,true,true,false,true,true,false,false,true};
    for(int i=0;i<STEP_NUM;i++){
        vvf_TimeLineTime.push_back(ofVec2f(timeLineData[i][0],timeLineData[i][1]));
        vi_TimeLineActId.push_back(timeLineData[i][2]);
        vi_TimeLineParam.push_back(timeLineData[i][3]);
        vb_TimeLineUsedFlag.push_back(false);
    }
    for(int i=0;i<ACT_ID_NUM;i++){
        vb_ActState.push_back(false);
        vb_ActOnceFlag.push_back(onceFlag[i]);
        vb_ActParam.push_back(-1);
    }
}

//--------------------------------------------------------------
void ofxTimeLine::update(float pos){
    for(int i=0;i<STEP_NUM;i++){
        if((vvf_TimeLineTime[i][0] < pos) && (pos < vvf_TimeLineTime[i][1])){
            if(vb_ActOnceFlag[vi_TimeLineActId[i]] && (vb_TimeLineUsedFlag[i])){
            }else{
                vb_ActState[vi_TimeLineActId[i]]=true;
                vb_ActParam[vi_TimeLineActId[i]]=vi_TimeLineParam[i];
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
    return vb_ActParam[actId];
}

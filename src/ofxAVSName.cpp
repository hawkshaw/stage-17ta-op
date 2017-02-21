/*
 *
*/
#pragma once

#ifndef __ofxAVSName__
#define __ofxAVSName__

#include "ofMain.h"
#include "ofApp.h"

void ofApp::setupAVSName(){
    i_AvsId = 0;
    font.loadFont(OF_TTF_SANS, 50);
    avs.setup("YUKI TAKAHASHI");
    ofxAVString avs1,avs2,avs3,avs4,avs5,avs6,avs7,avs8,avs9,avs10,avs11,avs12;
    
    avs1.setup("YUKI TAKAHASHI");
    vv_AvsPos.push_back(ofVec3f(50, -100, 15));
    avs2.setup("YURINA SHIKANO");
    vv_AvsPos.push_back(ofVec3f(30, -100, 0));
    avs3.setup("SHIHO ONOZAWA");
    vv_AvsPos.push_back(ofVec3f(10, -100, -15));

    avs4.setup("MANA YARITANI");
    vv_AvsPos.push_back(ofVec3f(0, 10, 15));
    avs5.setup("KOKI HODAMA");
    vv_AvsPos.push_back(ofVec3f(20, 10, 0));

    avs6.setup("MAKITO KOBAYASHI");
    vv_AvsPos.push_back(ofVec3f(0, -10, 15));
    avs7.setup("GO NAKATANI");
    vv_AvsPos.push_back(ofVec3f(20, -10, 0));
    avs8.setup("HARUKA NAKAI");
    vv_AvsPos.push_back(ofVec3f(40, -10, -15));
    
    avs9.setup("HIROYUKI SHIMA");
    vv_AvsPos.push_back(ofVec3f(10, -50, 0));
    avs10.setup("KYOHEI KIKUCHI");
    vv_AvsPos.push_back(ofVec3f(30, -50, -15));

    avs11.setup("MASARU MIZUOCHI");
    vv_AvsPos.push_back(ofVec3f(-10, -80, 15));
    avs12.setup("MINAMI BABA");
    vv_AvsPos.push_back(ofVec3f(-30, -80, 0));

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
    for(int i=0;i<v_avs.size();i++){
        vf_RandomAngleZ.push_back(0);
    }
    b_DrawAVS = true;
}

void ofApp::updateAVSName(){
    int avsParam = timeLine.getParam(AID_AVS);
    ofVec3f avsPos = vv_AvsPos[avsParam];
    v_avs[avsParam].play(0,600,600);
    //cout << (v_avs[avsParam].getTar()).size() << endl;
    //cout << -20*(unsigned short)(v_avs[avsParam].getTar().size()) << endl;
    vf_RandomAngleZ[avsParam] = ofRandom(-10.0,10.0);
    if(avsParam<3){
        vf_RandomAngleZ[avsParam] = 0;
    }
    rollCam.setPos(avsPos.x, avsPos.y, avsPos.z);
}

void ofApp::drawAVSName(){
    {
        ofPushMatrix();
        if(avs.getLastMillis() + 1000 > ofGetElapsedTimeMillis()){
            font.drawString(avs, 0, 0);
        }
        
        for(int i=0;i<v_avs.size();i++){
            ofPushMatrix();
            
            ofRotateZ(-vv_AvsPos[i].z);
            ofRotateY(-vv_AvsPos[i].y);
            ofRotateX(-vv_AvsPos[i].x);
            ofTranslate(0, 0,300);
            ofRotateZ(vf_RandomAngleZ[i]);
            //ofTranslate(0, 0,50);
            
            ofScale(0.2,0.2,0.2);
            int stayMillis = 5000;
            if (i<3)stayMillis = 1000;
            if(v_avs[i].getLastMillis() + stayMillis > ofGetElapsedTimeMillis()){
                myFboAvs.begin();
                //ofBackground(0);
                ofClearAlpha();
                ofClear(0);
                //font.drawString(v_avs[i], -25*(unsigned short)(v_avs[i].getTar().size()), 0);
                font.drawString(v_avs[i], 30, 30);
                myFboAvs.end();
                myGlitchAvs.generateFx();
                ofRotateX(180);
                if(i<3){
 //                   myFboAvs.draw(-10-19*(unsigned short)(v_avs[i].getTar().size()), -230);
                    myFboAvs.draw(-220-5*(unsigned short)(v_avs[i].getTar().size()), -230);
                }else{
                    myFboAvs.draw(-25*(unsigned short)(v_avs[i].getTar().size()), -30);
                }
            }
            
            ofPopMatrix();
        }
        ofPopMatrix();
    }
}

#endif

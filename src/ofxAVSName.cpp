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
    font.loadFont(OF_TTF_SANS, 100);
    avs.setup("YUKI TAKAHASHI");
    ofxAVString avs1,avs2,avs3,avs4,avs5,avs6,avs7,avs8,avs9,avs10,avs11,avs12;
    
    avs1.setup("YUKI TAKAHASHI");
    vv_AvsPos.push_back(ofVec3f(30, 120, 15));
    avs2.setup("YURINA SHIKANO");
    vv_AvsPos.push_back(ofVec3f(-30, 0, -15));
    avs3.setup("SHIHO ONOZAWA");
    vv_AvsPos.push_back(ofVec3f(30, -120, 15));

    avs4.setup("MANA YARITANI");
    vv_AvsPos.push_back(ofVec3f(0, 60, 15));
    avs5.setup("KOKI HODAMA");
    vv_AvsPos.push_back(ofVec3f(10, 60, 0));

    avs6.setup("MAKITO KOBAYASHI");
    vv_AvsPos.push_back(ofVec3f(0, -60, 15));
    avs7.setup("GO NAKATANI");
    vv_AvsPos.push_back(ofVec3f(10, -60, 0));
    avs8.setup("HARUKA NAKAI");
    vv_AvsPos.push_back(ofVec3f(20, -60, -15));
    
    avs9.setup("HIROYUKI SHIMA");
    vv_AvsPos.push_back(ofVec3f(20, 100, 0));
    avs10.setup("KYOHEI KIKUCHI");
    vv_AvsPos.push_back(ofVec3f(30, 100, -15));

    avs11.setup("MASARU MIZUOCHI");
    vv_AvsPos.push_back(ofVec3f(-10, -30, 15));
    avs12.setup("MINAMI BABA");
    vv_AvsPos.push_back(ofVec3f(-20, -30, 0));

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

void ofApp::updateAVSName(){
}

void ofApp::drawAVSName(){
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
}

#endif

/*
 *
*/
#pragma once

#ifndef __ofxFireFluid__
#define __ofxFireFluid__

#include "ofMain.h"
#include "ofApp.h"

void ofApp::setupFireFluid(){
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
}


void ofApp::updateFireFluid(){
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

void ofApp::drawFireFluid(){
    ofPushMatrix();
    {
        ofTranslate(-width/2, -height/2);
        {
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
}

#endif

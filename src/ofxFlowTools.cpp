/*
 *
 */
#pragma once

#ifndef __ofxFlowTools__
#define __ofxFlowTools__

#include "ofMain.h"
#include "ofApp.h"

void ofApp::setupFlowTools(){
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
    
    b_FlowToolsBarActivate=false;
    i_FlowToolsBarCount=0;
    
    // MOUSE DRAW
    mouseForces.setup(flowWidth, flowHeight, drawWidth, drawHeight);
    
    // CAMERA
    simpleCam.setup(640, 480, true);
    didCamUpdate = false;
    cameraFbo.allocate(640, 480);
    cameraFbo.black();

    
    simpleMovie.load("back.mov");
    simpleMovie.setVolume(0);
    i_FlowToolAlpha=0;
}


void ofApp::updateFlowTools(){
    deltaTime = ofGetElapsedTimef() - lastTime;
    lastTime = ofGetElapsedTimef();
    
    //simpleCam.update();

    simpleMovie.update();

    
    //if (simpleCam.isFrameNew()) {
    if (simpleMovie.isFrameNew()) {
        ofPushStyle();
        ofEnableBlendMode(OF_BLENDMODE_DISABLED);
        cameraFbo.begin();
        
        if (doFlipCamera){
            //simpleCam.draw(cameraFbo.getWidth(), 0, -cameraFbo.getWidth(), cameraFbo.getHeight());  // Flip Horizontal
            simpleMovie.draw(cameraFbo.getWidth(), 0, -cameraFbo.getWidth(), cameraFbo.getHeight());  // Flip Horizontal
        }else{
            //simpleCam.draw(0, 0, cameraFbo.getWidth(), cameraFbo.getHeight());
            simpleMovie.draw(0, 0, cameraFbo.getWidth(), cameraFbo.getHeight());
        }
        
        /*
        ofBackground(0, 0, 0);
        ofFill();
        ofSetColor(255);
        ofDrawRectangle(0, ofGetHeight()/2, ofGetWidth(), 400*sin(ofGetElapsedTimeMillis()/100.0));
        
        
        if(b_FlowToolsBarActivate && (i_FlowToolsBarCount==0)){
            i_FlowToolsBarCount = 10;
        }
        if(b_FlowToolsBarActivate && (i_FlowToolsBarCount==1)){
            i_FlowToolsBarCount = 0;
            b_FlowToolsBarActivate = false;
        }
        if(b_FlowToolsBarActivate && (i_FlowToolsBarCount>0)){
            i_FlowToolsBarCount -= 1;
            ofBackground(0, 0, 0);
            ofFill();
            ofSetColor(255);
            ofDrawRectangle(0, ofGetHeight()-100+10*i_FlowToolsBarCount, ofGetWidth(), 30);
        }
        */
        /*ofFill();
        ofBackground(0, 0, 0);
        ofSetColor(128,128,0);
        cout << ((int)(ofGetElapsedTimeMillis()))%ofGetHeight() <<endl;
        ofDrawRectangle(0, ((int)(ofGetElapsedTimeMillis()/2))%ofGetHeight(), ofGetWidth(), 50);
        */
        
        
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

void ofApp::drawFlowTools(){
    ofPushStyle();
    ofPushMatrix();
    {
        //ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2); //when in cam
        //if(doDrawCamBackground.get())drawSource();
        {
            i_FlowToolAlpha = testParam1;
            ofEnableAlphaBlending();
            ofSetColor(255, 255, 255, i_FlowToolAlpha);
            drawComposite();
        }
    }
    ofPopMatrix();
    ofPopStyle();
    
}

#endif

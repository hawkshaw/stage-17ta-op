/*
 *
*/
#pragma once

#ifndef __ofxFireParticle__
#define __ofxFireParticle__

#include "ofMain.h"
#include "ofApp.h"

void ofApp::setupFireParticle(){
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

}


void ofApp::updateFireParticle(){
    {
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
}

void ofApp::drawFireParticle(){
    ofPushMatrix();
    {
        em.setFlow(1500);
        //sys.debugDraw();
        sys.draw();
    }
    ofPopMatrix();
}

#endif

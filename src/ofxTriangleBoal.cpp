/*
 *
 */
#pragma once

#ifndef __ofxTriangleBoal__
#define __ofxTriangleBoal__

#include "ofMain.h"
#include "ofApp.h"

void ofApp::setupTriangleBoal(){
    
    nTri = 1500;			//The number of the triangles
    nVert= nTri * 3;		//The number of the vertices
    
    float Rad = 250;	//The sphere's radius
    float rad = 25;	//Maximal triangle's √¨radius√Æ
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
    


    i_TriBoalAppearCount=10;
    i_TriBoalAppearCountBuf = i_TriBoalAppearCount;

}


void ofApp::updateTriangleBoal(){
    //Fill the array of triangles' colors
    colors.resize( nTri );
    for (int i=0; i<nTri; i++) {
        //Take a random color from black to red
        colors[i] = ofColor( ofRandom( 0, 255 ), 0, 0 ,(int)((1-(1.0*i_TriBoalAppearCountBuf/i_TriBoalAppearCount))*ofRandom( 0, 255 )));
        //colors[i] = ofColor( ofRandom( 0, 255 ), 0, 0 ,(int)(0.1*ofRandom( 0, 255 )));
        //ofSetColor(255, 255, 255, 255*(1-(i_TriBoalAppearCountBuf/i_TriBoalAppearCount)));
        
    }
    
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
}

void ofApp::plusTriangleBoal(){
    i_TriBoalAppearCountBuf = max(0,i_TriBoalAppearCountBuf-1);
}
void ofApp::minusTriangleBoal(){
    i_TriBoalAppearCountBuf = min(i_TriBoalAppearCount,i_TriBoalAppearCountBuf+1);
}

void ofApp::drawTriangleBoal(){
    ofPushStyle();
    ofEnableAlphaBlending();
    ofPushMatrix();						//Store the coordinate system
    //Move the coordinate center to screen's center
    //ofTranslate( ofGetWidth()/2, ofGetHeight()/2, 0 );
    ofTranslate(ofPoint(0,-200.0* pow((1.0*i_TriBoalAppearCountBuf/i_TriBoalAppearCount),2),0));
    ofTranslate(ofPoint(0,-height*0.15+300,-400));
    drawOneTriangleBoal();
    ofTranslate(ofPoint(400,0,100));
    drawOneTriangleBoal();
    ofTranslate(ofPoint(-800,0,0));
    drawOneTriangleBoal();
    ofTranslate(ofPoint(200,-300,100));
    drawOneTriangleBoal();
    ofTranslate(ofPoint(400,0,100));
    drawOneTriangleBoal();
    
    //Calculate the rotation angle
    ofPopMatrix();	//Restore the coordinate system
    ofPopStyle();
}

void ofApp::drawOneTriangleBoal(){
    ofPushStyle();
    ofPushMatrix();
    ofScale(0.3, 0.3, 0.3);
    
    float time = ofGetElapsedTimef();	//Get time in seconds
    float angle = time * 10;			//Compute angle. We rotate at speed 10 degrees per second
    ofRotate( angle, 0, 1, 0 );			//Rotate the coordinate system along y-axe
    
    //Draw the triangles
    const float f_triSoundLevel = 80.0;
    for (int i=0; i<nTri; i++) {
        ofSetColor( colors[i] );		//Set color
        ofTriangle(vertices[ i*3     ] * min(2.0,0.8 + ofRandom(0.5,1.0)*spectrum_ave_edge1*f_triSoundLevel),
                   vertices[ i*3 + 1 ] * min(2.0,0.8 + ofRandom(0.5,1.0)*spectrum_ave_edge2*f_triSoundLevel),
                   vertices[ i*3 + 2 ] * min(2.0,0.8 + ofRandom(0.5,1.0)*spectrum_ave_edge3*f_triSoundLevel));		//Draw triangle
    }
    ofPopMatrix();
    ofPopStyle();
}



#endif

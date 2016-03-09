#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    shadertoy.load("shaders/p_malin_reprojection.frag");
    ofImage img;
    img.load("shaders/tex04.jpg");
    ///*
    ofTexture tex;
    tex.allocate(img.getWidth(), img.getHeight(), GL_RGBA, false); // fourth parameter is false to avoid generation of a GL_TEXTURE_2D_RECTANGLE texture - we don't want this.
    tex.loadData(img.getPixels());
    tex.generateMipmap();
    tex.setTextureWrap(GL_REPEAT, GL_REPEAT);
    shadertoy.setTexture(0, tex);
    ofSetFrameRate(60);
    shadertoy.setAdvanceTime(true);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    shadertoy.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == 'f') {
        ofToggleFullscreen();
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

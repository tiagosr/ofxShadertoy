#include "ofApp.h"

bool showfps = false;
float fps;

//--------------------------------------------------------------
void ofApp::setup(){
    shadertoy.load("shaders/raymarch.frag");
    ofSetFrameRate(60);
    camera.movespeed = 0.05;
    camera.rollspeed = camera.sensitivity = 0.2;
    camera.enableControls();
    shadertoy.setAdvanceTime(true);
    shadertoy.setCamera(&camera);
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
    shadertoy.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    if(showfps)ofDrawBitmapString(ofToString(ofGetFrameRate()), ofPoint(10, 10));

}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == 'r') {
        shadertoy.load("shaders/raymarch.frag");
    }
    if (key == ' ') {
        showfps = !showfps;
    }if(key == 'f') {
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

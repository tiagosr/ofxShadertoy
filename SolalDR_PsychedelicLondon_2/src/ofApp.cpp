#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofDisableArbTex();
    if (!shadertoy.load("shaders/SolalDR_PsychedelicLondon-2.frag")) {
        ofLogError() << "Error loading shader!";
    }
    ofImage london("images/london.jpg");
    // textures in GLSL are flipped vertically, so we need to flip them back up
    // to use with Shadertoy shaders - most images are preset to use VFlip there
    // this has the same effect of setting VFlip on the texture in Shadertoy
    london.mirror(true, false);
    londonImage.allocate(london.getPixelsRef());
    // this is the same as setting Wrap to "repeat" in Shadertoy
    londonImage.setTextureWrap(GL_REPEAT, GL_REPEAT);

    noiseImage = ofxShadertoy::createRGBANoiseSmall();
    // this is the same as setting Wrap to "repeat" in Shadertoy
    noiseImage.setTextureWrap(GL_REPEAT, GL_REPEAT);
    shadertoy.setTexture(0, londonImage);
    shadertoy.setTexture(1, noiseImage);
    ofSetFrameRate(60);
    shadertoy.setAdvanceTime(true);
    gui.setup();
    gui.add(effectStrength.setup("effect strength", 0.5, 0.0, 1.0));
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
    shadertoy.setDimensions(ofGetWindowWidth(), ofGetWindowHeight());
    shadertoy.begin();
    shadertoy.setUniform1f("effectStrength", effectStrength);
    ofDrawRectangle(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    shadertoy.end();
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'f') {
        ofToggleFullscreen();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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

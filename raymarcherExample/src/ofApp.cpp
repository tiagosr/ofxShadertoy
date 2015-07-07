#include "ofApp.h"

bool showfps = false;
float fps;
void reloadShaders(ofApp &app) {
    ofShader currentShader;
    currentShader.setupShaderFromSource(GL_VERTEX_SHADER, string("#version 330\n"
                                        "precision mediump float;"
                                        "uniform mat4 modelViewProjectionMatrix;"
                                        "uniform mat4 tCameraMatrix;"
                                        "in vec4 position;"
                                        "void main(){"
                                        "    gl_Position = modelViewProjectionMatrix * position;"
                                        "}\n"));
    ofFile f;
    f.open("shaders/raymarch.frag");
    ofBuffer b = f.readToBuffer();
    string fragprogram =
    string("#version 330\n"
           "precision mediump float;\n"
           "uniform vec3      iResolution;\n"
           "uniform float     iGlobalTime;\n"
           "uniform float     iChannelTime[4];\n"
           "uniform vec4      iMouse;\n"
           "uniform vec4      iDate;\n"
           "uniform float     iSampleRate;\n"
           "uniform vec3      iChannelResolution[4];\n"
           "uniform mat4      tCameraMatrix;\n")+
    b.getText()+
    string("\nout vec4 FragColor;"
           "void main( void )"
           "{"
           "vec4 color = vec4(0.0,0.0,0.0,1.0);"
           "mainImage( color, gl_FragCoord.xy );"
           "color.w = 1.0;"
           "FragColor = color;"
           "}");
    if(!currentShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragprogram)) {
        ofLogError("shader") << "error loading shader";
    } else {
        if(currentShader.linkProgram()) {
            currentShader.bindDefaults();
            currentShader.setUniform1f("iGlobalTime", ofGetElapsedTimef());
            currentShader.setUniform4f("iMouse", ofGetMouseX(), ofGetMouseY(), ofGetMousePressed()?1:0, 0);
            currentShader.setUniform3f("iResolution", (float)ofGetWindowWidth(), (float)ofGetWindowHeight(), 4.0f);
            currentShader.setUniform4f("iDate", ofGetYear(), ofGetMonth(), ofGetDay(), ((ofGetHours()*60+ofGetMinutes())*60)+ofGetSeconds());
            currentShader.setUniformMatrix4f("tCameraMatrix", ofMatrix4x4::newIdentityMatrix());
            app.currentShader = currentShader;
        } else {
            ofLogError("shader") << "did not link";
        }
    }
}

//--------------------------------------------------------------
void ofApp::setup(){
    reloadShaders(*this);
    ofSetFrameRate(60);
    camerapos = ofVec3f(0, 0.25, 2.0);
    camerarot = ofQuaternion();
	//ofGrabCursor();
    camera.movespeed = 0.05;
    camera.rollspeed = camera.sensitivity = 0.2;
    camera.enableControls();
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    currentShader.begin();
    currentShader.setUniform1f("iGlobalTime", ofGetElapsedTimef());
    currentShader.setUniform4f("iMouse", mousepos.x, mousepos.y, ofGetMousePressed()?1:0, 0);
    currentShader.setUniform3f("iResolution", (float)ofGetWindowWidth(), (float)ofGetWindowHeight(), 0);
    currentShader.setUniform4f("iDate", ofGetYear(), ofGetMonth(), ofGetDay(), ((ofGetHours()*60+ofGetMinutes())*60)+ofGetSeconds());
    ofMatrix4x4 cmtx(camera.getOrientationQuat());
    cmtx.setTranslation(-camera.getPosition());
    currentShader.setUniformMatrix4f("tCameraMatrix", cmtx);
    
    ofRect(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
    currentShader.end();
    if(showfps)ofDrawBitmapString(ofToString(ofGetFrameRate()), ofPoint(10, 10));

}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == 'r') {
        reloadShaders(*this);
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
    mousepos.set(ofGetMouseX(), ofGetMouseY());
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    mousepos.set(ofGetMouseX(), ofGetMouseY());
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

#pragma once

#include "ofMain.h"
#include "ofxFPSControls.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    ofShader currentShader;
    ofVec3f mousepos, mousedelta;
    ofVec3f cameramove;
    ofVec3f camerapos;
    ofQuaternion camerarot;
    ofxFPSCamera camera;
    bool b_up, b_down, b_left, b_right;
};

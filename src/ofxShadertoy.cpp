//
//  ofxShadertoy.cpp
//  RayWarper
//
//  Created by Tiago Rezende on 6/26/15.
//
//

#include "ofxShadertoy.h"

ofxShadertoy::ofxShadertoy():globalTime(0),advanceTime(true),camera(NULL) {
    channel0.allocate(1, 1, GL_RGBA);
    channel1.allocate(1, 1, GL_RGBA);
    channel2.allocate(1, 1, GL_RGBA);
    channel3.allocate(1, 1, GL_RGBA);
    ofAddListener(ofEvents().update, this, &ofxShadertoy::update);
}
ofxShadertoy::~ofxShadertoy() {
    ofRemoveListener(ofEvents().update, this, &ofxShadertoy::update);
}

bool ofxShadertoy::load(string shaderfilename, bool chan0cube, bool chan1cube, bool chan2cube, bool chan3cube) {
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
    f.open(shaderfilename);
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
    string(chan0cube?"uniform sampler2DCube iChannel0;\n":"uniform sampler2D  iChannel0;\n")+
    string(chan1cube?"uniform sampler2DCube iChannel1;\n":"uniform sampler2D  iChannel1;\n")+
    string(chan2cube?"uniform sampler2DCube iChannel2;\n":"uniform sampler2D  iChannel2;\n")+
    string(chan3cube?"uniform sampler2DCube iChannel3;\n":"uniform sampler2D  iChannel3;\n")+
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
        return false;
    } else {
        if(currentShader.linkProgram()) {
            currentShader.bindDefaults();
            currentShader.setUniform1f("iGlobalTime", ofGetElapsedTimef());
            currentShader.setUniform4f("iMouse", mousepos.x, mousepos.y, ofGetMousePressed()?1:0, 0);
            dimensions.set(ofGetWindowWidth(), ofGetWindowHeight());
            currentShader.setUniform3f("iResolution", dimensions.x, dimensions.y, 4.0f);
            currentShader.setUniform4f("iDate", ofGetYear(), ofGetMonth(), ofGetDay(), ((ofGetHours()*60+ofGetMinutes())*60)+ofGetSeconds());
            currentShader.setUniformMatrix4f("tCameraMatrix", ofMatrix4x4::newIdentityMatrix());
            shader = currentShader;
            return true;
        } else {
            ofLogError("shader") << "did not link";
            return false;
        }
    }
}

void ofxShadertoy::begin() {
    shader.begin();
    shader.setUniform1f("iGlobalTime", globalTime);
    shader.setUniform4f("iMouse", mousepos.x, mousepos.y, ofGetMousePressed()?1:0, 0);
    shader.setUniform3f("iResolution", dimensions.x, dimensions.y, 4.0f);
    shader.setUniform4f("iDate", ofGetYear(), ofGetMonth(), ofGetDay(), ((ofGetHours()*60+ofGetMinutes())*60)+ofGetSeconds());
    if(camera) {
        ofMatrix4x4 cmtx(camera->getOrientationQuat());
        cmtx.setTranslation(-camera->getPosition());
        shader.setUniformMatrix4f("tCameraMatrix", cmtx);
    } else {
        shader.setUniformMatrix4f("tCameraMatrix", ofMatrix4x4::newIdentityMatrix());
    }
    shader.setUniformTexture("iChannel0", channel0, 0);
    shader.setUniformTexture("iChannel1", channel1, 1);
    shader.setUniformTexture("iChannel2", channel2, 0);
    shader.setUniformTexture("iChannel3", channel3, 1);
}

void ofxShadertoy::end() {
    shader.end();
}

void ofxShadertoy::draw(float x, float y, float z, float w, float h) {
    dimensions.x = w;
    dimensions.y = h;
    begin();
    ofRect(x, y, z, w, h);
    end();
}

void ofxShadertoy::update(ofEventArgs &event) {
    if(advanceTime) {
        globalTime += ofGetLastFrameTime();
    }
    if(useMouse && ofGetMousePressed()) {
        mousepos.set(ofGetMouseX(), ofGetMouseY());
    }
}

void ofxShadertoy::setAdvanceTime(bool advance) {
    advanceTime = advance;
}

void ofxShadertoy::setUseMouse(bool use) {
    useMouse = use;
}

void ofxShadertoy::setCamera(ofCamera* cam) {
    camera = cam;
}

void ofxShadertoy::setTexture(int index, const ofTexture& tex) {
    switch (index) {
        case 0:
            channel0 = tex;
            break;
        case 1:
            channel1 = tex;
            break;
        case 2:
            channel2 = tex;
            break;
        case 3:
            channel3 = tex;
            break;
    }
}

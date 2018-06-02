//
//  ofxShadertoy.cpp
//  RayWarper
//
//  Created by Tiago Rezende on 6/26/15.
//
//

#include "ofxShadertoy.h"

ofxShadertoy::ofxShadertoy():globalTime(0),advanceTime(true),camera(NULL),dimensions(640.f, 480.f) {
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
        string(
            "#version 330\n"
            "precision mediump float;\n"
            "vec4 texture2D(sampler2D tex, vec2 coords) { return texture(tex, coords.xy); }\n"
            //"vec4 texture2DCube(sampler2DCube tex, vec2 coords) { return texture(tex, coords); }\n"
            "#define texture   texture2D\n"
            "uniform vec3      iResolution;\n"
            "uniform float     iGlobalTime;\n"
            "uniform float     iTime;\n"
            "uniform int       iFrame;\n"
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

void ofxShadertoy::begin() const {
    shader.begin();
    shader.setUniform1f("iGlobalTime", globalTime);
    shader.setUniform1f("iTime", globalTime);
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
    if(channel0.isAllocated()) {
        channel0.bind();
        shader.setUniformTexture("iChannel0", channel0, 1);
    }
    shader.setUniformTexture("iChannel1", channel1, 2);
    shader.setUniformTexture("iChannel2", channel2, 3);
    shader.setUniformTexture("iChannel3", channel3, 4);
}

void ofxShadertoy::end() const {
    shader.end();
}

void ofxShadertoy::draw(float x, float y, float z, float w, float h) const {
    begin();
    ofDrawRectangle(x, y, z, w, h);
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

void ofxShadertoy::setTexture(int index, ofFbo const & tex) {
    switch (index) {
    case 0:
        channel0 = tex.getTexture(0);
        break;
    case 1:
        channel1 = tex.getTexture(0);
        break;
    case 2:
        channel2 = tex.getTexture(0);
        break;
    case 3:
        channel3 = tex.getTexture(0);
        break;
    }
}

ofTexture ofxShadertoy::createGrayNoise(size_t size) {
    ofTexture tex;
    ofFloatPixels noisePixels;
    noisePixels.allocate(size, size, ofImageType::OF_IMAGE_GRAYSCALE);
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            noisePixels[(x + size * y)] = ofNoise(x, y);
        }
    }
    tex.allocate(noisePixels);
    return tex;
}

ofTexture ofxShadertoy::createGrayNoiseSmall() {
    return createGrayNoise(64);
}

ofTexture ofxShadertoy::createGrayNoiseMedium() {
    return createGrayNoise(256);
}

ofTexture ofxShadertoy::createGrayNoiseLarge() {
    return createGrayNoise(1024);
}

ofTexture ofxShadertoy::createRGBANoise(size_t size) {
    ofTexture tex;
    ofFloatPixels noisePixels;
    noisePixels.allocate(size, size, ofImageType::OF_IMAGE_COLOR_ALPHA);
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            noisePixels[(x + size * y) * 4    ] = ofNoise(x, y);
            noisePixels[(x + size * y) * 4 + 1] = ofNoise(x+size, y);
            noisePixels[(x + size * y) * 4 + 2] = ofNoise(x, y+size);
            noisePixels[(x + size * y) * 4 + 3] = ofNoise(x+size, y+size);
        }
    }
    tex.allocate(noisePixels);
    return tex;
}

ofTexture ofxShadertoy::createRGBANoiseSmall() {
    return createRGBANoise(64);
}

ofTexture ofxShadertoy::createRGBANoiseMedium() {
    return createRGBANoise(256);
}

ofTexture ofxShadertoy::createRGBANoiseLarge() {
    return createRGBANoise(1024);
}

void ofxShadertoy::setUniform1i(std::string const & uniformName, int value) const {
    shader.setUniform1i(uniformName, value);
}

void ofxShadertoy::setUniform2i(std::string const & uniformName, int v1, int v2) const {
    shader.setUniform2i(uniformName, v1, v2);
}

void ofxShadertoy::setUniform3i(std::string const & uniformName, int v1, int v2, int v3) const {
    shader.setUniform3i(uniformName, v1, v2, v3);
}

void ofxShadertoy::setUniform4i(std::string const & uniformName, int v1, int v2, int v3, int v4) const {
    shader.setUniform4i(uniformName, v1, v2, v3, v4);
}

void ofxShadertoy::setUniform1f(std::string const & uniformName, float value) const {
    shader.setUniform1f(uniformName, value);
}

void ofxShadertoy::setUniform2f(std::string const & uniformName, float v1, float v2) const {
    shader.setUniform2f(uniformName, v1, v2);
}

void ofxShadertoy::setUniform2f(std::string const & uniformName, ofVec2f const & value) const {
    shader.setUniform2f(uniformName, value);
}

void ofxShadertoy::setUniform3f(std::string const & uniformName, float v1, float v2, float v3) const {
    shader.setUniform3f(uniformName, v1, v2, v3);
}

void ofxShadertoy::setUniform3f(std::string const & uniformName, ofVec3f const & value) const {
    shader.setUniform3f(uniformName, value);
}

void ofxShadertoy::setUniform4f(std::string const & uniformName, float v1, float v2, float v3, float v4) const {
    shader.setUniform4f(uniformName, v1, v2, v3, v4);
}

void ofxShadertoy::setUniform4f(std::string const & uniformName, ofVec4f const & value) const {
    shader.setUniform4f(uniformName, value);
}

void ofxShadertoy::setUniform4f(std::string const & uniformName, ofFloatColor const & value) const {
    shader.setUniform4f(uniformName, value);
}

void ofxShadertoy::setUniform1iv(std::string const & uniformName, int const * v, size_t count) const {
    shader.setUniform1iv(uniformName, v, count);
}

void ofxShadertoy::setUniform2iv(std::string const & uniformName, int const * v, size_t count) const {
    shader.setUniform2iv(uniformName, v, count);
}

void ofxShadertoy::setUniform3iv(std::string const & uniformName, int const * v, size_t count) const {
    shader.setUniform3iv(uniformName, v, count);
}

void ofxShadertoy::setUniform4iv(std::string const & uniformName, int const * v, size_t count) const {
    shader.setUniform4iv(uniformName, v, count);
}

void ofxShadertoy::setUniform1fv(std::string const & uniformName, float const * v, size_t count) const {
    shader.setUniform1fv(uniformName, v, count);
}

void ofxShadertoy::setUniform2fv(std::string const & uniformName, float const * v, size_t count) const {
    shader.setUniform2fv(uniformName, v, count);
}

void ofxShadertoy::setUniform3fv(std::string const & uniformName, float const * v, size_t count) const {
    shader.setUniform3fv(uniformName, v, count);
}

void ofxShadertoy::setUniform4fv(std::string const & uniformName, float const * v, size_t count) const {
    shader.setUniform4fv(uniformName, v, count);
}

void ofxShadertoy::setUniforms(ofParameterGroup const & parameters) const {
    shader.setUniforms(parameters);
}

void ofxShadertoy::setUniformMatrix3f(std::string const & uniformName, ofMatrix3x3 const & matrix) const {
    shader.setUniformMatrix3f(uniformName, matrix);
}

void ofxShadertoy::setUniformMatrix4f(std::string const & uniformName, ofMatrix4x4 const & matrix) const {
    shader.setUniformMatrix4f(uniformName, matrix);
}





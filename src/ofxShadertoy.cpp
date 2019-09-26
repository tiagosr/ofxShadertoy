//
//  ofxShadertoy.cpp
//  RayWarper
//
//  Created by Tiago Rezende on 6/26/15.
//
//

#include "ofxShadertoy.h"

bool ofxShadertoy::makeShader(std::string const & textBuffer, ofShader & outShader) {
    ofShader currentShader;
    currentShader.setupShaderFromSource(
        GL_VERTEX_SHADER,
        string(
            "#version 330\n"
            "precision mediump float;"
            "uniform mat4 modelViewProjectionMatrix;"
            "uniform mat4 tCameraMatrix;"
            "in vec4 position;"
            "void main(){"
            "    gl_Position = modelViewProjectionMatrix * position;"
            "}\n"));
    string fragprogram =
        string(
            "#version 330\n"
            "precision mediump float;\n"
            "vec4 texture2D(sampler2D tex, vec2 coords) { return texture(tex, coords.xy); }\n"
            "vec4 texture2DCube(samplerCube tex, vec3 coords) { return texture(tex, coords); }\n"
            "uniform vec3       iResolution;\n"
            "uniform float      iGlobalTime;\n"
            "uniform float      iTime;\n"
            "uniform int        iFrame;\n"
            "uniform float      iChannelTime[4];\n"
            "uniform vec4       iMouse;\n"
            "uniform vec4       iDate;\n"
            "uniform float      iSampleRate;\n"
            "uniform vec3       iChannelResolution[4];\n"
            "uniform mat4       tCameraMatrix;\n"
            "uniform float      xOutputAlpha;\n"
            "uniform sampler2D  iChannel0;\n"
            "uniform sampler2D  iChannel1;\n"
            "uniform sampler2D  iChannel2;\n"
            "uniform sampler2D  iChannel3;\n")+
        textBuffer+
        string("\nout vec4 FragColor;"
            "void main( void ) {"
            "    vec4 color = vec4(0.0,0.0,0.0,1.0);"
            "    mainImage( color, gl_FragCoord.xy );"
            "    color.w = 1.0;"
            "    FragColor = color;"
            "}");
    if(!currentShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragprogram)) {
        ofLogError("shader") << "error loading shader";
        return false;
    } else {
        if(currentShader.linkProgram()) {
            currentShader.bindDefaults();
            currentShader.setUniform1f("iGlobalTime", ofGetElapsedTimef());
            currentShader.setUniform4f("iMouse", mousepos.x, mousepos.y, ofGetMousePressed()?1:0, 0);
            currentShader.setUniform3f("iResolution", dimensions.x, dimensions.y, 4.0f);
            currentShader.setUniform4f("iDate", ofGetYear(), ofGetMonth(), ofGetDay(), ((ofGetHours()*60+ofGetMinutes())*60)+ofGetSeconds());
            currentShader.setUniformMatrix4f("tCameraMatrix", ofMatrix4x4::newIdentityMatrix());
            outShader = currentShader;
            return true;
        } else {
            ofLogError("shader") << "did not link";
            return false;
        }
    }
}

bool ofxShadertoy::makeSoundShader(std::string const & textBuffer, ofShader & outShader) {
    ofShader currentShader;
    currentShader.setupShaderFromSource(
        GL_VERTEX_SHADER,
        string(
            "#version 330\n"
            "precision mediump float;"
            "uniform mat4 modelViewProjectionMatrix;"
            "uniform mat4 tCameraMatrix;"
            "in vec4 position;"
            "void main(){"
            "    gl_Position = modelViewProjectionMatrix * position;"
            "}\n"));
    string fragprogram =
        string(
            "#version 330\n"
            "precision mediump float;\n"
            "vec4 texture2D(sampler2D tex, vec2 coords) { return texture(tex, coords.xy); }\n"
            "vec4 texture2DCube(samplerCube tex, vec3 coords) { return texture(tex, coords); }\n"
            "uniform vec3       iResolution;\n"
            "uniform float      iGlobalTime;\n"
            "uniform float      iTime;\n"
            "uniform int        iFrame;\n"
            "uniform float      iChannelTime[4];\n"
            "uniform vec4       iMouse;\n"
            "uniform vec4       iDate;\n"
            "uniform float      iSampleRate;\n"
            "uniform vec3       iChannelResolution[4];\n"
            "uniform mat4       tCameraMatrix;\n"
            "uniform sampler2D  iChannel0;\n"
            "uniform sampler2D  iChannel1;\n"
            "uniform sampler2D  iChannel2;\n"
            "uniform sampler2D  iChannel3;\n") +
        textBuffer +
        string("\nout vec4 FragColor;"
            "void main( void ) {"
            "    vec2 sample = vec2();"
            "    mainSound(sample, gl_FragCoord.x);"
            "    vec4 color = vec4(sample, 0.0, 1.0);"
            "    FragColor = color;"
            "}");
    if(!currentShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragprogram)) {
        ofLogError("shader") << "error loading shader";
        return false;
    } else {
        if(currentShader.linkProgram()) {
            currentShader.bindDefaults();
            currentShader.setUniform1f("iGlobalTime", ofGetElapsedTimef());
            currentShader.setUniform4f("iMouse", mousepos.x, mousepos.y, ofGetMousePressed()?1:0, 0);
            currentShader.setUniform3f("iResolution", dimensions.x, dimensions.y, 4.0f);
            currentShader.setUniform4f("iDate", ofGetYear(), ofGetMonth(), ofGetDay(), ((ofGetHours()*60+ofGetMinutes())*60)+ofGetSeconds());
            currentShader.setUniformMatrix4f("tCameraMatrix", ofMatrix4x4::newIdentityMatrix());
            outShader = currentShader;
            return true;
        } else {
            ofLogError("shader") << "did not link";
            return false;
        }
    }
}


void ofxShadertoy::onKeyEvent(ofKeyEventArgs & ev) {
    keyboardTexturePixels[ev.scancode] = ev.Pressed ? 255 : 0;
}

void ofxShadertoy::setupShaderUniforms(ofShader const & bufShader, Buffer buffer) {
    bufShader.setUniform1f("iGlobalTime", globalTime);
    bufShader.setUniform1f("iTime", globalTime);
    bufShader.setUniform4f("iMouse", mousepos.x, mousepos.y, ofGetMousePressed()?1:0, 0);
    bufShader.setUniform3f("iResolution", dimensions.x, dimensions.y, 4.0f);
    bufShader.setUniform4f("iDate", ofGetYear(), ofGetMonth(), ofGetDay(), ((ofGetHours()*60+ofGetMinutes())*60)+ofGetSeconds());
    if(camera) {
        ofMatrix4x4 cmtx(camera->getOrientationQuat());
        cmtx.setTranslation(-camera->getPosition());
        bufShader.setUniformMatrix4f("tCameraMatrix", cmtx);
    } else {
        bufShader.setUniformMatrix4f("tCameraMatrix", ofMatrix4x4::newIdentityMatrix());
    }
    if(channels[(size_t)buffer][0].isAllocated()) {
        channels[(size_t)buffer][0].bind();
    }
    bufShader.setUniformTexture("iChannel0", channels[(size_t)buffer][0], 1);
    bufShader.setUniformTexture("iChannel1", channels[(size_t)buffer][1], 2);
    bufShader.setUniformTexture("iChannel2", channels[(size_t)buffer][2], 3);
    bufShader.setUniformTexture("iChannel3", channels[(size_t)buffer][3], 4);
}

void ofxShadertoy::applyBuffer(ofShader const & bufShader, Buffer buffer, ofFbo & bufFbo) {
    if ((bufFbo.getWidth() != (int)dimensions.x) ||
        (bufFbo.getHeight() != (int)dimensions.y)) {
        bufFbo.allocate((int)dimensions.x, (int)dimensions.y);
    }
    bufFbo.begin();
    bufShader.begin();

    setupShaderUniforms(bufShader, buffer);

    ofDrawRectangle(0, 0, dimensions.x, dimensions.y);

    bufShader.end();
    bufFbo.end();
}

ofxShadertoy::ofxShadertoy():globalTime(0),advanceTime(true),camera(NULL),dimensions(640.f, 480.f) {
    for (size_t buf = 0; buf < 6; buf++) {
        for (size_t chan = 0; chan < 4; chan++) {
            channels[buf][chan].allocate(1, 1, GL_RGBA);
        }
    }
    ofAddListener(ofEvents().update, this, &ofxShadertoy::update);
}
ofxShadertoy::~ofxShadertoy() {
    ofRemoveListener(ofEvents().update, this, &ofxShadertoy::update);
}

bool ofxShadertoy::load(string shaderfilename, Buffer buffer) {

    ofFile f;
    f.open(shaderfilename);
    ofBuffer b = f.readToBuffer();
    switch (buffer) {
    case Buffer::BufferA:
        return makeShader(b.getText(), bufAShader);
    case Buffer::BufferB:
        return makeShader(b.getText(), bufBShader);
    case Buffer::BufferC:
        return makeShader(b.getText(), bufCShader);
    case Buffer::BufferD:
        return makeShader(b.getText(), bufDShader);
    case Buffer::Sound:
        return makeShader(b.getText(), soundShader);
    case Buffer::Image:
    default:
        return makeShader(b.getText(), mainShader);
    }
    

}

void ofxShadertoy::begin() {
    // upload the keyboard texture (if it is loaded)
    if (keyboardTexturePixels.isAllocated()) {
        keyboardTexture.loadData(keyboardTexturePixels);
    }

    // prepare bufA if it was set up
    if (bufAShader.isLoaded()) {
        applyBuffer(bufAShader, Buffer::BufferA, bufAFbo);
    }

    // prepare bufB if it was set up
    if (bufBShader.isLoaded()) {
        applyBuffer(bufBShader, Buffer::BufferB, bufBFbo);
    }

    // prepare bufC if it was set up
    if (bufCShader.isLoaded()) {
        applyBuffer(bufCShader, Buffer::BufferC, bufCFbo);
    }

    // prepare bufD if it was set up
    if (bufDShader.isLoaded()) {
        applyBuffer(bufDShader, Buffer::BufferD, bufDFbo);
    }
    
    // set the main image shader
    mainShader.begin();
    setupShaderUniforms(mainShader, Buffer::Image);
}

void ofxShadertoy::end() const {
    mainShader.end();
}

void ofxShadertoy::draw(float x, float y, float z, float w, float h) {
    begin();
    ofDrawRectangle(x, y, z, w, h);
    end();
}

void ofxShadertoy::audioOut(ofSoundBuffer & buffer) {
    
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

void ofxShadertoy::setTexture(int index, const ofTexture& tex, Buffer buffer) {
    if (index > 4 || index < 0) return;
    channels[(size_t)buffer][index] = tex;
}

void ofxShadertoy::setTexture(int index, ofFbo const & tex, Buffer buffer) {
    if (index > 4 || index < 0) return;
    channels[(size_t)buffer][index] = tex.getTextureReference(0);
}

void ofxShadertoy::setTexture(int index, Buffer sourceBuffer, Buffer destBuffer) {
    if (index > 4 || index < 0) return;
    ofFbo& sourceFbo = bufAFbo;
    switch (sourceBuffer) {
    case Buffer::BufferB:
        sourceFbo = bufBFbo;
        break;
    case Buffer::BufferC:
        sourceFbo = bufCFbo;
        break;
    case Buffer::BufferD:
        sourceFbo = bufDFbo;
        break;
    case Buffer::Image:
    case Buffer::Sound:
        return;
    }
    setTexture(index, sourceFbo, destBuffer);
}

bool ofxShadertoy::makeBufferA(std::string const & text, ofTexture & outTexture) {
    if (!text.empty() && makeShader(text, bufAShader)) {
        bufAFbo.allocate(dimensions.x, dimensions.y);
        outTexture = bufAFbo.getTextureReference(0);
        return true;
    }
    else {
        if (bufAFbo.isAllocated()) {
            bufAFbo.destroy();
        }
    }
    return false;
}

bool ofxShadertoy::makeBufferB(std::string const & text, ofTexture & outTexture) {
    if (!text.empty() && makeShader(text, bufBShader)) {
        bufBFbo.allocate(dimensions.x, dimensions.y);
        outTexture = bufBFbo.getTextureReference(0);
        return true;
    }
    else {
        if (bufBFbo.isAllocated()) {
            bufBFbo.destroy();
        }
    }
    return false;
}

bool ofxShadertoy::makeBufferC(std::string const & text, ofTexture & outTexture) {
    if (!text.empty() && makeShader(text, bufCShader)) {
        bufCFbo.allocate(dimensions.x, dimensions.y);
        outTexture = bufCFbo.getTextureReference(0);
        return true;
    }
    else {
        if (bufCFbo.isAllocated()) {
            bufCFbo.destroy();
        }
    }
    return false;
}

bool ofxShadertoy::makeBufferD(std::string const & text, ofTexture & outTexture) {
    if (!text.empty() && makeShader(text, bufAShader)) {
        bufDFbo.allocate(dimensions.x, dimensions.y);
        outTexture = bufDFbo.getTextureReference(0);
        return true;
    }
    else {
        if (bufDFbo.isAllocated()) {
            bufDFbo.destroy();
        }
    }
    return false;
}

bool ofxShadertoy::makeKeyboardTexture(ofTexture & outTexture) {
    if (!keyboardTexturePixels.isAllocated()) {
        keyboardTexturePixels.allocate(256, 1, ofImageType::OF_IMAGE_GRAYSCALE);
        keyboardTexture.allocate(keyboardTexturePixels);
        ofAddListener(ofEvents().keyPressed, this, &ofxShadertoy::onKeyEvent);
        ofAddListener(ofEvents().keyReleased, this, &ofxShadertoy::onKeyEvent);
    }
    outTexture = keyboardTexture;
    return false;
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
    bufAShader.setUniform1i(uniformName, value);
    bufBShader.setUniform1i(uniformName, value);
    bufCShader.setUniform1i(uniformName, value);
    bufDShader.setUniform1i(uniformName, value);
    soundShader.setUniform1i(uniformName, value);
    mainShader.setUniform1i(uniformName, value);
}

void ofxShadertoy::setUniform2i(std::string const & uniformName, int v1, int v2) const {
    bufAShader.setUniform2i(uniformName, v1, v2);
    bufBShader.setUniform2i(uniformName, v1, v2);
    bufCShader.setUniform2i(uniformName, v1, v2);
    bufDShader.setUniform2i(uniformName, v1, v2);
    soundShader.setUniform2i(uniformName, v1, v2);
    mainShader.setUniform2i(uniformName, v1, v2);
}

void ofxShadertoy::setUniform3i(std::string const & uniformName, int v1, int v2, int v3) const {
    bufAShader.setUniform3i(uniformName, v1, v2, v3);
    bufBShader.setUniform3i(uniformName, v1, v2, v3);
    bufCShader.setUniform3i(uniformName, v1, v2, v3);
    bufDShader.setUniform3i(uniformName, v1, v2, v3);
    soundShader.setUniform3i(uniformName, v1, v2, v3);
    mainShader.setUniform3i(uniformName, v1, v2, v3);
}

void ofxShadertoy::setUniform4i(std::string const & uniformName, int v1, int v2, int v3, int v4) const {
    bufAShader.setUniform4i(uniformName, v1, v2, v3, v4);
    bufBShader.setUniform4i(uniformName, v1, v2, v3, v4);
    bufCShader.setUniform4i(uniformName, v1, v2, v3, v4);
    bufDShader.setUniform4i(uniformName, v1, v2, v3, v4);
    soundShader.setUniform4i(uniformName, v1, v2, v3, v4);
    mainShader.setUniform4i(uniformName, v1, v2, v3, v4);
}

void ofxShadertoy::setUniform1f(std::string const & uniformName, float value) const {
    bufAShader.setUniform1f(uniformName, value);
    bufBShader.setUniform1f(uniformName, value);
    bufCShader.setUniform1f(uniformName, value);
    bufDShader.setUniform1f(uniformName, value);
    soundShader.setUniform1f(uniformName, value);
    mainShader.setUniform1f(uniformName, value);
}

void ofxShadertoy::setUniform2f(std::string const & uniformName, float v1, float v2) const {
    bufAShader.setUniform2f(uniformName, v1, v2);
    bufBShader.setUniform2f(uniformName, v1, v2);
    bufCShader.setUniform2f(uniformName, v1, v2);
    bufDShader.setUniform2f(uniformName, v1, v2);
    soundShader.setUniform2f(uniformName, v1, v2);
    mainShader.setUniform2f(uniformName, v1, v2);
}

void ofxShadertoy::setUniform2f(std::string const & uniformName, ofVec2f const & value) const {
    setUniform2f(uniformName, value.x, value.y);
}

void ofxShadertoy::setUniform3f(std::string const & uniformName, float v1, float v2, float v3) const {
    bufAShader.setUniform3f(uniformName, v1, v2, v3);
    bufBShader.setUniform3f(uniformName, v1, v2, v3);
    bufCShader.setUniform3f(uniformName, v1, v2, v3);
    bufDShader.setUniform3f(uniformName, v1, v2, v3);
    soundShader.setUniform3f(uniformName, v1, v2, v3);
    mainShader.setUniform3f(uniformName, v1, v2, v3);
}

void ofxShadertoy::setUniform3f(std::string const & uniformName, ofVec3f const & value) const {
    setUniform3f(uniformName, value.x, value.y, value.z);
}

void ofxShadertoy::setUniform4f(std::string const & uniformName, float v1, float v2, float v3, float v4) const {
    bufAShader.setUniform4f(uniformName, v1, v2, v3, v4);
    bufBShader.setUniform4f(uniformName, v1, v2, v3, v4);
    bufCShader.setUniform4f(uniformName, v1, v2, v3, v4);
    bufDShader.setUniform4f(uniformName, v1, v2, v3, v4);
    soundShader.setUniform4f(uniformName, v1, v2, v3, v4);
    mainShader.setUniform4f(uniformName, v1, v2, v3, v4);
}

void ofxShadertoy::setUniform4f(std::string const & uniformName, ofVec4f const & value) const {
    bufAShader.setUniform4f(uniformName, value);
    bufBShader.setUniform4f(uniformName, value);
    bufCShader.setUniform4f(uniformName, value);
    bufDShader.setUniform4f(uniformName, value);
    soundShader.setUniform4f(uniformName, value);
    mainShader.setUniform4f(uniformName, value);
}

void ofxShadertoy::setUniform4f(std::string const & uniformName, ofFloatColor const & value) const {
    bufAShader.setUniform4f(uniformName, value);
    bufBShader.setUniform4f(uniformName, value);
    bufCShader.setUniform4f(uniformName, value);
    bufDShader.setUniform4f(uniformName, value);
    soundShader.setUniform4f(uniformName, value);
    mainShader.setUniform4f(uniformName, value);
}

void ofxShadertoy::setUniform1iv(std::string const & uniformName, int const * v, size_t count) const {
    bufAShader.setUniform1iv(uniformName, v, count);
    bufBShader.setUniform1iv(uniformName, v, count);
    bufCShader.setUniform1iv(uniformName, v, count);
    bufDShader.setUniform1iv(uniformName, v, count);
    soundShader.setUniform1iv(uniformName, v, count);
    mainShader.setUniform1iv(uniformName, v, count);
}

void ofxShadertoy::setUniform2iv(std::string const & uniformName, int const * v, size_t count) const {
    bufAShader.setUniform2iv(uniformName, v, count);
    bufBShader.setUniform2iv(uniformName, v, count);
    bufCShader.setUniform2iv(uniformName, v, count);
    bufDShader.setUniform2iv(uniformName, v, count);
    soundShader.setUniform2iv(uniformName, v, count);
    mainShader.setUniform2iv(uniformName, v, count);
}

void ofxShadertoy::setUniform3iv(std::string const & uniformName, int const * v, size_t count) const {
    bufAShader.setUniform3iv(uniformName, v, count);
    bufBShader.setUniform3iv(uniformName, v, count);
    bufCShader.setUniform3iv(uniformName, v, count);
    bufDShader.setUniform3iv(uniformName, v, count);
    soundShader.setUniform3iv(uniformName, v, count);
    mainShader.setUniform3iv(uniformName, v, count);
}

void ofxShadertoy::setUniform4iv(std::string const & uniformName, int const * v, size_t count) const {
    bufAShader.setUniform4iv(uniformName, v, count);
    bufBShader.setUniform4iv(uniformName, v, count);
    bufCShader.setUniform4iv(uniformName, v, count);
    bufDShader.setUniform4iv(uniformName, v, count);
    soundShader.setUniform4iv(uniformName, v, count);
    mainShader.setUniform4iv(uniformName, v, count);
}

void ofxShadertoy::setUniform1fv(std::string const & uniformName, float const * v, size_t count) const {
    bufAShader.setUniform1fv(uniformName, v, count);
    bufBShader.setUniform1fv(uniformName, v, count);
    bufCShader.setUniform1fv(uniformName, v, count);
    bufDShader.setUniform1fv(uniformName, v, count);
    soundShader.setUniform1fv(uniformName, v, count);
    mainShader.setUniform1fv(uniformName, v, count);
}

void ofxShadertoy::setUniform2fv(std::string const & uniformName, float const * v, size_t count) const {
    bufAShader.setUniform2fv(uniformName, v, count);
    bufBShader.setUniform2fv(uniformName, v, count);
    bufCShader.setUniform2fv(uniformName, v, count);
    bufDShader.setUniform2fv(uniformName, v, count);
    soundShader.setUniform2fv(uniformName, v, count);
    mainShader.setUniform2fv(uniformName, v, count);
}

void ofxShadertoy::setUniform3fv(std::string const & uniformName, float const * v, size_t count) const {
    bufAShader.setUniform3fv(uniformName, v, count);
    bufBShader.setUniform3fv(uniformName, v, count);
    bufCShader.setUniform3fv(uniformName, v, count);
    bufDShader.setUniform3fv(uniformName, v, count);
    soundShader.setUniform3fv(uniformName, v, count);
    mainShader.setUniform3fv(uniformName, v, count);
}

void ofxShadertoy::setUniform4fv(std::string const & uniformName, float const * v, size_t count) const {
    bufAShader.setUniform4fv(uniformName, v, count);
    bufBShader.setUniform4fv(uniformName, v, count);
    bufCShader.setUniform4fv(uniformName, v, count);
    bufDShader.setUniform4fv(uniformName, v, count);
    soundShader.setUniform4fv(uniformName, v, count);
    mainShader.setUniform4fv(uniformName, v, count);
}

void ofxShadertoy::setUniforms(ofParameterGroup const & parameters) const {
    bufAShader.setUniforms(parameters);
    bufBShader.setUniforms(parameters);
    bufCShader.setUniforms(parameters);
    bufDShader.setUniforms(parameters);
    soundShader.setUniforms(parameters);
    mainShader.setUniforms(parameters);
}

void ofxShadertoy::setUniformMatrix3f(std::string const & uniformName, ofMatrix3x3 const & matrix) const {
    bufAShader.setUniformMatrix3f(uniformName, matrix);
    bufBShader.setUniformMatrix3f(uniformName, matrix);
    bufCShader.setUniformMatrix3f(uniformName, matrix);
    bufDShader.setUniformMatrix3f(uniformName, matrix);
    soundShader.setUniformMatrix3f(uniformName, matrix);
    mainShader.setUniformMatrix3f(uniformName, matrix);
}

void ofxShadertoy::setUniformMatrix4f(std::string const & uniformName, ofMatrix4x4 const & matrix) const {
    bufAShader.setUniformMatrix4f(uniformName, matrix);
    bufBShader.setUniformMatrix4f(uniformName, matrix);
    bufCShader.setUniformMatrix4f(uniformName, matrix);
    bufDShader.setUniformMatrix4f(uniformName, matrix);
    soundShader.setUniformMatrix4f(uniformName, matrix);
    mainShader.setUniformMatrix4f(uniformName, matrix);
}

void ofxShadertoy::setUniformTexture(std::string const & uniform, ofTexture const & texture, int location) const {
    bufAShader.setUniformTexture(uniform, texture, location);
    bufBShader.setUniformTexture(uniform, texture, location);
    bufCShader.setUniformTexture(uniform, texture, location);
    bufDShader.setUniformTexture(uniform, texture, location);
    soundShader.setUniformTexture(uniform, texture, location);
    mainShader.setUniformTexture(uniform, texture, location);
}





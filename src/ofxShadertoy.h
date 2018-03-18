//
//  ofxShadertoy.h
//  Addon for loading and displaying www.shadertoy.com shaders
//
//  Created by @tiagosr on 6/26/15.
//
//

#pragma once
#include "ofMain.h"

class ofxShadertoy: public ofBaseDraws {
protected:
    ofShader shader;
    ofTexture channel0, channel1, channel2, channel3;
    bool useMouse, advanceTime;
    ofVec2f mousepos;
    ofVec2f dimensions;
    ofCamera *camera;
    double globalTime;
public:
    ofxShadertoy();
    virtual ~ofxShadertoy();
    bool load(string shaderfilename,
              bool chan0cube = false,
              bool chan1cube = false,
              bool chan2cube = false,
              bool chan3cube = false);
    virtual void begin() const;
    virtual void end() const;
    virtual void update(ofEventArgs&);
    virtual void setUseMouse(bool);
    virtual void setAdvanceTime(bool);
    virtual void draw(float x, float y) const { draw(x, y, 0, dimensions.x, dimensions.y); }
    virtual void draw(float x, float y, float w, float h) const { draw(x, y, 0, w, h); }
    virtual void draw(float x, float y, float z, float w, float h) const;
    virtual float getWidth() const { return dimensions.x; }
    virtual float getHeight() const { return dimensions.y; }
    virtual void setWidth(float w) { dimensions.x = w; }
    virtual void setHeight(float h) { dimensions.y = h; }
    virtual void setDimensions(float w, float h) { setDimensions(ofVec2f(w, h)); }
    virtual void setDimensions(ofVec2f const & v) { dimensions = v; }
    virtual void setTexture(int index, ofTexture const & tex);
    virtual void setTexture(int index, ofFbo const & tex);

    static ofTexture createGrayNoise(size_t size);
    static ofTexture createGrayNoiseSmall();
    static ofTexture createGrayNoiseMedium();
    static ofTexture createGrayNoiseLarge();


    static ofTexture createRGBANoise(size_t size);
    static ofTexture createRGBANoiseSmall();
    static ofTexture createRGBANoiseMedium();
    static ofTexture createRGBANoiseLarge();

    // extensions

    virtual void setCamera(ofCamera*);

    virtual void setUniform1i(std::string const & uniformName, int value) const;
    virtual void setUniform2i(std::string const & uniformName, int v1, int v2) const;
    virtual void setUniform3i(std::string const & uniformName, int v1, int v2, int v3) const;
    virtual void setUniform4i(std::string const & uniformName, int v1, int v2, int v3, int v4) const;
    virtual void setUniform1f(std::string const & uniformName, float value) const;
    virtual void setUniform2f(std::string const & uniformName, float v1, float v2) const;
    virtual void setUniform2f(std::string const & uniformName, ofVec2f const & value) const;
    virtual void setUniform3f(std::string const & uniformName, float v1, float v2, float v3) const;
    virtual void setUniform3f(std::string const & uniformName, ofVec3f const & value) const;
    virtual void setUniform4f(std::string const & uniformName, float v1, float v2, float v3, float v4) const;
    virtual void setUniform4f(std::string const & uniformName, ofVec4f const & value) const;
    virtual void setUniform4f(std::string const & uniformName, ofFloatColor const & value) const;

    virtual void setUniform1iv(std::string const & uniformName, int const * v, size_t count) const;
    virtual void setUniform2iv(std::string const & uniformName, int const * v, size_t count) const;
    virtual void setUniform3iv(std::string const & uniformName, int const * v, size_t count) const;
    virtual void setUniform4iv(std::string const & uniformName, int const * v, size_t count) const;

    virtual void setUniform1fv(std::string const & uniformName, float const * v, size_t count) const;
    virtual void setUniform2fv(std::string const & uniformName, float const * v, size_t count) const;
    virtual void setUniform3fv(std::string const & uniformName, float const * v, size_t count) const;
    virtual void setUniform4fv(std::string const & uniformName, float const * v, size_t count) const;

    virtual void setUniforms(ofParameterGroup const & parameters) const;

    virtual void setUniformMatrix3f(std::string const & uniformName, ofMatrix3x3 const & matrix) const;
    virtual void setUniformMatrix4f(std::string const & uniformName, ofMatrix4x4 const & matrix) const;
};

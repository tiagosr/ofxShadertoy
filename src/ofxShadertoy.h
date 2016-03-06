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
    virtual void setCamera(ofCamera*);
    virtual void draw(float x, float y) const { draw(x, y, 0, dimensions.x, dimensions.y); }
    virtual void draw(float x, float y, float w, float h) const { draw(x, y, 0, w, h); }
    virtual void draw(float x, float y, float z, float w, float h) const;
    virtual float getWidth() const { return dimensions.x; }
    virtual float getHeight() const { return dimensions.y; }
    virtual void setWidth(float w) { dimensions.x = w; }
    virtual void setHeight(float h) { dimensions.y = h; }
    virtual void setTexture(int index, const ofTexture& tex);
};

//
//  ofxShadertoy.h
//  RayWarper
//
//  Created by Tiago Rezende on 6/26/15.
//
//

#ifndef __RayWarper__ofxShadertoy__
#define __RayWarper__ofxShadertoy__

#include "ofMain.h"

class ofxShadertoy: public ofBaseDraws {
protected:
    ofShader shader;
    ofTexture channel0, channel1, channel2, channel3;
    bool useMouse, advanceTime;
    ofVec4f mousepos;
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
    virtual void begin();
    virtual void end();
    virtual void update(ofEventArgs&);
    virtual void mouseMoved(ofMouseEventArgs&);
    virtual void mouseDragged(ofMouseEventArgs&);
    virtual void setUseMouse(bool);
    virtual void setAdvanceTime(bool advance) { advanceTime = advance; }
    virtual void draw(float x, float y, float z, float w, float h);
};

#endif /* defined(__RayWarper__ofxShadertoy__) */

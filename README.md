# ofxShadertoy

openFrameworks addon for loading and displaying shaders from the [Shadertoy](https://www.shadertoy.com) page.  
Shadertoy is (c) iq  
This addon is distributed under the terms of the MIT License.

## Usage

1. Set up your project to use the programmable shader.
   in the `int main(...)` function, before setting up openGL or the app, use:
   ```c++
   ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
   ```

2. Set up the ofxShadertoy to load a file with the shadertoy glsl code
   ```c++
   ofxShadertoy shadertoy;
   ...
   shadertoy.load("shadertoy/raymarch.frag");
   ```

3. Set up textures if needed by the shader
   ```c++
   shadertoy.setTexture(0, nifty_texture);
   ```

4. In your draw() routine, draw a quad (or use begin() and end() to set up the shaders for a mesh)
   Effects happen in screen space.

## Extensions

I've set up some extensions to the shader parameters, mostly relating to using a camera (for use, non-exclusively, with another one of my addons, ofxFPSControls). The new uniforms are:

`uniform mat4 tCameraMatrix;` - a camera matrix that can be set up using `ofCamera` or any child class (e.g. [ofxFPSControls](https://github.com/tiagosr/ofxFPSControls))

//
//  FacesApp.h
//  BaseApp
//
//  Created by Mark van de Korput on 04/05/15.
//
//

#ifndef __BaseApp__FacesApp__
#define __BaseApp__FacesApp__

#include "ofMain.h"
#include "ofxOculusDK2.h"

class FacesApp {
    
public:
    FacesApp();
    void defineParams();
    void setup();
    void update(float dt);
    void draw();
    void paramsUpdated();
    void drawScene();
    void keyPressed(int key);
    void dragEvent(ofDragInfo &dragInfo);

public:
    bool enabled;
    
protected:
    ofxOculusDK2		oculusRift;
    ofEasyCam			cam;
    // ofSoundPlayer soundPlayer;
    ofFbo fbo;
    ofShader maskShader;
};


#endif /* defined(__BaseApp__FacesApp__) */

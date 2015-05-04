//
//  SoundBallsApp.h
//  BaseApp
//
//  Created by Mark van de Korput on 04/05/15.
//
//

#ifndef __BaseApp__SoundBallsApp__
#define __BaseApp__SoundBallsApp__

#include "ofMain.h"

#include "ofxOculusDK2.h"

typedef struct{
    ofColor color;
    ofVec3f pos;
    ofVec3f floatPos;
    float radius;
    bool bMouseOver;
    bool bGazeOver;
} DemoSphere;


class SoundBallsApp {
    
public:
    SoundBallsApp();
    void defineParams();
    void setup();
    void update(float dt);
    void draw();

    void drawScene();
    void keyPressed(int key);
    void dragEvent(ofDragInfo &dragInfo);

public:
    bool enabled;

protected:
    bool loadAudio(string path);
    ofxOculusDK2		oculusRift;
    
    ofLight				light;
    ofEasyCam			cam;
    bool showOverlay;
    bool predictive;
    vector<DemoSphere> demos;
    
    ofVec3f cursor2D;
    ofVec3f cursor3D;
    
    ofVec3f cursorRift;
    ofVec3f demoRift;
    
    ofVec3f cursorGaze;
    
    ofSoundPlayer soundPlayer;
    float playbackSpeed;
};

#endif /* defined(__BaseApp__SoundBallsApp__) */

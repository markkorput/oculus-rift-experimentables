//
//  SoundBallsApp.cpp
//  BaseApp
//
//  Created by Mark van de Korput on 04/05/15.
//
//

#include "SoundBallsApp.h"

#include "ofxSuperLog.h"
//#include "ofxTimeMeasurements.h"
#include "ofxRemoteUIVars.h"

SoundBallsApp::SoundBallsApp(){
    enabled = false;
    showOverlay = false;
    predictive = true;
}

void SoundBallsApp::defineParams(){
    RUI_NEW_GROUP("SoundBalls");
    RUI_DEFINE_VAR_WV(string, "audio-path", "");
}

void SoundBallsApp::setup(){
    oculusRift.baseCamera = &cam;
    oculusRift.setup();

    for(int i = 0; i < 20; i++){
        DemoSphere d;
        d.color = ofColor(ofRandom(255),
                          ofRandom(255),
                          ofRandom(255));
        
        d.pos = ofVec3f(ofRandom(-500, 500),0,ofRandom(-500,500));
        
        d.floatPos.x = d.pos.x;
        d.floatPos.z = d.pos.z;
        
        d.radius = ofRandom(2, 50);
        
        d.bMouseOver = false;
        d.bGazeOver  = false;
        
        demos.push_back(d);
    }
    
    //enable mouse;
    cam.begin();
    cam.end();
    
    playbackSpeed = 1.0;
    
    if(RUI_VAR(string, "audio-path") != ""){
        //{, "audio/weedflute_mac.wav");
        loadAudio(RUI_VAR(string, "audio-path"));
    }
}

void SoundBallsApp::update(float dt){
    for(int i = 0; i < demos.size(); i++){
        demos[i].floatPos.y = ofSignedNoise(ofGetElapsedTimef()/10.0,
                                            demos[i].pos.x/100.0,
                                            demos[i].pos.z/100.0,
                                            demos[i].radius*100.0) * demos[i].radius*20.;
        
    }
    
    if(oculusRift.isSetup()){
        ofRectangle viewport = oculusRift.getOculusViewport();
        for(int i = 0; i < demos.size(); i++){
            // mouse selection
            float mouseDist = oculusRift.distanceFromMouse(demos[i].floatPos);
            demos[i].bMouseOver = (mouseDist < 50);
            
            // gaze selection
            ofVec3f screenPos = oculusRift.worldToScreen(demos[i].floatPos, true);
            float gazeDist = ofDist(screenPos.x, screenPos.y,
                                    viewport.getCenter().x, viewport.getCenter().y);
            demos[i].bGazeOver = (gazeDist < 25);
        }
        
        
        ofQuaternion quat = oculusRift.getOrientationQuat();
        playbackSpeed = 1 + quat.x() / PI * 16.0;
        if(playbackSpeed < 0.2) playbackSpeed = 0.2;
        soundPlayer.setSpeed(playbackSpeed);
    }
}

void SoundBallsApp::draw(){
    ofBackgroundGradient(ofColor(120), ofColor(170));
    
    if(oculusRift.isSetup()){
        
        if(showOverlay){
            
            oculusRift.beginOverlay(-230, 320,240);
            ofRectangle overlayRect = oculusRift.getOverlayRectangle();
            
            ofPushStyle();
            ofEnableAlphaBlending();
            ofFill();
            ofSetColor(255, 40, 10, 200);
            
            ofRect(overlayRect);
            
            ofSetColor(255,255);
            ofFill();
            ofDrawBitmapString("ofxOculusRift by\nAndreas Muller\nJames George\nJason Walters\nElie Zananiri\nFPS:"+ofToString(ofGetFrameRate())+"\nPredictive Tracking " + (oculusRift.getUsePredictiveOrientation() ? "YES" : "NO"), 40, 40);
            
            ofSetColor(0, 255, 0);
            ofNoFill();
            ofCircle(overlayRect.getCenter(), 20);
            
            ofPopStyle();
            oculusRift.endOverlay();
        }
        
        ofSetColor(255);
        glEnable(GL_DEPTH_TEST);
        
        
        oculusRift.beginLeftEye();
        drawScene();
        oculusRift.endLeftEye();
        
        oculusRift.beginRightEye();
        drawScene();
        oculusRift.endRightEye();
        
        
        oculusRift.draw();
        
        glDisable(GL_DEPTH_TEST);
    }
    else{
        cam.begin();
        drawScene();
        cam.end();
    }
}

void SoundBallsApp::drawScene(){
    ofPushMatrix();
    ofRotate(90, 0, 0, -1);
    ofDrawGridPlane(500.0f, 10.0f, false );
    ofPopMatrix();
    
    ofPushStyle();
    ofNoFill();
    for(int i = 0; i < demos.size(); i++){
        ofPushMatrix();
        //		ofRotate(ofGetElapsedTimef()*(50-demos[i].radius), 0, 1, 0);
        ofTranslate(demos[i].floatPos);
        //		ofRotate(ofGetElapsedTimef()*4*(50-demos[i].radius), 0, 1, 0);
        
        if (demos[i].bMouseOver)
            ofSetColor(ofColor::white.getLerped(ofColor::red, sin(ofGetElapsedTimef()*10.0)*.5+.5));
        else if (demos[i].bGazeOver)
            ofSetColor(ofColor::white.getLerped(ofColor::green, sin(ofGetElapsedTimef()*10.0)*.5+.5));
        else
            ofSetColor(demos[i].color);
        
        ofSphere(demos[i].radius);
        ofPopMatrix();
    }
    
    
    
    //billboard and draw the mouse
    if(oculusRift.isSetup()){
        
        ofPushMatrix();
        oculusRift.multBillboardMatrix();
        ofSetColor(255, 0, 0);
        ofCircle(0,0,.5);
        ofPopMatrix();
        
    }
    
    ofPopStyle();
}

void SoundBallsApp::keyPressed(int key){
    switch(key){
        case 'a': soundPlayer.setVolume(soundPlayer.getVolume() == 0 ? 1.0 : 0.0);
        case 's': oculusRift.reloadShader(); break;
        case 'l': oculusRift.lockView = !oculusRift.lockView; break;
        case 'o': showOverlay = !showOverlay; break;
        case 'r': oculusRift.reset(); break;
        case 'h': ofHideCursor(); break;
        case 'H': ofShowCursor(); break;
        case 'p':
            predictive = !predictive;
            oculusRift.setUsePredictedOrientation(predictive);
            break;
    }
    
    // sound manipulators
    
    if(soundPlayer.isLoaded()){
        switch(key){
            case '.':
                playbackSpeed = ofRandom(0.1, 5.0);
                soundPlayer.setSpeed(playbackSpeed);
        }
    }
}

void SoundBallsApp::dragEvent(ofDragInfo &dragInfo){
    if(loadAudio(dragInfo.files[0])){
        // if dragged file is succesfully loaded, store its path in our config param so it's loaded automatically next time
        OFX_REMOTEUI_SERVER_SET_VAR(string, "audio-path", dragInfo.files[0]);
    }
}

bool SoundBallsApp::loadAudio(string path){
    LOG << "Loading audio file: " << path;
    if(!soundPlayer.loadSound(path)) return false;
    soundPlayer.setLoop(true);
    soundPlayer.setSpeed(playbackSpeed);
    soundPlayer.play();
    return true;
}
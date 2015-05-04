#include "ofApp.h"

#include "ofxSuperLog.h"
#include "ofxTimeMeasurements.h"
#include "ofxRemoteUIVars.h"

//--------------------------------------------------------------
// callback function for ofxRemoteUI actions
bool bRecalcNow = true;

void ruiServerCallback(RemoteUIServerCallBackArg arg){
    switch (arg.action) {
        case CLIENT_DID_SET_PRESET:
        case CLIENT_UPDATED_PARAM:
            bRecalcNow = true;
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::setup(){
    // ofSetFrameRate(60);
    // ofEnableAlphaBlending();
    //    ofSetWindowPosition(1920, 0);
    //    ofToggleFullscreen();
    ofBackground(0);
    ofSetLogLevel( OF_LOG_VERBOSE );
    ofSetVerticalSync( true );
    ofLogToFile("log.txt", true);
    ofHideCursor();

    TIME_SAMPLE_SET_FRAMERATE(60);
    // TIME_SAMPLE_ENABLE();

    soundBallsApp = new SoundBallsApp();
    soundBallsApp->enabled = true;
    
    // setup ofxRemoteUI
    RUI_SETUP();
    RUI_SET_CALLBACK(ruiServerCallback);
    // setup params here
    RUI_NEW_GROUP("App");
    RUI_DEFINE_VAR_WV(bool, "app-fullscreen", false);
    if(soundBallsApp)
        soundBallsApp->defineParams();
    RUI_LOAD_FROM_XML();

    if(soundBallsApp && soundBallsApp->enabled)
        soundBallsApp->setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    float dt = (1.f/60.f); // elapsed time (60 fps)

    if(bRecalcNow){
        // TODO: perform post-param-change updates
        ofSetFullscreen(RUI_VAR(bool, "app-fullscreen"));
        
    }

    if(soundBallsApp && soundBallsApp->enabled)
        soundBallsApp->update(dt);
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(soundBallsApp && soundBallsApp->enabled)
        soundBallsApp->draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(soundBallsApp && soundBallsApp->enabled)
        soundBallsApp->keyPressed(key);

    switch(key){
        case 'f':
            // ofToggleFullscreen();
            OFX_REMOTEUI_SERVER_SET_VAR(bool, "app-fullscreen", !RUI_VAR(bool, "app-fullscreen"));
            bRecalcNow = true;
            break;
    }
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    if(soundBallsApp && soundBallsApp->enabled)
        soundBallsApp->dragEvent(dragInfo);
}
//
//  FacesApp.cpp
//  BaseApp
//
//  Created by Mark van de Korput on 04/05/15.
//
//

#include "FacesApp.h"

#include "ofxSuperLog.h"
//#include "ofxTimeMeasurements.h"
#include "ofxRemoteUIVars.h"


FacesApp::FacesApp(){
    enabled = false;
}

void FacesApp::defineParams(){
    RUI_NEW_GROUP("Faces");
    // RUI_DEFINE_VAR_WV(string, "audio-path", "");
}

void FacesApp::setup(){
    //oculusRift.baseCamera = &cam;
    oculusRift.lockView = true;
    oculusRift.setup();
   
    //enable mouse;
    cam.begin();
    cam.end();
}

void FacesApp::update(float dt){
}

void FacesApp::draw(){
    ofBackground(0);

    if(oculusRift.isSetup()){
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

void FacesApp::drawScene(){

}

void FacesApp::keyPressed(int key){
    switch(key){
        case 's':
            break;
    }
}

void FacesApp::dragEvent(ofDragInfo &dragInfo){
//    if(loadAudio(dragInfo.files[0])){
//        OFX_REMOTEUI_SERVER_SET_VAR(string, "file-path", dragInfo.files[0]);
//    }
}
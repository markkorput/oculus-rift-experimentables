//
//  FaceCollector.cpp
//  BaseApp
//
//  Created by Mark van de Korput on 09/04/15.
//
//

#include "FaceCollector.h"
#include "ofxRemoteUIVars.h"

FaceCollector::FaceCollector(){
    width = 640;
    height = 460;
    setup();
}

void FaceCollector::setupParams(){
    RUI_NEW_GROUP("FaceCollector");
    RUI_DEFINE_VAR_WV(float, "FaceCollector::delay", 0.5, 0.0, 3.);
    RUI_DEFINE_VAR_WV(string, "FaceCollector::xml_path", "haarcascade_frontalface_default.xml");
}

void FaceCollector::setup(){
    vidGrabber.initGrabber(width, height);
    colorImage.allocate(width, height);
    grayImage.allocate(width, height);
    grayImage.setUseTexture(false);
    string xmlPath = RUI_VAR(string, "FaceCollector::xml_path");
    if(xmlPath != "")
        finder.setup(xmlPath,
                     false // don't use texture, so the finder we can run
                            // in a different thread
                     );
}

void FaceCollector::update(float dt){
    vidGrabber.update();

    if(!vidGrabber.isFrameNew())
        return;

    colorImage.setFromPixels(vidGrabber.getPixels(), width, height);
    grayImage = colorImage;

    if(delayAnim.isAnimating()){
        delayAnim.update(dt);
        return;
    }

    startThread(false);

    // restart delay
    delayAnim.setDuration(RUI_VAR(float, "FaceCollector::delay"));
    delayAnim.animateFromTo(0., 1.);
}

void FaceCollector::draw(){
    colorImage.draw(0,0);
    if(!lock()) return;
    for(int i = 0; i < blobs.size(); i++) {
        ofxCvBlob blob = blobs[i];
        //printf("found: %i, %i %ix%i\n",
        //       cur.x, cur.y, cur.width, cur.height);
        ofSetColor(255);
        ofNoFill();
        ofRect(blob.boundingRect);
    }
    unlock();
}

void FaceCollector::threadedFunction(){
    finder.findHaarObjects(grayImage); // an ofxCvGrayscaleImage
    while(!lock())
        sleep(10);
    blobs = finder.blobs;
    unlock();
}
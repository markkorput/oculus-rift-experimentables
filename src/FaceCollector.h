//
//  FaceCollector.h
//  BaseApp
//
//  Created by Mark van de Korput on 09/04/15.
//
//

#ifndef __BaseApp__FaceCollector__
#define __BaseApp__FaceCollector__

#include "ofMain.h"
#include "ofxCvHaarFinder.h"
#include "ofxAnimatableFloat.h"

#define FACECOLLECTOR FaceCollector::one()

class FaceCollector : public ofThread {
public:
    FaceCollector();
    // ~FaceCollector();
    
public: // singleton instance stuff
    // generally only one instance of this class is needed
    // this instance can be automatically created and its reference an be
    // obtained by calling FaceCollector::one()
    static FaceCollector& one()
    {
        static FaceCollector instance;
        // Instantiated on first use.
        return instance;
    }

public:
    void setupParams();
    void setup();
    void update(float dt);
    void draw();

    void threadedFunction();

protected: // attributes
    int width, height;
    ofVideoGrabber vidGrabber;
    ofxCvColorImage colorImage;
    ofxCvGrayscaleImage grayImage;
    ofxCvHaarFinder finder;
    ofxAnimatableFloat delayAnim;
    vector<ofxCvBlob> blobs;
};

#endif /* defined(__BaseApp__FaceCollector__) */

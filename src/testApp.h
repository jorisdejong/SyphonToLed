#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxDmx.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    ofxSyphonClient syphonInput;
    int w;
    int h;
    int oldW;
    int oldH;
    
    bool textureTooBig;
    bool connectedToEnttec;
    
    ofTexture tex;
    ofPixels pix;
    
    float opacity;
    
    ofxDmx dmx;
    string deviceName;
    //string packet;
    //ofSerial arduino;
    ofSerial deviceList;
    
    bool mouseDown;
    ofVec2f mousePos;
    ofVec2f startMousePos;
		
};

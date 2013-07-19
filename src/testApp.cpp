#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    syphonInput.setup();
    syphonInput.setApplicationName("Arena");
    syphonInput.setServerName("LED");
    
    //why can't I make this dynamic
    tex.allocate(32,1,GL_RGB); //don't need alpha
    //pix.allocate(8,8,3);
    
    //connect to the Enttec
    dmx.connect("tty.usbserial-EN099397", 96);
    
    //connect to the Arduino
    //arduino.setup(0,9600);
    

}

//--------------------------------------------------------------
void testApp::update(){
    
    w = syphonInput.getWidth();
    h = syphonInput.getHeight();
    //cout<<w<<","<<h<<endl;
    
   

}

//--------------------------------------------------------------
void testApp::draw(){
    
    
    //to do: how do I get the Syphon texture loaded as an ofTexture without loading and reading it from the screen
    ofSetColor(255,255,255);
    syphonInput.draw(0,0);
    tex.loadScreenData(0,0,w,h); 
    tex.readToPixels(pix);
    
    
    
    //crawl through the pixels, get their HEX and send it via serial
    for(int y = 0; y < h; y++){
        for(int x = 0; x < w; x++){
            
            //apparently pix.GetColor reads bottom left to top right, so we have to flip the y
            int hexCol = pix.getColor(x, h-y-1).getHex();   //color in Hex for the arduino
            ofColor col = pix.getColor(x, h-y-1); //color in ofColor for the Enttec
        
            int loc = pix.getPixelIndex(x, y)/3; //get the current pixelindex
            
            //set the Enttec channels
            dmx.setLevel(loc*3+1, col.r);
            dmx.setLevel(loc*3+2, col.g);
            dmx.setLevel(loc*3+3, col.b);

            //send the message to the Arduino
            /**
            string stringCol = ofToHex(hexCol); //get the HEX value of every pixel as a string
            packet="<"+ ofToString(loc)+"/"+stringCol.substr(2,6)+">"; // add the index, hexcolors + headers to the packet
            
            unsigned char* packetChar = (unsigned char*) packet.c_str();  //convert to unsigned char
            int l = strlen((char*)packetChar); //get the length
            
            int numSent = arduino.writeBytes(packetChar,l); //send the package, I don't think I need error handling here, as the Arduino will toss the message anyway if the last ">" doesn't arrive
            
            //cout<<l<<packet<<endl; //uncomment for debug
            **/

            
            ofSetHexColor(hexCol);
            int s = ofGetWidth()/(w+1);
            ofRect(x*s+10, y*s+10, s, s);
        }
    }
    
    //tell the Enttect to update
    dmx.update();
    
    //send a single byte to let the Arduino know it should display
    //arduino.writeByte('x');
 
}    
    
    
    
 


//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    syphonInput.setup();
    syphonInput.setApplicationName("Arena");
    syphonInput.setServerName("LED");
    
    opacity = 1.0;
        
    //get the serial device list
    vector <ofSerialDeviceInfo> devices = deviceList.getDeviceList();
    for(int i = 0; i < devices.size(); i++)
    {
        //cout<<devices[i].getDeviceName()<<endl;
        deviceName = devices[i].getDeviceName();
        if(deviceName.substr(0,3) == "tty" && deviceName.substr(14,2) == "EN")
        {
            //connect to the Enttec
            dmx.connect(deviceName, 96);
            connectedToEnttec = true;
            return;
            
        }
        else
            connectedToEnttec = false;

    }
    
    //connect to the Arduino
    //arduino.setup(0,9600);
    

}

//--------------------------------------------------------------
void testApp::update(){
    
    
    w = syphonInput.getWidth();
    h = syphonInput.getHeight();

    if(oldW != w || oldH != h)
    {
        
        tex.clear();
        tex.allocate(w,h,GL_RGB); //don't need alpha
        
        if(3*w*h>512)
        {
            dmx.setChannels(512);
            ofSetColor(255);
            textureTooBig = true;
        }
        else
        {
            dmx.setChannels(3*w*h);
            textureTooBig = false;
        }
        
        oldW = w;
        oldH = h;
    }
    //pix.allocate(8,8,3);

    
   

}

//--------------------------------------------------------------
void testApp::draw(){
    
    
    //to do: how do I get the Syphon texture loaded as an ofTexture without displaying and reading it from the screen
    ofSetColor(255,255,255);
    syphonInput.draw(0,0);
    tex.loadScreenData(0,0,w,h); 
    tex.readToPixels(pix);

    
    ofBackground(24,24,24);
    
    
    //crawl through the pixels, get their HEX and send it via serial
    for(int y = 0; y < h; y++){
        for(int x = 0; x < w; x++){
            
            //apparently pix.GetColor reads bottom left to top right, so we have to flip the y
            int hexCol = pix.getColor(x, h-y-1).getHex();   //color in Hex for the arduino
            ofColor col = pix.getColor(x, h-y-1); //color in ofColor for the Enttec
        
            int loc = pix.getPixelIndex(x, y)/3; //get the current pixelindex
            
            if(loc*3+3<=512) //clamp to 512 channels
            {
                //set the Enttec channels
                dmx.setLevel(loc*3+1, col.r * opacity);
                dmx.setLevel(loc*3+2, col.g * opacity);
                dmx.setLevel(loc*3+3, col.b * opacity);
            }

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
            int s;
            if(w>h)
                s = ofGetWidth()/(w+1);
            else
                s = ofGetHeight()/(h+1);
            ofRect(x*s+10, y*s+10, s, s);
        }
    }
    
    //tell the Enttect to update
    dmx.update();
    
    //draw "GUI"
    ofSetColor(64,64,64);
    int guiY = 658;
    ofRectRounded(10, guiY, 1004,100,10);
    
    if(textureTooBig)
    {
        ofSetColor(200,0,0);
        ofDrawBitmapString("That's too many DMX channels for this universe, so I'm only sending the first 512...", 20,guiY+70);
    }
    ofSetColor(128);
    if(connectedToEnttec)
        ofDrawBitmapString("Connected to Enttec, "+deviceName, 20, guiY+20);
    else
        ofDrawBitmapString("No Enttec devices found...", 20,guiY+20);
    
    //draw opacity slider GUI
    int sliderPosX = 900;
    int sliderPosY = guiY + 10;
    int sliderWidth = 30;
    int sliderHeight = 80;
    
    ofRect(sliderPosX, sliderPosY, sliderWidth, sliderHeight);
    
    ofSetColor(192);
    ofSetLineWidth(5);
    float opacityPos = 80 - opacity * 80;
    ofLine(sliderPosX, sliderPosY + opacityPos, sliderPosX + sliderWidth, sliderPosY + opacityPos);
    ofSetLineWidth(1);
    
    //opacity slider logic
    if(mouseDown)
    {
        if(startMousePos.x > sliderPosX && startMousePos.x < sliderPosX + sliderWidth && startMousePos.y > sliderPosY  && startMousePos.y < sliderPosY + sliderHeight)
        {
            opacity = 1.0 - (mousePos.y - sliderPosY) / 80 ;
            if(opacity<0)
                opacity=0;
            if(opacity>1.0)
                opacity=1.0;
        }
    }

    
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
    mouseDown = true;
    mousePos.set(x,y);

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    mouseDown = true;
    startMousePos.set(x,y);

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    mouseDown = false;
    mousePos.set(x,y);

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
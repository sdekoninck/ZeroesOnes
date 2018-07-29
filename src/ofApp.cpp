#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(255,255,255);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetOrientation(OF_ORIENTATION_90_LEFT);
	// this makes the camera directly return grayscale image, faster!
	grabber.setPixelFormat(OF_PIXELS_MONO);
	grabber.initGrabber(videoWidth, videoHeight);
	flowSolver.setup(grabber.getWidth(), grabber.getHeight(), 0.5, 3, 10, 1, 7, 1.5, false, false);
	gray.allocate(grabber.getWidth(),grabber.getHeight());

    for(int i=0; i<numberofsquares; i++){
        soundPlayer[i].loadSound(ofToString(i + 1) + "_reencoded.wav");
    }

    int currentY = 0;
    for(int i=0; i<numberofsquares; i++){
        yPosition[i] = currentY;
        currentY = currentY + 20;
    }

}

//--------------------------------------------------------------
void ofApp::update(){
	grabber.update();
	if(grabber.isFrameNew()){
		flowSolver.update(grabber);
		velocity = getVelocity();

		//ofLog(OF_LOG_NOTICE, "X="+ofToString(velocity.x));
		//ofLog(OF_LOG_NOTICE,"Y="+ofToString(velocity.y));

	}
	gray.setFromPixels(grabber.getPixels(),grabber.getWidth(),grabber.getHeight());
	gray.threshold(sensorThreshold*255);
	gray.resetROI();

    for(int i=0; i<numberofsquares; i++){
        sensorCount[i] = gray.countNonZeroInRegion(xPosition, yPosition[i], sensorWidth, sensorHeight);
    }
    ofLog(OF_LOG_NOTICE, "Searching fabric");
    	for(int i=0; i<numberofsquares; i++){
    	       if(sensorCount[i]  > (sensorWidth * sensorHeight) * sensorThreshold){

    	          if (soundPlayer[i].getIsPlaying() == false){
    	              soundPlayer[i].play();

    	              ofLog(OF_LOG_NOTICE, "Playing sound"+ofToString(i));

    	          }

    	       }

    	       else{

    	             soundPlayer[i].stop();
    	       }

    	       if( velocity.x > 100){
    	            soundPlayer[i].setSpeed(velocity.x / 100);

    	        }
    	        else{
    	            if(velocity.x < -100){
    	            soundPlayer[i].setSpeed(0);

    	            }
    	        }
    	}
}


//--------------------------------------------------------------
void ofApp::draw(){
	ofSetHexColor(0xFFFFFF);
	gray.draw(0, 0, ofGetWidth(), ofGetHeight());
//
//	ofSetColor(0);
//	ofRect(0,0,300,30);
//	ofSetColor(255);
//	ofDrawBitmapString("Sensor Threshold: "+ofToString(sensorThreshold*255), 10, 10);
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::touchDown(int x, int y, int id){
	sensorThreshold = ofMap(x, 0, ofGetWidth(), 0, 1);
}

void ofApp::touchUp(int x, int y, int id){

	sensorThreshold = ofMap(x, 0, ofGetWidth(), 0, 1);
}

ofPoint ofApp::getVelocity(float step){
	ofPoint p;

	for (int x=0; x < grabber.getWidth(); x += step)
	{
		for (int y=0; y < grabber.getHeight(); y += step)
		{
			p += flowSolver.getVelAtPixel(x, y);
		}
	}

	return p;
}


//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void ofApp::touchDoubleTap(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

//--------------------------------------------------------------
void ofApp::pause(){
}

//--------------------------------------------------------------
void ofApp::stop(){

}

//--------------------------------------------------------------
void ofApp::resume(){

}

//--------------------------------------------------------------
void ofApp::reloadTextures(){

}

//--------------------------------------------------------------
bool ofApp::backPressed(){
	return false;
}

//--------------------------------------------------------------
void ofApp::okPressed(){

}

//--------------------------------------------------------------
void ofApp::cancelPressed(){

}

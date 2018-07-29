#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofBackground(255,255,255);
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetOrientation(OF_ORIENTATION_90_LEFT);

    ofSetFrameRate(60);

	// this makes the camera directly return grayscale image, faster!
	grabber.setPixelFormat(OF_PIXELS_MONO);
	grabber.initGrabber(videoWidth, videoHeight);

#ifdef DO_OPTICAL_FLOW
	flowSolver.setup(grabber.getWidth(), grabber.getHeight(), 0.5, 3, 10, 1, 7, 1.5, false, false);
#endif

	gray.allocate(grabber.getWidth(),grabber.getHeight());


	for(int i=0; i<MAX_NUMBER_SQUARES; i++){
		if (soundPlayer[i].load(ofToString(i + 1) + "_reencoded.wav", false)) {
		} else {
			ofLogError() << "could not load file: " << ofToString(i + 1) << "_reencoded.wav";
		}
		soundPlayer[i].setVolume(0.8);
	}

	// change number of squares
	numberofsquares = 2;
	setVariables();

    lastUpdate = ofGetElapsedTimef();
}

void ofApp::setVariables() {

	sensorHeight = videoHeight / numberofsquares;

	int currentY = 0;
	for(int i=0; i<numberofsquares; i++){
		yPosition[i] = currentY;
		currentY = currentY + sensorHeight;
	}
}

//--------------------------------------------------------------
void ofApp::update(){

	grabber.update();

    if(!grabber.isFrameNew() ||
			grabber.getPixels().getWidth() <= 0 ||
            grabber.getPixels().getHeight() <= 0){
		return;
	}

#ifdef DO_OPTICAL_FLOW
    // enable this for optical flow
    flowSolver.update(grabber);
    velocity = getVelocity();

    //ofLog(OF_LOG_NOTICE, "X="+ofToString(velocity.x));
    //ofLog(OF_LOG_NOTICE,"Y="+ofToString(velocity.y));
#endif

	// set gray pixels
	gray.setFromPixels(grabber.getPixels());
	gray.threshold(sensorThreshold*255);
	gray.resetROI();

	float now = ofGetElapsedTimef();
	if (now - lastUpdate < repeatRate) {
		return;
	}

    lastUpdate = now;

	// play sound, if more than x white pixels
	ofLog(OF_LOG_NOTICE, "Searching fabric");
	for(int i=0; i<numberofsquares; i++){

		sensorCount[i] = gray.countNonZeroInRegion(xPosition, yPosition[i], sensorWidth, sensorHeight);

		if(sensorCount[i]  > (sensorWidth * sensorHeight) * sensorThreshold){

			if (continuous || soundPlayer[i].isPlaying() == false){
				soundPlayer[i].setPosition(0.0);
				soundPlayer[i].play();
				ofLog(OF_LOG_NOTICE, "Playing sound"+ofToString(i));
			}
		}
		else{
			soundPlayer[i].stop();
		}

#ifdef DO_OPTICAL_FLOW
		if( velocity.x > 100){
			soundPlayer[i].setSpeed(velocity.x / 100);

		}
		else if(velocity.x < -100){
			soundPlayer[i].setSpeed(0);
		}
#endif
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetHexColor(0xFFFFFF);
	//grabber.draw(0, 0);
	gray.draw(0, 0, ofGetWidth(), ofGetHeight());

	ofSetColor(255,0,0);
    ofSetLineWidth(1);
    ofNoFill();

	int x = (xPosition/(float)videoWidth)*ofGetWindowWidth();
	int w = (sensorWidth/(float)videoWidth)*ofGetWindowWidth();
	int h = (sensorHeight/(float)videoHeight)*ofGetWindowHeight();
	for(int i=0; i<numberofsquares; i++){
		ofDrawRectangle(x, (yPosition[i]/(float)videoHeight)*ofGetWindowHeight(), w, h);
	}


	ofSetColor(0,255,255);
	ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate()), 10, 10);

	// draw sensor and repeatreate marks
    ofFill();

	int sensor_y = ofGetHeight() - (ofGetHeight()*sensorThreshold)-3;
	ofDrawBitmapString(ofToString(sensorThreshold), 33, sensor_y);
    ofDrawRectangle(0, sensor_y, 30, 6);

	int repeat_x = (repeatRate / 2.f) * ofGetWidth() - 3;
	if (repeat_x < ofGetWidth()) {
		ofDrawBitmapString(ofToString(repeatRate), repeat_x, ofGetHeight() - 35);
		ofDrawRectangle(repeat_x, ofGetHeight() - 20, 6, 20);
	} else {
		ofDrawBitmapString(ofToString(repeatRate)+"\n--->", ofGetWidth() - 40, ofGetHeight() - 35);
	}

	// draw buttons
	ofEnableAlphaBlending();
	ofSetColor(255, 255, 0, 100);
	ofDrawRectangle(0, 0, BUTTON_WIDTH, BUTTON_HEIGHT);
	ofDrawRectangle(0, ofGetHeight() - BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT);
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

	if (x < BUTTON_WIDTH) {
		return;
	}

	//sensorThreshold = ofMap(x, 0, ofGetWidth(), 0, 1);
    touchdown.x = x;
	touchdown.y = y;
	touchdownThreshold = sensorThreshold;
    touchdownRepeatRate = repeatRate;
}

//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id){

	if (x < BUTTON_WIDTH) {
		return;
	}

	if (!dragging) {
		if (abs(touchdown.x - x) > DRAG_DISTANCE) {
			drawHorizontal = true;
			dragging = true;
		} else if (abs(touchdown.y - y) > DRAG_DISTANCE) {
			drawHorizontal = false;
			dragging = true;
		}
	}

	if (!dragging) {
		return;
	}

	if (drawHorizontal) {
		// set repeatrate
		float delta = ((touchdown.x - x) / ((float)ofGetWindowWidth() / 2.f));
		repeatRate = touchdownRepeatRate - delta;
		if (repeatRate < 0.001) {
			repeatRate = 0.001;
		}
	} else {
		// set sensor threshold
		float delta = (touchdown.y - y) / ((float)ofGetWindowHeight() / 1.f);
		sensorThreshold = touchdownThreshold + delta;
		if (sensorThreshold > 1.0) {
			sensorThreshold = 1.0;
		} else if (sensorThreshold < 0.0) {
			sensorThreshold = 0.0;
		}
	}
}

//--------------------------------------------------------------
void ofApp::touchUp(int x, int y, int id){

	if (x < BUTTON_WIDTH) {
		if (y < BUTTON_HEIGHT) {
			// up the separations
			numberofsquares++;
			if (numberofsquares > 12) {
				numberofsquares = 12;
			}
			setVariables();
		} else if (y > ofGetHeight() - 150) {
			// down the separations
			numberofsquares--;
			if (numberofsquares < 1) {
				numberofsquares = 1;
			}
			setVariables();
		}
	}

	dragging = false;
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(int x, int y, int id){
	sensorThreshold = DEFAULT_THRESHOLD;
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


ofPoint ofApp::getVelocity(float step){
	ofPoint p = {0,0};

#ifdef DO_OPTICAL_FLOW
	for (int x=0; x < grabber.getWidth(); x += step)
	{
		for (int y=0; y < grabber.getHeight(); y += step)
		{
			p += flowSolver.getVelAtPixel(x, y);
		}
	}
#endif

	return p;
}
#pragma once

#include "ofMain.h"
#include "ofxAndroid.h"

#include "ofxOpenCv.h"
#include "ofxOpticalFlowFarneback.h"

class ofApp : public ofxAndroidApp{
	
	public:
		
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void windowResized(int w, int h);

		void touchDown(int x, int y, int id);
		void touchMoved(int x, int y, int id);
		void touchUp(int x, int y, int id);
		void touchDoubleTap(int x, int y, int id);
		void touchCancelled(int x, int y, int id);
		void swipe(ofxAndroidSwipeDir swipeDir, int id);

		void pause();
		void stop();
		void resume();
		void reloadTextures();

		bool backPressed();
		void okPressed();
		void cancelPressed();

		ofPoint getVelocity(float step = 10);
		ofPoint velocity;
		ofxOpticalFlowFarneback flowSolver;

		ofVideoGrabber grabber;
		ofxCvGrayscaleImage gray;

		int videoWidth = 320;
		int videoHeight = 240;

		int sensorWidth = 20;
		int sensorHeight = 20;

		#define numberofsquares 12
		ofSoundPlayer soundPlayer[numberofsquares];

		int xPosition = 320 / 2 - 20 / 2;

		float yPosition[numberofsquares];
		float sensorCount[numberofsquares];

		float sensorThreshold = 0.3137; // a value between 0 and 1.

};

#pragma once

#include "ofMain.h"
#include "ofxAndroid.h"

#include "ofxOpenCv.h"

#define MAX_NUMBER_SQUARES 12
#define DEFAULT_THRESHOLD 0.3137
//#define DO_OPTICAL_FLOW

#define BUTTON_WIDTH 150
#define BUTTON_HEIGHT 100
#define DRAG_DISTANCE 10

#ifdef DO_OPTICAL_FLOW
#include "ofxOpticalFlowFarneback.h"
#endif

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

	// functions
	ofPoint getVelocity(float step = 10);
	void setVariables();

	// fields
	ofPoint velocity = {100, 100};
#ifdef DO_OPTICAL_FLOW
	ofxOpticalFlowFarneback flowSolver;
#endif

	ofVideoGrabber grabber;
	ofxCvGrayscaleImage gray;

	int videoWidth = 320;
	int videoHeight = 240;

	int sensorWidth = 20;
	int sensorHeight = 20;

	int numberofsquares = 2;
	ofSoundPlayer soundPlayer[MAX_NUMBER_SQUARES];

	int xPosition = (videoWidth / 2) - (sensorWidth / 2);

	float yPosition[MAX_NUMBER_SQUARES];
	float sensorCount[MAX_NUMBER_SQUARES];

	float sensorThreshold = DEFAULT_THRESHOLD; // a value between 0 and 1.
	float repeatRate = 0.5; // seconds

	ofPoint touchdown;
	float touchdownThreshold = sensorThreshold;
	float touchdownRepeatRate = repeatRate;
    bool continuous = true;
	float lastUpdate = 0;
	bool drawHorizontal = false;
	bool dragging = false;
};

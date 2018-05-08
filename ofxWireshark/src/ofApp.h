#pragma once

#include "ofMain.h"
#include "sharkThread.h"
#include "dataThread.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void exit();

	ofxPanel gui;
	void setupGui();
	ofxToggle	mode;
	ofxToggle	limitCapture;
	ofxIntSlider	captureSize;

	bool showGui;

	void killShark();

	ofFile dataFile;
	string systemResponse;
	string interfacesList;

	string consoleBuffer;

	bool threadOn;

	bool hasPoints;

	sharkThread myShark;
	dataThread myData;

	//void getData();

	vector< vector <string> > dataLines;

	vector<ofPoint> dataPoints;

	//vector<vector <string> > sortedData;

	void sortData();
	void uniqueIP(vector< vector<string> > uipInput);
	vector<string> uIP;
	vector <ofPoint> ipPoint;
	void assignPoints(int amtPoints);
	void drawPoints(vector <ofPoint> drawPoints);
	void drawStrings(vector <ofPoint> drawPoints);
	void drawConnections();

	bool listen;

};

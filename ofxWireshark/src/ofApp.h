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
	void exit();

	ofxPanel gui;
	void setupGui();
	ofxToggle	mode;
	ofxToggle	limitCapture;
	ofxToggle	ringToggle;
	ofxIntSlider	captureSize;

	bool showGui;

	void killShark();

	ofFile dataFile;
	string systemResponse;
	string interfacesList;

	string consoleBuffer;

	bool threadOn;

	bool hasPoints;

	bool fileFlag;

	string retrievedPath;

	string amtIP;

	sharkThread myShark;
	dataThread myData;

	vector< vector <string> > dataLines;

	vector<ofPoint> dataPoints;

	void sortData();
	void uniqueIP(vector< vector<string> > uipInput);

	vector <vector<string> > uIP;

	vector <ofPoint> ipPoint;

	void animPoints(vector <ofPoint> drawPoints, vector <vector <string> > getTime);
	void drawStrings(vector<ofPoint> drawPoints);
	void drawConnections();

	bool ringBool;

	int totCaptures;
	int amtRings;
	int oldOffsetTime;
	int animSize(int capTime);

	void protocolLine(string proto);

	int rotationSpeed;

	ofEasyCam cam;
	ofMesh mesh;
	ofMesh mLines;
};

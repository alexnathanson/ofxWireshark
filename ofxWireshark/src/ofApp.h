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

	void drawPoints(vector <ofPoint> drawPoints, vector <vector <string> > getTime);
	void drawStrings(vector<ofPoint> drawPoints);
	void drawConnections();

	bool ringBool;

	int totCaptures;
	int amtRings;
	int oldOffsetTime;
	int drawSize(int capTime, int scaler);

	void protocolLine(string proto);

	int rotationSpeed;

	ofFbo textFbo;

	ofEasyCam cam;
	ofMesh mesh;
	ofMesh mLines;
};

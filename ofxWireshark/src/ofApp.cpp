#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	threadOn = false;

	myData.setup();
	myData.startThread();

	myShark.setup("We up");
	interfacesList =  myShark.networkInterfaces;
	systemResponse = myShark.systemStream;

	hasPoints = false;

}

//--------------------------------------------------------------
void ofApp::update() {

	if (myData.loaded) {
		sortData();
		myData.loaded = false;

		uniqueIP(dataLines);

		assignPoints(uIP.size());

		hasPoints = true;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(ofColor::darkBlue);
	ofDrawBitmapString(interfacesList, 20, 20);

	ofSetColor(ofColor::darkBlue);
	ofDrawBitmapString(systemResponse, 20, 400);

	ofSetColor(ofColor::orangeRed);
	if (hasPoints) {
		drawPoints(ipPoint);
		drawStrings(ipPoint);
	}
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case 's':
		myShark.startThread();
		threadOn = true;
		dataFile.path() = myShark.writeFullPath;
		ofLogNotice("file location", myShark.writeFullPath);
		break;
	case 'e':
		myShark.stopThread();
		threadOn = false;
		break;
	case 'p':
		myShark.startShark = false;
		break;
	case 'r':
		myData.startThread();
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

void ofApp::exit() {
	if (threadOn) {
		myShark.stopThread();
	}
}

void ofApp::sortData() {
	dataLines.clear();

	//split the string into a vector by spaces
	for (int ve = 0; ve < myData.linesOfData.size(); ve++) {
		dataLines.push_back(ofSplitString(myData.linesOfData[ve], " "));
	}

	//remove empty vectors
	for (int lre = 0; lre < dataLines.size(); lre++) {
		for (int re = dataLines[lre].size() - 1; re > -1; re--) {
			if (dataLines[lre][re].empty()) {
				dataLines[lre].erase(dataLines[lre].begin() + re);
			}
		}
	}

	//should add in a way to reassamble things that we split by accident like multiword phrases inside of []
	
	/*
	ofLogNotice("Test");
	int teeest = 7;
	for (int test = 0; test < dataLines[teeest].size(); test++) {
		std::cout << dataLines[teeest][test] << endl;
	}*/
	
}

void ofApp::uniqueIP(vector< vector<string> > uipInput) {
	uIP.clear();
	bool isUnique = true;
	int wIP;

	std::cout << uipInput[0][2] << endl;
	std::cout << uipInput[0][4] << endl;

	//loop through input
	for (int u1 = 0; u1 < uipInput.size(); u1++) {
		//loop through existing ip and check against input
		wIP = 2;
		for (int u2 = 0; u2 < uIP.size(); u2++) {
			if (uIP[u2] == uipInput[u1][2]) {
				isUnique = false;
				break;
			}
		}
		if (isUnique) {
			uIP.push_back(uipInput[u1][wIP]);
		}
		isUnique = true;	

		wIP = 4;
		for (int u2 = 0; u2 < uIP.size(); u2++) {
			if (uIP[u2] == uipInput[u1][4]) {
				isUnique = false;
				break;
			}
		}
		if (isUnique) {
			uIP.push_back(uipInput[u1][wIP]);
		}
		isUnique = true;
	}

	//print results
	for(int j = 0; j < uIP.size(); j++) {
		std::cout << uIP[j] << endl;
	}
}

void ofApp::assignPoints(int amtPoints) {
	ipPoint.clear();

	for (int aP = 0; aP < amtPoints; aP++) {
		int x = ofRandom(ofGetWidth());
		int y = ofRandom(ofGetHeight());
		
		ofPoint thisPoint(x, y, 0);
		ipPoint.push_back(thisPoint);
	}

	for (int tes = 0; tes < ipPoint.size(); tes++) {
		std::cout << ipPoint[tes][0] << endl;
	}
}

void ofApp::drawPoints(vector <ofPoint> drawPoints) {
	for (int p = 0; p < drawPoints.size(); p++) {
		ofDrawEllipse(drawPoints[p], 5, 5);
	}
}


void ofApp::drawStrings(vector <ofPoint> drawPoints) {
	for (int p = 0; p < drawPoints.size(); p++) {
		ofDrawBitmapString(uIP[p], drawPoints[p]);

	}
}